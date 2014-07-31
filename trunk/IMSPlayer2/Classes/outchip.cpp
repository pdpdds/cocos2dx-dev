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
	
	���ø�(aflee04@hotmail.com), 2006/02/25

	OUTCHIP.ASM������ SndOutput() �Լ��� ������
	FMOPL ����� �Լ� ȣ��� ��ü�Ͽ����ϴ�.

	Borland C++ 5.6.4 for Win32���� �׽�Ʈ ��.

	bcc32 -c -w OUTCHIP.C

	�������ϱ� ���� MAME 0.104 �ҽ��� ���Ե� FMOPL.C 0.72��
	�ణ ������	0.72.1 ������ �ʿ��մϴ�.
	----------------------------------------------------------------------------
*/


#include "outchip.h"
#include "fmopl.h"

void *ym3812p;		// FMOPL ��� ����� ���� ���� ������

// Set the register at address 'addr' to the value 'data'.
// Take care of data register to data register write delay.
void SndOutput(int addr, int dataVal)
{
	YM3812Write(ym3812p, 0, addr);
	YM3812Write(ym3812p, 1, dataVal);
}
