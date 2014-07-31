#ifndef _BNK_H_
#define _BNK_H_


#include <string>

#pragma pack(push, 1)
#pragma pack(1)
typedef struct {
	char verMajor;		// 1 byte
	char verMinor;		// 1 byte
	char signature[6];	// 6 byte
	unsigned short usedEntry;		// 2 byte
	unsigned short totalEntry;	// 2 byte
	unsigned int recSeekPos;	// 4 byte
	unsigned int dataSeekPos;	// 4 byte
} MYADLIB_BNK_HEADER;	// 20 byte

typedef struct {
	// index into data section
	unsigned short	index;		// 2 byte
	char used;	 // 1 byte
	char name[8+1]; // NULL terminated instrument name
} MYADLIB_BNK_NAME_RECORD;	// 2 + 1 + 9 = 12 byte

typedef struct {
	char keyScaleLevel;
	char freqMultiplier;
	char feedback;
	char attackRate;
	char sustainLevel;
	char sustainSound;
	char decayRate;
	char releaseRate;
	char outputLevel;
	char amplitudeVibrato;
	char freqVibrato;
	char envelopeScale;
	char fmType;			// 0:FM, 1:Additive
} OP_PARAM;	// 13 byte

typedef struct {
	char instType;		// 1 byte
	char voiceNum;		// 1 byte
	OP_PARAM op1;			// 13 byte
	OP_PARAM op2;			// 13 byte
	char op1WaveForm;	// 1 byte
	char op2WaveForm;	// 1 byte
} MYADLIB_BNK_INST_RECORD;				// 30 byte
#pragma pack(pop)

typedef struct {
	int keyScaleLevel;
	int freqMultiplier;
	int feedback;
	int attackRate;
	int sustainLevel;
	int sustainSound;
	int decayRate;
	int releaseRate;
	int outputLevel;
	int amplitudeVibrato;
	int freqVibrato;
	int envelopeScale;
	int fmType;
} OP_PARAM32;	// 4 byte X 13개

typedef struct {
	int	instType;
	int	voiceNum;
	OP_PARAM32 op1;
	OP_PARAM32 op2;
	int	op1WaveForm;
	int	op2WaveForm;
} MYADLIB_BNK_INST_RECORD32;		// 4 byte X 30개

typedef struct {
	char *raw; // bnk 파일 내용 전체를 통째로 읽어올 버퍼
    char *rawPtr;

	MYADLIB_BNK_HEADER header;
	MYADLIB_BNK_NAME_RECORD	*nameRecord;
	MYADLIB_BNK_INST_RECORD	*instRecord;
	MYADLIB_BNK_INST_RECORD32 *instRecord32;

	int	nameRecordSize;
	int	instRecordSize;
	int	instRecordSize32;
} MYADLIB_BNK;

class Bnk
{
public:
	MYADLIB_BNK *m_bnk;
	std::string m_fileName; // 경로를 포함한 파일명
	int	m_fileSize; // raw 버퍼로 읽어들인 실제 크기

public:
	Bnk();
	~Bnk();
	int GetTotalInst();
	MYADLIB_BNK *GetBnk();
	bool Open(std::string file);
};

#endif	//__BNK_H_
