/*
	Copyright Ad Lib Inc., 1990

	This file is part of the Ad Lib Programmer's Manual product and is
	subject to copyright laws.  As such, you may make copies of this file
	only for the purpose of having backup copies.  This file may not be
	redistributed in any form whatsoever.

	If you find yourself in possession of this file without having received
	it directly from Ad Lib Inc., then you are in violation of copyright
	laws, which is a form of theft.


	SETFREQ.ASM

	Adlib Inc, 2-mar-90

	The routine of this module is
	  int SetFreq (voice, note, pitch, keyOn)
	and is used to send frequency information to the sound board, according
	to the note, pitch bend and pitch bend range.

	Normally called by module ADLIB.C

	----------------------------------------------------------------------------
	SETFREQ.C
	
	어플리(aflee04@hotmail.com), 2006/02/25

	SETFREQ.ASM에서 주석 처리된 SetFreq() 함수의 C코드를
	그대로 옮긴 후 일부 오류를 수정함.

	Borland C++ 5.6.4 for Win32에서 테스트 함.

	bcc32 -c -w SETFREQ.C
	----------------------------------------------------------------------------
*/

#include "SetFreq.h"
#include "outchip.h"

//------------------------------------------------------------------------------
extern int pitchRange;	// pitch bend range (global)
						// ADLIB.C에 정의되어 있다

unsigned short fNumTbl[192] = {
	0x02B2, 0x02B4, 0x02B7, 0x02B9, 0x02BC, 0x02BE, 0x02C1, 0x02C3, 0x02C6, 0x02C9,
	0x02CB, 0x02CE, 0x02D0, 0x02D3, 0x02D6, 0x02D8, 0x02DB, 0x02DD, 0x02E0, 0x02E3,
	0x02E5, 0x02E8, 0x02EB, 0x02ED, 0x02F0, 0x02F3, 0x02F6, 0x02F8, 0x02FB, 0x02FE,
	0x0301, 0x0303, 0x0306, 0x0309, 0x030C, 0x030F, 0x0311, 0x0314, 0x0317, 0x031A,
	0x031D, 0x0320, 0x0323, 0x0326, 0x0329, 0x032B, 0x032E, 0x0331, 0x0334, 0x0337,
	0x033A, 0x033D, 0x0340, 0x0343, 0x0346, 0x0349, 0x034C, 0x034F, 0x0352, 0x0356,
	0x0359, 0x035C, 0x035F, 0x0362, 0x0365, 0x0368, 0x036B, 0x036F, 0x0372, 0x0375,
	0x0378, 0x037B, 0x037F, 0x0382, 0x0385, 0x0388, 0x038C, 0x038F, 0x0392, 0x0395,
	0x0399, 0x039C, 0x039F, 0x03A3, 0x03A6, 0x03A9, 0x03AD, 0x03B0, 0x03B4, 0x03B7,
	0x03BB, 0x03BE, 0x03C1, 0x03C5, 0x03C8, 0x03CC, 0x03CF, 0x03D3, 0x03D7, 0x03DA,
	0x03DE, 0x03E1, 0x03E5, 0x03E8, 0x03EC, 0x03F0, 0x03F3, 0x03F7, 0x03FB, 0x03FE,
	0xFE01, 0xFE03, 0xFE05, 0xFE07, 0xFE08, 0xFE0A, 0xFE0C, 0xFE0E, 0xFE10, 0xFE12,
	0xFE14, 0xFE16, 0xFE18, 0xFE1A, 0xFE1C, 0xFE1E, 0xFE20, 0xFE21, 0xFE23, 0xFE25,
	0xFE27, 0xFE29, 0xFE2B, 0xFE2D, 0xFE2F, 0xFE31, 0xFE34, 0xFE36, 0xFE38, 0xFE3A,
	0xFE3C, 0xFE3E, 0xFE40, 0xFE42, 0xFE44, 0xFE46, 0xFE48, 0xFE4A, 0xFE4C, 0xFE4F,
	0xFE51, 0xFE53, 0xFE55, 0xFE57, 0xFE59, 0xFE5C, 0xFE5E, 0xFE60, 0xFE62, 0xFE64,
	0xFE67, 0xFE69, 0xFE6B, 0xFE6D, 0xFE6F, 0xFE72, 0xFE74, 0xFE76, 0xFE79, 0xFE7B,
	0xFE7D, 0xFE7F, 0xFE82, 0xFE84, 0xFE86, 0xFE89, 0xFE8B, 0xFE8D, 0xFE90, 0xFE92,
	0xFE95, 0xFE97, 0xFE99, 0xFE9C, 0xFE9E, 0xFEA1, 0xFEA3, 0xFEA5, 0xFEA8, 0xFEAA,
	0xFEAD, 0xFEAF
};

// Integer division by 12 table (96 entries).  It is used to find the octave
// for a given note value in the range [0, 95].
unsigned char noteDIV12[12 * 8] = {
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	1,  1,  1,  1,  1,	1,  1,  1,  1,  1,  1,  1,
	2,  2,  2,  2,  2,  2,  2,  2,  2,  2,	2,  2,
	3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
	4,  4,  4,	4,  4,  4,  4,  4,  4,  4,  4,  4,
	5,  5,  5,  5,  5,  5,  5,  5,	5,  5,  5,  5,
	6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
	7,	7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7
};

// Integer modulo 12 table (96 entries).  It is used to find the half-tone
// value of a note ([0, 95]) within an octave.
unsigned char noteMOD12[12 * 8] = {
	 0,	 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,
	 0,	 1,  2,  3,  4,	 5,  6,  7,  8,  9, 10, 11,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,	10, 11,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,
	 0,  1,  2,	 3,  4,  5,  6,  7,  8,  9, 10, 11,
	 0,  1,  2,  3,  4,  5,  6,  7,	 8,  9, 10, 11,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,
	 0,	 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11
};

//------------------------------------------------------------------------------
//void SndOutput(int addr, int dataVal);	// outputs 'dataVal' at register 'addr'
										// OUTCHIP.C 파일에 정의되어 있다

#define NB_NOTES			96			// 8 octave of 12 notes
#define OCTAVE				12			// half-tone by octave
#define NB_TABLE_DEMI_TON	OCTAVE
#define NB_STEP_PITCH		16			// 16 steps between two half-tones
#define LOG_NB_STEP_PITCH 	4			// LOG2(NB_STEP_PITCH)
#define TABLE_SIZE 			(NB_STEP_PITCH * NB_TABLE_DEMI_TON)
#define LOG_PITCH			8			// LOG2(TABLE_SIZE)

#define KEYON_BLOCK_FNUM 	0xB0
#define FNUM_LOW 			0xA0


//------------------------------------------------------------------------------
/*
	Set the frequency of voice 'voice' to note number 'note', shifting
	the note by 'pitch/0x2000' of 'pitchRange' (global, 1-12). The key-on bit
	is set to the passed 'keyOn' value.
	Returns the value written to register 0xB0+voice.
*/
//------------------------------------------------------------------------------

int SetFreq(short voice, unsigned short note, short pitch, short keyOn)
{
/*
	short voice;			// [0 - 8]
	unsigned short note;	// [0, 95]
	short pitch;			// 14 bits, [0, 3fff]
	short keyOn;			// key on bit-feild == 0 or 0x20
*/
	short tblValue, tNote;
	unsigned short t1, t2;
	unsigned short tableOff;
//	unsigned short fNLow, fNHigh;
	short block;
	short signP;
	int returnValue;

	// Convert unsigned pitch bend to [-0x2000, +0x2000] signed.
	signP = pitch - 0x2000;
	// Keep the 8 most significant bits of the pitch bend.
	t2 = signP >> (13 - LOG_PITCH);
	// Multiply the modified pitch bend by the pitch bend range to obtain the
	// signed note number offset in 1/256's (8 bit fix point).
	t2 = t2 * pitchRange;
	// convert note number in 8 bit fix point and add preceding computed note offset:
	t1 = note << LOG_PITCH;
	tNote = t1 + t2;
	// round note number to 1/16  (by adding 1/32):
	tNote = tNote + (1 << (LOG_PITCH - LOG_NB_STEP_PITCH - 1));
	// Convert 8 bit fix point to 4 bit fix point (1/16).
	// 'tNote' is then the resulting note number after adjusting for pitch bend,
	// expressed in 1/16 half-tones.
	tNote = tNote >> (LOG_PITCH - LOG_NB_STEP_PITCH);
	// Make sure that the note is in the range [0, 95].
	if (tNote < 0) tNote = 0;
	else {
		// Compare with 96 in 4 bit fix point.
		if (tNote >= (NB_NOTES << LOG_NB_STEP_PITCH) - 1)
			tNote = (NB_NOTES << LOG_NB_STEP_PITCH) - 1;
	}
	// Get half-tone value within 1 octave by using integer part of note number MODULO 12.
	tableOff = noteMOD12[(tNote >> LOG_NB_STEP_PITCH)];
	// Convert half-tone number to offset in table 'tableOff', in order
	// to point to start of data for this half-tone.
	// Since each half-tone consisting of 16 entries of 2 bytes, we multiply by 32.
	tableOff = tableOff << (LOG_NB_STEP_PITCH + 1);
	// Add to this offset the fractional part (low order 4 bits)
	// of note number multiplied by 2 to obtain the exact offset in 'tableOff'
	// for the note number.
	tableOff = tableOff + ((tNote << 1) & (NB_STEP_PITCH * 2 - 1));
	// Get frequency information pointed to...
	tblValue = *(unsigned short *)((unsigned char *)fNumTbl + tableOff);
	// Get octave number using integer part of note number and DIV 12 table.
	block = noteDIV12[(tNote >> LOG_NB_STEP_PITCH)] - 1;
	// If high bit of frequency information is set, this means that we must
	// add 1 to octave number (frequency information has been divided by 2).
	if (tblValue < 0) block++;
	if (block < 0) {
		block++;
		tblValue = tblValue >> 1;
	}
	// Send low bits of frequency information to sound board.
	SndOutput(FNUM_LOW + voice, tblValue & 0xFF);
	returnValue = keyOn + (block << 2) + ((tblValue >> 8) & 3);
	// Send 'keyOn' bit, octave number and 2 high bits of frequency information
	// to sound board.
	SndOutput(KEYON_BLOCK_FNUM + voice, returnValue);

	return returnValue;
}
