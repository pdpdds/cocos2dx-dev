#ifndef __ROL_H_
#define __ROL_H_

#include "datatype.h"
#include "bnk.h"

#define MAX_VOICE 11

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(1)
//------------------------------------------------------------------------------
// ① Header (201 byte)
//------------------------------------------------------------------------------
typedef struct {
	WORD	FileVersion_Major;
	WORD	FileVersion_Minor;
	char	Filler1[40];			// "\roll\default"
	WORD	TicksPerBeat;			// Ticks/Beats : 박자당 Ticks 수 (2 바이트)
	WORD	BeatsPerMeasure;		// Beats/Mesure : 마디당 박자 수 (2 바이트)
	WORD	Editing_Scale_Y;
	WORD	Editing_Scale_X;
	char	Filler2;
	char	SoundMode;			// 0 = percussive mode, 1 = melodic mode
	char	Filler3[90];		// ROL 파일마다 다른 정보를 가지고 있으나 어떤 용도의 정보인지는 모르겠음.
	char	Filler4[38];		// 0으로 채워진 38 바이트
	char	Filler5[15];		// "Tempo"로 시작하고 나머지는 0으로 채워진 15 바이트
	float	BasicTempo;
} TROLHeader;
//------------------------------------------------------------------------------
// ③ Tempo Events (가변. TempoEventsCount 수만큼 존재함)
//------------------------------------------------------------------------------
typedef struct {
	WORD	TimeOfEvents;		// time of events, in ticks
	float	TempoMultiplier;	// tempo multiplier (0.01 ~ 10.0)
} TTempoEvents;
//------------------------------------------------------------------------------
//④-1. Note Duration (4 byte)
//------------------------------------------------------------------------------
/*
	NoteNumber : 0 => silence
				 12 ~ 107 => normal note
				 (you must subtract 60 to obtain the correct value for the sound driver)
	NoteDuration : note duration, in ticks

	TNoteDuration 레코드는 NoteDuration 필드의 누적 합계가
	TimeOfLastNote이 될 때까지의 반복된다.
*/
//------------------------------------------------------------------------------
typedef struct {
	WORD	NoteNumber;
	WORD	NoteDuration;
} TNoteDuration;
//------------------------------------------------------------------------------
// ④-2. Instrument Events (14 byte)
//------------------------------------------------------------------------------
typedef struct {
	WORD	TimeOfEvents;		// time of events, in ticks
	char	InstrName[8 + 1];
	char	Filler;
	WORD	unused;		// 악기 인덱스??
} TInstrEvents;
//------------------------------------------------------------------------------
// ④-3. Volume Events (6 byte)
//------------------------------------------------------------------------------
typedef struct {
	WORD    TimeOfEvents;		// in ticks
	float	VolumeMultiplier;	// (0.0 - 1.0)
} TVolumeEvents;
//------------------------------------------------------------------------------
// ④-4. Pitch Events (6 byte)
//------------------------------------------------------------------------------
typedef struct {
	WORD	TimeOfEvents;		// in ticks
	float	PitchVariation;		// (0.0 - 2.0, nominal is 1.0)
} TPitchEvents;
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#define MAXVOICE	11		// 총 11개의 채널이 있음을 의미함

typedef struct {
	// ④-1. Note Duration
	char			Filler1[15];
	WORD			TimeOfLastNote;		// time (in ticks) of last note + 1
    int				NoteCount;			// ★파일에 없는 정보(계산하여 구함)
	TNoteDuration  *Note;		// 가변
	// ④-2. Instrument Events
	char			Filler2[15];
	WORD			InstrCount;
	TInstrEvents   *Instr;		// 가변
	int			   *InstrIndex;			// ★파일에 없는 정보
	// ④-3. Volume Events
	char			Filler3[15];
	WORD			VolumeCount;
	TVolumeEvents  *Volume;		// 가변
	// ④-4. Pitch Events
	char			Filler4[15];
	WORD			PitchCount;
	TPitchEvents   *Pitch;		// 가변
} TVoice;


//------------------------------------------------------------------------------
typedef struct {
	char				InstName[8 + 1];
    MYADLIB_BNK_INST_RECORD32	Instrument;
} TInstData;

//------------------------------------------------------------------------------
typedef struct {
	BYTE			FileName[1024];		// 경로를 포함한 파일명
	int				FileSize;			// Raw 버퍼로 읽어들인 실제 크기
	BYTE		   *Raw;				// ROL 파일을 통째로 읽어올 버퍼
	BYTE		   *RawPtr;

	TROLHeader		Header;				// ① 헤더 (0 ~ 200 : 201 byte)
	WORD			TempoCount;			// ② 템포 이벤트 수 (201 ~ 202 : 2 byte)
	TTempoEvents   *Tempo;	   			// ③ 템포 이벤트 데이터 (203 ~ 가변)
	TVoice			Voice[MAXVOICE];	// ④ 보이스 데이터 (시작 위치는 ③에 크기에 따라 결정됨)

	int				VoiceCount;			// 채널 수(9 or 11)
    int				TimeOfLastNote;
    int				UsedInstCount;		// 사용된 악기 수
    TInstData  	   *UsedInstData;		// 사용된 악기 정보 (악기 이름 + 악기 데이터)

	MYADLIB_BNK	   *BNKFile;
} TROLFile;
#pragma pack(pop)


TROLFile   *LoadROLFile(char *ROLFileName);
void		FreeROLFile(TROLFile *R);

#ifdef __cplusplus
}
#endif

class Rol
{
public:
	TROLFile *m_pRol;
	
	std::string m_fileName; 	// 경로를 포함한 파일명
	int	m_fileSize;			// raw 버퍼로 읽어들인 실제 크기

	int m_basicTempo;
	int	m_songDataIndex;
	int	m_songDataTotalIndex;
	int m_tick;


	int TempoIndex;
	int NoteIndex[MAXVOICE]	;
	int InstrIndex[MAXVOICE]  ;
	int VolumeIndex[MAXVOICE] ;
	int PitchIndex[MAXVOICE] ;

	int	ROLTempo ;		// 현재 빠르기 (기본값 120)
	int CurrentTick;

public:
	Rol();
	~Rol();

	TROLFile* GetRol(){return m_pRol;}
	char* GetTitle();
	char *GetTitleJohab();
	int GetBasicTempo();
	int GetCurrentTick();
	bool Open(std::string file);
	void Rewind(void);
	int PlayEvent(void);
	bool MatchBnk(MYADLIB_BNK *bnk);
	bool Reset();

	void LoadUsedInstData(TROLFile *R, MYADLIB_BNK *B);
	void OnTempoEvent(void);
	void OnInstrEvent(int voice);
	void OnPitchEvent(int voice);
	void OnVolumeEvent(int voice);
	void OnNoteEvent(int voice);
};

#endif	//__ROL_H_
