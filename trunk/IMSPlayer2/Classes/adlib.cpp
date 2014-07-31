/*
	 Copyright Ad Lib Inc., 1990

	 This file is part of the Ad Lib Programmer's Manual product and is
	 subject to copyright laws.  As such, you may make copies of this file
	 only for the purpose of having backup copies.  This file may not be
	 redistributed in any form whatsoever.

	 If you find yourself in possession of this file without having received
	 it directly from Ad Lib Inc., then you are in violation of copyright
	 laws, which is a form of theft.
*/

/**
	ADLIB.C, low level sound driver V1.04
	===== To be used with OUTCHIP.ASM & SETFREQ.ASM files. =====

	Copyright Ad Lib Inc, 1988, 1989, 1990

	1988/06/22, Marc Savary.
	1989/04/20, Marc savary.
	1990/03/15, Marc Savary.	Modification of frequency calculation (SETFREQ.ASM)
								Correction of volume bugs on additive timbres

	The following routines are public (see each routine for further
	documentation):

		SoundWarmInit()
		SetMode(mode)
		SetWaveSel(state)
		SetPitchRange(pR)
		SetGParam(amD, vibD, nSel)
		SetVoiceTimbre(voice, paramArray)
		SetVoiceVolume(voice, volume)
		SetVoicePitch(voice, pitchBend)
		NoteOn(voice, pitch)
		NoteOff(voice)
**/

#include "fmopl.h"
#include "adlib.h"
#include "outchip.h"
#include "setfreq.h"



//------------------------------------------------------------------------------
/*
	In percussive mode, the last 4 voices  (SD TOM HH CYMB) are created
	using melodic voices 7 & 8. A noise generator use channels 7 & 8
	frequency information for creating rhythm instruments. Best results
	are obtained by setting TOM two octaves below mid C and SD 7 half-tones
	above TOM.
	In this implementation, only the TOM pitch may vary, with the SD always
	set 7 half-tones above it.
*/
//------------------------------------------------------------------------------
#define TOM_PITCH				24	// best frequency, in range of 0 to 95
#define TOM_TO_SD				7	// 7 half-tones between voice 7 & 8
#define SD_PITCH				(TOM_PITCH + TOM_TO_SD)

#define GetLocPrm(slot, prm)	(paramSlot[slot][prm])

//------------------------------------------------------------------------------
int pitchRange;			// pitch variation, half-tone[+1,+12]
int modeWaveSel;		// != 0 if used with the 'wave-select' parameters
int percBits;			// control bits of percussive voices
int percMasks[5] = {0x10, 0x08, 0x04, 0x02, 0x01};
int voiceNote[9];		// pitch of last note-on of each voice
int voiceKeyOn[9];		// state of keyOn bit of each voice
int vPitchBend[9];		// current pitch bend of each voice
int bxRegister[9];		// current val. of 0xB0 - 0xB8 reg
int lVoiceVolume[11];	// volume for each of 11 logical voices
int modeVoices;			// 9 or 11, depending on 'percussion'

// definition of the ELECTRIC-PIANO voice (opr0 & opr1)
int pianoParamsOp0[nbLocParam] = {1, 1, 3, 15, 5, 0, 1, 3, 15, 0, 0, 0, 1, 0};
int pianoParamsOp1[nbLocParam] = {0, 1, 1, 15, 7, 0, 2, 4, 0, 0, 0, 1, 0, 0};

// definition of default percussive voices:
int bdOpr0[14]	= {0,  0, 0, 10,  4, 0, 8, 12, 11, 0, 0, 0, 1, 0};
int bdOpr1[14]	= {0,  0, 0, 13,  4, 0, 6, 15,  0, 0, 0, 0, 1, 0};
int sdOpr[14]	= {0, 12, 0, 15, 11, 0, 8,  5,  0, 0, 0, 0, 0, 0};
int tomOpr[14]	= {0,  4, 0, 15, 11, 0, 7,  5,  0, 0, 0, 0, 0, 0};
int cymbOpr[14]	= {0,  1, 0, 15, 11, 0, 5,  5,  0, 0, 0, 0, 0, 0};
int hhOpr[14]	= {0,  1, 0, 15, 11, 0, 7,  5,  0, 0, 0, 0, 0, 0};

int paramSlot[18][nbLocParam];	// all the parameters of slots...
int amDepth;					// chip global parameters ..
int vibDepth;					// ...
int noteSel;					// ...
int percussion;					// percussion mode parameter

// Slot numbers as a function of the voice and the operator. (melodic mode only)
int slotMVoice[9][2] = {
	{0, 3},		/* voix 0 */
	{1, 4},		/* 1 */
	{2, 5},		/* 2 */
	{6, 9},		/* 3 */
	{7, 10},	/* 4 */
	{8, 11},	/* 5 */
	{12, 15},	/* 6 */
	{13, 16},	/* 7 */
	{14, 17}	/* 8 */
};

/* Slot numbers for the percussive voices.
   255 indicates that there is only one slot used by a voice.	*/
int slotPVoice[11][2] = {
	{0, 3},		/* voice 0 */
	{1, 4},		/* 1 */
	{2, 5},		/* 2 */
	{6, 9},		/* 3 */
	{7, 10},	/* 4 */
	{8, 11},	/* 5 */
	{12, 15},	/* Bass Drum: slot 12 et 15 */
	{16, 255},	/* SD: slot 16 */
	{14, 255},	/* TOM: slot 14 */
	{17, 255},	/* TOP-CYM: slot 17 */
	{13, 255}	/* HH: slot 13 */
};

/* 	This table gives the offset of each slot within the chip.
	offset = fn (slot)	*/
int offsetSlot[18] = {
	 0,  1,  2,  3,  4,  5,
	 8,  9, 10, 11, 12, 13,
	16, 17, 18, 19, 20, 21
};

/* This table indicates if the slot is a modulator (operator 0) or a
   carrier (operator 1).
   opr = fn (slot)	*/
int carrierSlot[18] = {
	0, 0, 0,		/* 1 2 3 */
	1, 1, 1,		/* 4 5 6 */
	0, 0, 0, 		/* 7 8 9 */
	1, 1, 1, 		/* 10 11 12 */
	0, 0, 0, 		/* 13 14 15 */
	1, 1, 1,		/* 16 17 18 */
};

/* This table gives the voice number associated with each slot.
   (melodic mode only)
   voice = fn (slot)	*/
int voiceMSlot[18] = {
	0, 1, 2,
	0, 1, 2,
	3, 4, 5,
	3, 4, 5,
	6, 7, 8,
	6, 7, 8,
};

/* This table gives the voice number  (0-10) associated with each
   slot (percussive mode only),
   voice = fn (slot)	*/
int voicePSlot[18] = {
	0, 1, 2,
	0, 1, 2,
	3, 4, 5,
	3, 4, 5,
	BD, HIHAT, TOM,
	BD, SD, CYMB
};


//------------------------------------------------------------------------------
/*
	Initialize the chip in melodic mode (mode == 0),
	set all 9 voices to electric-piano timbres,
	set the 3 global parameters to zero,
	set the pitch bend range to 1 half-tone,
	set the pitch bend of each voice to 0x2000 (no detune),
	set the volume of each voice to maximum level,
	and enable the wave-select parameter.
*/
//------------------------------------------------------------------------------
void SoundWarmInit(void)
{
	int i;

	for (i = 1; i <= 0xF5; i++)	SndOutput(i, 0);   // clear all registers

	SndOutput(4, 6);	// mask T1 & T2

	for (i = 0; i < 9; i++)  {	// pitch bend for each voice = no detune
		vPitchBend[i] = MID_PITCH;
		voiceKeyOn[i] = 0;
		voiceNote[i] = 0;
	}

	for (i = 0; i < 11; i++) lVoiceVolume[i] = MAX_VOLUME;

	SetMode(0);                // melodic mode
	SetGParam(0, 0, 0);        // init global parameters
	SetPitchRange(1);          // default pitch range is 1 half-tone
	SetWaveSel(1);
}
//------------------------------------------------------------------------------
/*
	Put the chip in melodic mode (mode == 0),
	or in percussive mode  (mode != 0).

	If the melodic mode is chosen, all voices are
	set to electric-piano, else the first 5 are set
	to electric-piano, and the percussion voices
	to their default timbres.
*/
//------------------------------------------------------------------------------
void SetMode(int mode)
{
	if (mode) {
		// set the frequency for the last 4 percussion voices:
		voiceNote[TOM] = TOM_PITCH;
		vPitchBend[TOM] = MID_PITCH;
		UpdateFNums(TOM);

		voiceNote[SD] = SD_PITCH;
		vPitchBend[SD] = MID_PITCH;
		UpdateFNums(SD);
	}
	percussion = mode;
	modeVoices = mode ? 11 : 9;
	percBits = 0;

	InitSlotParams();
	SndSAmVibRhythm();
}
//------------------------------------------------------------------------------
/*
	Enable (state != 0) / disable (state == 0)
	the wave-select parameters.

	If you do not want to use the wave-select parameters, call
	this function with a value of 0 AFTER calling SoundColdInit()
	or SoundWarmInit().
*/
//------------------------------------------------------------------------------
void SetWaveSel(int state)
{
	int i;

	modeWaveSel = state ? 0x20 : 0;
	for (i = 0; i < 18; i++) SndOutput(0xE0 + offsetSlot[i], 0);
	SndOutput(1, modeWaveSel);
}
//------------------------------------------------------------------------------
/*
	Routine to change the pitch bend range. The value can be from
	1 to 12 (in half-tones).

	For example, the value 12 means that the pitch bend will
	range from -12 (pitchBend == 0, see function 'SetVoicePitch()')
	to +12 (pitchBend == 0x3fff) half-tones.

	The change will be effective as of the next call to
	'SetVoicePitch()'.
*/
//------------------------------------------------------------------------------
void SetPitchRange(int pR)
{
	if (pR > 12) pR = 12;
	if (pR < 1) pR = 1;
	pitchRange = pR;
}
//------------------------------------------------------------------------------
/*
	Set the 3 global parameters AmDepth,
	VibDepth & NoteSel

	The change takes place immediately.
*/
//------------------------------------------------------------------------------
void SetGParam(int amD, int vibD, int nSel)
{
	amDepth = amD;
	vibDepth = vibD;
	noteSel = nSel;

	SndSAmVibRhythm();
	SndSNoteSel();
}
//------------------------------------------------------------------------------
/*
	Set the parameters of the voice 'voice'.

	In melodic mode, 'voice' varies from 0 to 8.
	In percussive mode, voices 0 to 5 are melodic
	and 6 to 10 are percussive.

	A timbre (melodic or percussive) is defined as follows:
	the 13 first parameters of operator 0  (ksl, multi, feedBack,
	attack, sustain, eg-typem decay, release, level, am, vib, ksr, fm)
	followed by the 13 parameters of operator 1 (if a percussive voice, all
	the parameters are zero), followed by the wave-select parameter for
	the operators 0 and 1.

	'paramArray' is structured as follows:
		struct {
			int opr0Prm[13];       first 13 parameters
			int opr1Prm[13];       must be 0 if percussive timbre
			int	opr0WaveSel;		last parameter
			int opr1WaveSel;		must be 0 if percussive timbre
		} TimbreDef;

	The old timbre files (*.INS) do not contain the parameters
	'opr0WaveSel' and 'opr1WaveSel'.
	Set these two parameters to zero if you are using the old file
	format.
*/
//------------------------------------------------------------------------------
void SetVoiceTimbre(int voice, int *paramArray)
{
	int wave0, wave1;
	int *prm1, *wavePtr;
	int *slots;

	if (voice >= modeVoices) return;

	wavePtr = paramArray + 2 * (nbLocParam - 1);
	wave0 = *wavePtr, wavePtr++;
	wave1 = *wavePtr;
	prm1 = paramArray + nbLocParam - 1;

	if (percussion) slots = slotPVoice[voice];
	else slots = slotMVoice[voice];
	SetSlotParam(slots[0], paramArray, wave0);
	if (slots[1] != 255) SetSlotParam(slots[1], prm1, wave1);
}
//------------------------------------------------------------------------------
/*
	Set the volume of the voice 'voice' to 'volume'.

	The resulting output level is (timbreVolume * volume / 127).
	The change takes place immediately.

	0 <= volume <= 127
*/
//------------------------------------------------------------------------------
void SetVoiceVolume(int voice, int volume)	// volume : 0 - 0x7F
{
	int *slots;

	if (voice >= modeVoices) return;
	if (volume > MAX_VOLUME) volume = MAX_VOLUME;
	lVoiceVolume[voice] = volume;
	if (percussion) slots = slotPVoice[voice];
	else slots = slotMVoice[voice];
	SndSKslLevel(slots[0]);
	if (255 != slots[1]) SndSKslLevel(slots[1]);
}
//--------------------------------------------------------------------------------
/*
	Change the pitch value of a voice.

	The variation in pitch is a function of the previous call
	to 'SetPitchRange()' and the value of 'pitchBend'.
	A value of 0 means -half-tone * pitchRange,
	0x2000 means no variation (exact pitch) and
	0x3fff means +half-tone * pitchRange.

	Does not affect the percussive voices, except for the bass drum.

	The change takes place immediately.

	0 <= pitchBend <= 0x3fff, 0x2000 == exact tuning
*/
//------------------------------------------------------------------------------
void SetVoicePitch(int voice, int pitchBend)
{
	if ((!percussion && voice < 9) || (voice <= BD)) {
		/* melodic + bass-drum */
		if (pitchBend > MAX_PITCH) pitchBend = MAX_PITCH;
		vPitchBend[voice] = pitchBend;
		UpdateFNums(voice);
	}
}
//------------------------------------------------------------------------------
/*
	Routine to start a note playing.

	0 <= voice <= 8	in melodic mode,
	0 <= voice <= 10 in percussive mode;
	0 <= pitch <= 127, 60 == MID_C  (the card can play between 12 and 107 )
*/
//------------------------------------------------------------------------------
void NoteOn(int voice, int pitch)	// pitch : 0 - 127
{
	pitch -= (MID_C - CHIP_MID_C);
	if (pitch < 0) pitch = 0;

	if ((!percussion && voice < 9) || (voice < BD)) {
		// this is a melodic voice
		voiceNote[voice] = pitch;
		voiceKeyOn[voice] = 0x20;
		UpdateFNums(voice);
	} else if (percussion && (voice <= HIHAT)) {
		// this is a percussive voice
		if (voice == BD) {
			voiceNote[BD] = pitch;
			UpdateFNums(voice);
		} else if (voice == TOM) {
			// for the last 4 percussions, only the TOM may change in frequency,
			// modifying the three others:
			if (voiceNote[TOM] != pitch) {
				voiceNote[TOM] = pitch;
				voiceNote[SD] = pitch +TOM_TO_SD;
				UpdateFNums(TOM);
				UpdateFNums(SD);
			}
		}
		percBits = percBits | percMasks[voice - BD];
		SndSAmVibRhythm();
	}
}
//------------------------------------------------------------------------------
/*
	Routine to stop playing the note which was started in 'NoteOn()'.

	0 <= voice <= 8	in melodic mode,
	0 <= voice <= 10 in percussive mode;
*/
//------------------------------------------------------------------------------
void NoteOff(int voice)
{
	if ((!percussion && voice < 9) || (voice < BD)) {
		voiceKeyOn[voice] = 0;
		bxRegister[voice] = bxRegister[voice] & ~0x20;
		SndOutput(0xB0 + voice, bxRegister[voice]);
	}
	else if (percussion && (voice <= HIHAT)) {
		percBits = percBits & ~percMasks[voice - BD];
		SndSAmVibRhythm();
	}
}

//------------------------------------------------------------------------------
//	static functions ...
//------------------------------------------------------------------------------

/*
	In melodic mode, initialize all voices to electric-pianos.

	In percussive mode, initialize the first 6 voices to electric-pianos
	and the percussive voices to their default timbres.
*/
void InitSlotParams(void)
{
	int i;

	for (i = 0; i < 18; i++) {
		if (carrierSlot[i]) SetCharSlotParam(i, pianoParamsOp1, 0);
		else SetCharSlotParam(i, pianoParamsOp0, 0);
	}
	if (percussion) {
		SetCharSlotParam(12, bdOpr0, 0);
		SetCharSlotParam(15, bdOpr1, 0);
		SetCharSlotParam(16, sdOpr, 0);
		SetCharSlotParam(14, tomOpr, 0);
		SetCharSlotParam(17, cymbOpr, 0);
		SetCharSlotParam(13, hhOpr, 0);
	}
}
// Used to change the parameter 'param' of the slot 'slot'
// with the value 'val'. The chip registers are updated.
void SetASlotParam(int slot, int param, int val)	// param: parameter number
{
	paramSlot[slot][param] = val;
	SndSetPrm(slot, param);
}
// Set the 14 parameters  (13 in 'param', 1 in 'waveSel')
// of slot 'slot'. Update the parameter array and the chip.
void SetSlotParam(int slot, int *param, int waveSel)
{
	int i, *ptr;

	for (i = 0, ptr = &paramSlot[slot][0]; i < nbLocParam - 1; i++) {
		*ptr = *param;
		ptr++, param++;
	}
	*ptr = (waveSel &= 0x3);
	SndSetAllPrm(slot);
}

void SetCharSlotParam(int slot, int *cParam, int waveSel)
{
	int i, param[nbLocParam];

	for (i = 0; i < nbLocParam - 1; i++) param[i] = *cParam, cParam++;
	SetSlotParam(slot, param, waveSel);
}
// Update the parameter 'prm' for the slot 'slot'.	Update the chip registers.
void SndSetPrm(int slot, int prm)
{
	switch (prm) {
	case prmPercussion:
	case prmAmDepth:
	case prmVibDepth:	SndSAmVibRhythm();		break;
	case prmNoteSel:	SndSNoteSel();			break;
	case prmKsl:
	case prmLevel:		SndSKslLevel(slot);		break;
	case prmFm:
	case prmFeedBack:	SndSFeedFm(slot);		break;
	case prmAttack:
	case prmDecay:		SndSAttDecay(slot);		break;
	case prmRelease:
	case prmSustain:	SndSSusRelease(slot);	break;
	case prmMulti:
	case prmVib:
	case prmStaining:
	case prmKsr:
	case prmAm:			SndSAVEK(slot);			break;
	case prmWaveSel:	SndWaveSelect(slot);	break;
	}
}
// Transfer all the parameters from slot 'slot' to	the chip.
void SndSetAllPrm(int slot)
{
	SndSAmVibRhythm();
	SndSNoteSel();
	SndSKslLevel(slot);
	SndSFeedFm(slot);
	SndSAttDecay(slot);
	SndSSusRelease(slot);
	SndSAVEK(slot);
	SndWaveSelect(slot);
}
// Write to the register which controls output level and does
// key-on/key-offs for the percussive voice slots.
void SndSKslLevel(int slot)
{
	int t1, vc, singleSlot;

	if (percussion) vc = voicePSlot[slot];
	else vc = voiceMSlot[slot];

	t1 = 63 - (GetLocPrm(slot, prmLevel) & 63);		// amplitude
	singleSlot = percussion && (vc > BD);

	if  ((carrierSlot[slot] || !GetLocPrm(slot, prmFm) || singleSlot))
		// Change the 0 - 127 volume change value to 0 - 63 for the chip.
		// (MAX_VOLUME+1)/2 is added to avoid round-off errors.
		t1 = (t1 * lVoiceVolume[vc] + (MAX_VOLUME + 1) / 2 ) >> LOG2_VOLUME;

	t1 = 63 - t1;
	t1 = t1 | (GetLocPrm(slot, prmKsl) << 6);
	SndOutput(0x40 + offsetSlot[slot], t1);
}
// Write to the register which controls the note select parameter.
void SndSNoteSel(void)
{
	SndOutput(0x08, noteSel ? 64 : 0);
}
// FEED-BACK and FM (connection). Applicable only to operator 0 in melodic mode.
void SndSFeedFm(int slot)
{
	int t1;

	if (carrierSlot[slot]) return;
	t1 = GetLocPrm(slot, prmFeedBack) << 1;
	t1 = t1 | (GetLocPrm(slot, prmFm) ? 0 : 1);
	SndOutput(0xC0 + voiceMSlot[slot], t1);
}
// ATTACK, DECAY
void SndSAttDecay(int slot)
{
	int t1;

	t1 = GetLocPrm(slot, prmAttack) << 4;
	t1 = t1 | (GetLocPrm(slot, prmDecay) & 0xf);
	SndOutput(0x60 + offsetSlot[slot], t1);
}
// SUSTAIN, RELEASE
void SndSSusRelease(int slot)
{
	int t1;

	t1 = GetLocPrm(slot, prmSustain) << 4;
	t1 = t1 | (GetLocPrm(slot, prmRelease) & 0xf);
	SndOutput(0x80 + offsetSlot[slot], t1);
}
// AM, VIB, EG-TYP (Sustaining), KSR, MULTI
void SndSAVEK(int slot)
{
	int t1;

	t1 = GetLocPrm(slot, prmAm) ? 0x80 : 0;
	t1 = t1 + (GetLocPrm(slot, prmVib) ? 0x40 : 0);
	t1 = t1 + (GetLocPrm(slot, prmStaining) ? 0x20 : 0);
	t1 = t1 + (GetLocPrm(slot, prmKsr) ? 0x10 : 0);
	t1 = t1 + (GetLocPrm(slot, prmMulti) & 0xf);
	SndOutput(0x20 + offsetSlot[slot], t1);
}
// Set the values: AM Depth, VIB depth & Rhythm (melo/perc mode)
void SndSAmVibRhythm(void)
{
	int t1;

	t1 = amDepth ? 0x80 : 0;
	t1 = t1 | (vibDepth ? 0x40 : 0);
	t1 = t1 | (percussion ? 0x20 : 0);
	t1 = t1 | percBits;
	SndOutput(0xBD, t1);
}
// Set the wave-select parameter.
void SndWaveSelect(int slot)
{
	int wave;

	if (modeWaveSel) wave = GetLocPrm(slot, prmWaveSel) & 0x03;
	else wave = 0;
	SndOutput(0xE0 + offsetSlot[slot], wave);
}
// Change pitch, pitchBend & keyOn of voices 0 to 8, for melodic or percussive mode.
void UpdateFNums(int voice)
{
	int i;
	bxRegister[voice] = SetFreq(voice, voiceNote[voice], vPitchBend[voice], voiceKeyOn[voice]);

	if (voice <= BD) {
		i = vPitchBend[voice] + 0x800;
		if (i > MAX_PITCH) i = MAX_PITCH;
		bxRegister[voice] = SetFreq(voice, voiceNote[voice], i, voiceKeyOn[voice]);
	}
}
