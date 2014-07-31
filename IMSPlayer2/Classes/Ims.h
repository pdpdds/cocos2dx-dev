#ifndef __IMS_H_
#define __IMS_H_

#include <string>
#include "Bnk.h"

#define MAX_VOICE 11

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
#pragma pack(1)
typedef struct {
	unsigned char nVerMajor;		// 1 byte
	unsigned char nVerMinor;		// 1 byte
	long int nTuneID;				// 4 byte
	char szTuneName[30];			// 30 byte
	unsigned char nTickBeat;		// 1 byte
	unsigned char nBeatMeasure;		// 1 byte
	long int nTotalTick;			// 4 byte
	long int cbDataSize;			// 4 byte
	long int nrCommand;				// 4 byte
	unsigned char nSrcTickBeat;		// 1 byte
	char filler[7];					// 7 byte
	unsigned char nSoundMode;		// 1 byte
	unsigned char nPitchBRange;		// 1 byte
	unsigned short int nBasicTempo;	// 2 byte
	char filler2[8];				// 8 byte
} IMS_HEADER;						// 70 byte
#pragma pack(pop)

typedef struct {
	char name[8+1];
    int	index;						// BNK 파일에서의 인덱스 번호
} IMS_INST;

typedef struct {
	unsigned char *raw;				// IMS 파일 내용 전체를 통째로 읽어올 버퍼
	unsigned char *rawPtr;
//	----------------------------------------------------------------------------
	IMS_HEADER header;
	unsigned char *songData;
//	----------------------------------------------------------------------------
	int instCount;					// 사용된 악기수
	IMS_INST *instIndex; 
//	----------------------------------------------------------------------------
	MYADLIB_BNK	*m_bnk;
} MYADLIB_IMS;

#ifdef __cplusplus
}
#endif

class Ims
{
public:
	MYADLIB_IMS *m_ims;
	
	std::string m_fileName; 	// 경로를 포함한 파일명
	int	m_fileSize;			// raw 버퍼로 읽어들인 실제 크기

	int m_basicTempo;
	int	m_songDataIndex;
	int	m_songDataTotalIndex;
	int m_tick;

public:
	Ims();
	~Ims();

	void Init();
	long GetTotalTick();
	long GetDuration();
	char* GetTitle();
	char *GetTitleJohab();
	int GetBasicTempo();
	int GetCurrentTick();
	bool Open(std::string file);
	void Rewind(void);
	int PlayEvent(void);
	bool MatchBnk(MYADLIB_BNK *bnk);
	bool Reset();
};

#endif	//__IMS_H_
