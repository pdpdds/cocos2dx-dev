#ifndef __OUTCHIP_H_
#define __OUTCHIP_H_

#ifdef __cplusplus	// 추가
extern "C" {		// 추가
#endif				// 추가

extern void *ym3812p;

void SndOutput(int addr, int dataVal);

#ifdef __cplusplus
}
#endif

#endif	//__OUTCHIP_H_

