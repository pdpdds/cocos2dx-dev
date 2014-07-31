/*
	ADLIB.H

	Defintions of sound chip parameters.

	1987/01/20, Marc Savary

*/

// Parameters of each voice:

#ifndef __ADLIB_H_
#define __ADLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
#define nbLocParam		14

#define prmKsl			0
#define prmMulti		1
#define prmFeedBack		2			// use for opr. 0 only
#define prmAttack		3
#define prmSustain		4
#define prmStaining		5			// Sustaining ...
#define prmDecay		6
#define prmRelease		7
#define prmLevel		8
#define prmAm			9
#define prmVib			10
#define prmKsr			11
#define prmFm			12			// use for opr. 0 only
#define prmWaveSel		13			// wave select

// globals parameters:
#define prmAmDepth		14
#define prmVibDepth		15
#define prmNoteSel		16
#define prmPercussion	17

// melodic voice numbers:
#define vMelo0			0
#define vMelo1			1
#define vMelo2			2
#define vMelo3			3
#define vMelo4			4
#define vMelo5			5
#define vMelo6			6
#define vMelo7			7
#define vMelo8			8

// percussive voice numbers:
#define BD				6
#define SD				7
#define TOM				8
#define CYMB			9
#define HIHAT			10

#define MAX_VOLUME		0x7F
#define LOG2_VOLUME		7			// log2 (MAX_VOLUME)
#define MAX_PITCH		0x3FFF
#define MID_PITCH		0x2000

#define MID_C			60			// MIDI standard mid C
#define CHIP_MID_C		48			// sound chip mid C
#define NR_NOTES		96			// # of notes we can play on chip

//------------------------------------------------------------------------------
void SoundWarmInit(void);
void SetMode(int mode);
void SetWaveSel(int state);
void SetPitchRange(int pR);
void SetGParam(int amD, int vibD, int nSel);
void SetVoiceTimbre (int voice, int *paramArray);
void SetVoiceVolume(int voice, int volume);
void SetVoicePitch (int voice, int pitchBend);
void NoteOn(int voice, int pitch);
void NoteOff(int voice);

void InitSlotParams(void);
void SetASlotParam(int slot, int param, int val);
void SetSlotParam(int slot, int *param, int waveSel);
void SetCharSlotParam (int slot, int *cParam, int waveSel);
void SndSetPrm(int slot, int prm);
void SndSetAllPrm(int slot);
void SndSKslLevel(int slot);
void SndSNoteSel(void);
void SndSFeedFm(int slot);
void SndSAttDecay(int slot);
void SndSSusRelease(int slot);
void SndSAVEK(int slot);
void SndSAmVibRhythm(void);
void SndWaveSelect(int slot);
void UpdateFNums(int voice);

#ifdef __cplusplus
}
#endif

#endif	//__ADLIB_H_
