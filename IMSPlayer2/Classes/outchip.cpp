/*
	Copyright Ad Lib Inc., 1990

	This file is part of the Ad Lib Programmer's Manual product and is
	subject to copyright laws.  As such, you may make copies of this file
	only for the purpose of having backup copies.  This file may not be
	redistributed in any form whatsoever.

	If you find yourself in possession of this file without having received
	it directly from Ad Lib Inc., then you are in violation of copyright
	laws, which is a form of theft.


	OUTCHIP.ASM

	Adlib Inc, 20-avr-89

	----------------------------------------------------------------------------
	OUTCHIP.C
	
	어플리(aflee04@hotmail.com), 2006/02/25

	OUTCHIP.ASM에서의 SndOutput() 함수의 내용을
	FMOPL 모듈의 함수 호출로 대체하였습니다.

	Borland C++ 5.6.4 for Win32에서 테스트 함.

	bcc32 -c -w OUTCHIP.C

	컴파일하기 위해 MAME 0.104 소스에 포함된 FMOPL.C 0.72의
	약간 수정된	0.72.1 버전이 필요합니다.
	----------------------------------------------------------------------------
*/


#include "outchip.h"
#include "fmopl.h"

void *ym3812p;		// FMOPL 모듈 사용을 위해 새로 정의함

// Set the register at address 'addr' to the value 'data'.
// Take care of data register to data register write delay.
void SndOutput(int addr, int dataVal)
{
	YM3812Write(ym3812p, 0, addr);
	YM3812Write(ym3812p, 1, dataVal);
}
