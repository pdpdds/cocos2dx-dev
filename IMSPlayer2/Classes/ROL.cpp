#include "ADLIB.H"
#include "rol.h"
#include "outchip.h"
#include "fmopl.h"

//------------------------------------------------------------------------------
// 개방한 파일의 크기가 ROLFILE_MAXSIZE (1MB)를 초과할 경우
// Adlib Visual Composor로 작성한 정상적인 ROL 파일이
// 아닐 가능성이 높다고 간주하여 로딩 FAIL 처리를 함.
//------------------------------------------------------------------------------
#define ROLFILE_MAXSIZE	(1024 * 1024)	// == 1MB


//------------------------------------------------------------------------------
//	정적 함수 선언
//------------------------------------------------------------------------------
BOOL LoadFromROLFile(TROLFile *R);
void FreeFromROLFile(TROLFile *R);
BOOL LoadHeader(TROLFile *R);
void LoadTempoEventsCount(TROLFile *R);
BOOL LoadTempoEvents(TROLFile *R);
void FreeTempoEvents(TROLFile *R);
void LoadTimeOfLastNote(TROLFile *R, int voice);
void LoadNoteDurationCount(TROLFile *R, int voice);
BOOL LoadNoteDuration(TROLFile *R, int voice);
void FreeNoteDuration(TROLFile *R, int voice);
void LoadInstrEventsCount(TROLFile *R, int voice);
BOOL LoadInstrEvents(TROLFile *R, int voice);
void FreeInstrEvents(TROLFile *R, int voice);
void LoadVolumeEventsCount(TROLFile *R, int voice);
BOOL LoadVolumeEvents(TROLFile *R, int voice);
void FreeVolumeEvents(TROLFile *R, int voice);
void LoadPitchEventsCount(TROLFile *R, int voice);
BOOL LoadPitchEvents(TROLFile *R, int voice);
void FreePitchEvents(TROLFile *R, int voice);

BOOL LoadUsedInstCount(TROLFile *R);
void FreeUsedInstCount(TROLFile *R);
BOOL LoadUsedInstName(TROLFile *R);
void FreeUsedInstName(TROLFile *R);


//------------------------------------------------------------------------------
//	주요 함수 정의
//------------------------------------------------------------------------------
TROLFile *LoadROLFile(char *ROLFileName)
{
	int i;
	TROLFile *R;

	R = (TROLFile *)malloc(sizeof(TROLFile));
	if (R == NULL) return NULL;

	memset(R, 0, sizeof(TROLFile));

	strcpy((char*)R->FileName, ROLFileName);

	if (LoadFromROLFile(R) == FALSE) goto FAIL;
 	if (LoadHeader(R) == FALSE) goto FAIL;
	LoadTempoEventsCount(R);
	if (LoadTempoEvents(R) == FALSE) goto FAIL;

    R->VoiceCount = R->Header.SoundMode ? 11 : 9;
    R->TimeOfLastNote = 0;

	for (i = 0; i < R->VoiceCount; i++) {
		LoadTimeOfLastNote(R, i);
        LoadNoteDurationCount(R, i);
		if (LoadNoteDuration(R, i) == FALSE) goto FAIL;
		LoadInstrEventsCount(R, i);
		if (LoadInstrEvents(R, i) == FALSE) goto FAIL;
		LoadVolumeEventsCount(R, i);
		if (LoadVolumeEvents(R, i) == FALSE) goto FAIL;
		LoadPitchEventsCount(R, i);
		if (LoadPitchEvents(R, i) == FALSE) goto FAIL;
	}

   	if (!LoadUsedInstCount(R)) goto FAIL;
   	if (!LoadUsedInstName(R)) goto FAIL;

SUCCESS:
	// ROL 파일의 이상 유무를 확인하는 마지막 절차
	//if (CheckROLFileSize(R) == FALSE) goto FAIL;
 	return R;

FAIL:
	FreeROLFile(R);	// 할당한 메모리 롤백
	return NULL;
}
//------------------------------------------------------------------------------
void FreeROLFile(TROLFile *R)
{
	int i;

    if (R == NULL) return;

	FreeUsedInstName(R);
   	FreeUsedInstCount(R);

	for (i = R->VoiceCount - 1; i >= 0; i--) {
		FreePitchEvents(R, i);
		FreeVolumeEvents(R, i);
		FreeInstrEvents(R, i);
		FreeNoteDuration(R, i);
	}

	FreeTempoEvents(R);
	FreeFromROLFile(R);

	memset(R, 0, sizeof(TROLFile));
	free(R);
}
//------------------------------------------------------------------------------
//	정적 함수 정의
//------------------------------------------------------------------------------
BOOL LoadFromROLFile(TROLFile *R)
{
	FILE *fp;
	
	fp = fopen((char*)R->FileName, "rb");
	if (fp == 0) return FALSE;

	fseek(fp, 0, SEEK_END);
	R->FileSize = ftell(fp);
    // ROLFILE_MAXSIZE == 1 MB
	if (R->FileSize > ROLFILE_MAXSIZE) {fclose(fp); return FALSE;}

	R->Raw = (BYTE *)malloc(R->FileSize);
	if (R->Raw == NULL) {fclose(fp); return FALSE;}

	fseek(fp, 0, SEEK_SET);
	fread((BYTE *)(R->Raw), R->FileSize, 1, fp);
	fclose(fp);

	R->RawPtr = R->Raw;	// ▶▶

	return TRUE;
}
//------------------------------------------------------------------------------
void FreeFromROLFile(TROLFile *R)
{
	if (R->Raw) free(R->Raw);
}
//------------------------------------------------------------------------------
BOOL LoadHeader(TROLFile *R)
{
	memcpy(&R->Header, R->RawPtr, sizeof(TROLHeader));
	R->RawPtr += sizeof(TROLHeader);	// ▶▶

	if (strcmp(R->Header.Filler1, "\\roll\\default") != 0) return FALSE;

	return TRUE;
}
//------------------------------------------------------------------------------
void LoadTempoEventsCount(TROLFile *R)
{
	memcpy(&R->TempoCount, R->RawPtr, sizeof(WORD));
	R->RawPtr += sizeof(WORD);	// ▶▶
}
//------------------------------------------------------------------------------
BOOL LoadTempoEvents(TROLFile *R)
{
	int Size;

	if (R->TempoCount == 0) return TRUE;

	Size = sizeof(TTempoEvents) * R->TempoCount;
	R->Tempo = (TTempoEvents *)malloc(Size);
	if (R->Tempo == NULL) return FALSE;

	memcpy(R->Tempo, R->RawPtr, Size);
	R->RawPtr += Size;	// ▶▶

	return TRUE;
}
//------------------------------------------------------------------------------
void FreeTempoEvents(TROLFile *R)
{
	if (R->Tempo) free(R->Tempo);
}
//------------------------------------------------------------------------------
void LoadTimeOfLastNote(TROLFile *R, int voice)
{
	memcpy(&R->Voice[voice].Filler1[0], R->RawPtr, 15);
	R->RawPtr += 15;			// ▶▶
	memcpy(&R->Voice[voice].TimeOfLastNote, R->RawPtr, sizeof(WORD));
	R->RawPtr += sizeof(WORD);	// ▶▶
}
//------------------------------------------------------------------------------
//	1. NoteDuration in Voice
//------------------------------------------------------------------------------
void LoadNoteDurationCount(TROLFile *R, int voice)
{
    WORD NoteNumber, NoteDuration;
    int NoteDurationSum = 0;
	BYTE *p = R->RawPtr;

    R->Voice[voice].NoteCount = 0;

	while (R->Voice[voice].TimeOfLastNote > NoteDurationSum) {
    	memcpy(&NoteNumber, p, sizeof(WORD));
	    p += sizeof(WORD);
        memcpy(&NoteDuration, p, sizeof(WORD));
	    p += sizeof(WORD);
        NoteDurationSum += NoteDuration;
        R->Voice[voice].NoteCount++;
    }
}
//------------------------------------------------------------------------------
BOOL LoadNoteDuration(TROLFile *R, int voice)
{
    int i, NoteDurationSize;
    WORD NoteNumber, NoteDuration;
    int NoteDurationSum = 0;

	if (R->Voice[voice].NoteCount == 0) return TRUE;

	NoteDurationSize = sizeof(TNoteDuration) * R->Voice[voice].NoteCount;
	R->Voice[voice].Note = (TNoteDuration *)malloc(NoteDurationSize);
    if (R->Voice[voice].Note == NULL) return FALSE;

    if (R->Voice[voice].TimeOfLastNote > R->TimeOfLastNote)
    	R->TimeOfLastNote = R->Voice[voice].TimeOfLastNote;

    NoteDurationSum = 0;

    for (i = 0; i < R->Voice[voice].NoteCount; i++) {
    	memcpy(&NoteNumber, R->RawPtr, sizeof(WORD));
	    R->RawPtr += sizeof(WORD);
        R->Voice[voice].Note[i].NoteNumber = NoteNumber;
	    memcpy(&NoteDuration, R->RawPtr, sizeof(WORD));
    	R->RawPtr += sizeof(WORD);
        R->Voice[voice].Note[i].NoteDuration = NoteDurationSum;
        NoteDurationSum += NoteDuration;
    }

	return TRUE;
}
//------------------------------------------------------------------------------
void FreeNoteDuration(TROLFile *R, int voice)
{
	if (R->Voice[voice].Note) free(R->Voice[voice].Note);
}
//------------------------------------------------------------------------------
//	2. InstrEvents in Voice
//------------------------------------------------------------------------------
void LoadInstrEventsCount(TROLFile *R, int voice)
{
	memcpy(&R->Voice[voice].Filler2[0], R->RawPtr, 15);
    R->RawPtr += 15;			// ▶▶
	memcpy(&R->Voice[voice].InstrCount, R->RawPtr, sizeof(WORD));
	R->RawPtr += sizeof(WORD);	// ▶▶
}
//------------------------------------------------------------------------------
BOOL LoadInstrEvents(TROLFile *R, int voice)
{
	int Size = sizeof(TInstrEvents) * R->Voice[voice].InstrCount;

    if (R->Voice[voice].InstrCount == 0) return TRUE;

	R->Voice[voice].Instr = (TInstrEvents *)malloc(Size);
	if (R->Voice[voice].Instr == NULL) return FALSE;

	memcpy(R->Voice[voice].Instr, R->RawPtr, Size);
	R->RawPtr += Size;	// ▶▶

	return TRUE;
}
//------------------------------------------------------------------------------
void FreeInstrEvents(TROLFile *R, int voice)
{
	if (R->Voice[voice].Instr) free(R->Voice[voice].Instr);
}
//------------------------------------------------------------------------------
//	3. VolumeEvents in Voice
//------------------------------------------------------------------------------
void LoadVolumeEventsCount(TROLFile *R, int voice)
{
	memcpy(&R->Voice[voice].Filler3[0], R->RawPtr, 15);
	R->RawPtr += 15;			// ▶▶
	memcpy(&R->Voice[voice].VolumeCount, R->RawPtr, sizeof(WORD));
	R->RawPtr += sizeof(WORD);	// ▶▶
}
//------------------------------------------------------------------------------
BOOL LoadVolumeEvents(TROLFile *R, int voice)
{
	int Size;

    if (R->Voice[voice].VolumeCount == 0) return TRUE;

	Size = sizeof(TVolumeEvents) * R->Voice[voice].VolumeCount;

	R->Voice[voice].Volume = (TVolumeEvents *)malloc(Size);
	if (R->Voice[voice].Volume == NULL) return FALSE;

	memcpy(R->Voice[voice].Volume, R->RawPtr, Size);
	R->RawPtr += Size;	// ▶▶

	return TRUE;
}
//------------------------------------------------------------------------------
void FreeVolumeEvents(TROLFile *R, int voice)
{
	if (R->Voice[voice].Volume) free(R->Voice[voice].Volume);
}
//------------------------------------------------------------------------------
//	4. PitchEvents in Voice
//------------------------------------------------------------------------------
void LoadPitchEventsCount(TROLFile *R, int voice)
{
	memcpy(&R->Voice[voice].Filler4[0], R->RawPtr, 15);
	R->RawPtr += 15;			// ▶▶
	memcpy(&R->Voice[voice].PitchCount, R->RawPtr, sizeof(WORD));
	R->RawPtr += sizeof(WORD);	// ▶▶
}
//------------------------------------------------------------------------------
BOOL LoadPitchEvents(TROLFile *R, int voice)
{
	int Size;

    if (R->Voice[voice].PitchCount == 0) return TRUE;

    Size = sizeof(TPitchEvents) * R->Voice[voice].PitchCount;
	R->Voice[voice].Pitch = (TPitchEvents *)malloc(Size);
	if (R->Voice[voice].Pitch == NULL) return FALSE;

	memcpy(R->Voice[voice].Pitch, R->RawPtr, Size);
	R->RawPtr += Size;	// ▶▶

	return TRUE;
}
//------------------------------------------------------------------------------
void FreePitchEvents(TROLFile *R, int voice)
{
	if (R->Voice[voice].Pitch) free(R->Voice[voice].Pitch);
}
//------------------------------------------------------------------------------
BOOL LoadUsedInstCount(TROLFile *R)
{
	int i, j, k, l, UsedInstCount = 0;

    for (i = 0; i < R->VoiceCount; i++) {
    	int InstListSize = sizeof(int) * R->Voice[i].InstrCount;
	    R->Voice[i].InstrIndex = (int *)malloc(InstListSize);
        if (R->Voice[i].InstrIndex == NULL) return FALSE;

		memset(R->Voice[i].InstrIndex, -1, InstListSize);
    }

    for (i = 0; i < R->VoiceCount; i++) {
        for (j = 0; j < R->Voice[i].InstrCount; j++) {
            if (R->Voice[i].InstrIndex[j] == -1) {
	            char *tempptr1;

                R->Voice[i].InstrIndex[j] = UsedInstCount++;
                tempptr1 = R->Voice[i].Instr[j].InstrName;
                for (k = i; k < R->VoiceCount; k++) {
                    for (l = 0; l < R->Voice[k].InstrCount; l++) {
		                char *tempptr2 = R->Voice[k].Instr[l].InstrName;
#ifdef WIN32
						if (_stricmp(tempptr1, tempptr2) == 0)
#else
                        if (strcasecmp(tempptr1, tempptr2) == 0)
#endif
                        	R->Voice[k].InstrIndex[l] = R->Voice[i].InstrIndex[j];
                    }
                }
            }
        }
    }

    R->UsedInstCount = UsedInstCount;

    return TRUE;
}
//------------------------------------------------------------------------------
void FreeUsedInstCount(TROLFile *R)
{
	int i;

    for (i = 0; i < R->VoiceCount; i++)
	    if (R->Voice[i].InstrIndex) free(R->Voice[i].InstrIndex);
}
//------------------------------------------------------------------------------
BOOL LoadUsedInstName(TROLFile *R)
{
	int i, j, k;

    if (R->UsedInstCount == 0) return FALSE;

	R->UsedInstData = (TInstData *)malloc(sizeof(TInstData) * R->UsedInstCount);
    if (R->UsedInstData == NULL) return FALSE;

    for (i = 0; i < R->UsedInstCount; i++) {
        for (j = 0; j < R->VoiceCount; j++) {
            for (k = 0; k < R->Voice[j].InstrCount; k++) {
                if (R->Voice[j].InstrIndex[k] == i) {
                    char *tempptr1 = R->Voice[j].Instr[k].InstrName;
                    strcpy(R->UsedInstData[i].InstName, tempptr1);
                }
            }
        }
    }

    return TRUE;
}
//------------------------------------------------------------------------------
void FreeUsedInstName(TROLFile *R)
{
	if (R->UsedInstData) free(R->UsedInstData);
}
//------------------------------------------------------------------------------

Rol::Rol () 
{
	m_basicTempo = 120;		// 현재 빠르기
	m_songDataTotalIndex = 0;
	m_songDataIndex = 0;
	m_tick = 0;


	TempoIndex = 0;
	memset(NoteIndex, 0 , sizeof(int) * MAXVOICE);
	memset(InstrIndex, 0 , sizeof(int) * MAXVOICE);
	memset(VolumeIndex, 0 , sizeof(int) * MAXVOICE);
	memset(PitchIndex, 0 , sizeof(int) * MAXVOICE);

	ROLTempo  = 0;		// 현재 빠르기 (기본값 120)
	CurrentTick = 0;
}

Rol::~Rol ()
{
	
}

int Rol::GetCurrentTick()
{
	return m_tick;
}

int Rol::GetBasicTempo()
{
	return m_basicTempo;
}

char* Rol::GetTitle()
{
	char title[30];
	//han_conv(0, m_ims->header.szTuneName, title);

	//wchar_t wszBuffer[30];
	// cp949 -> unicode
	//MultiByteToWideChar(CP_ACP, 0, title, -1, wszBuffer, 30);
	// unicode -> utf-8
	//WideCharToMultiByte(CP_UTF8, 0, wszBuffer, -1, title, 30, NULL, NULL);

	//return wxString(wszBuffer, wxConvUTF8);
	return (char*)"dfdsfd";
}

bool Rol::Open(std::string file)
{
	int x;
	FILE *fp;

	//m_pRol = (TROLFile *)malloc(sizeof(TROLFile));
	//if (m_pRol == NULL) return NULL;

	//memset(m_pRol, 0, sizeof(TROLFile));
	
	m_fileName = file;
	
	if((m_pRol = LoadROLFile((char*)m_fileName.c_str())) == 0)
	{
		return false;
	}

	return true;
}

void Rol::Rewind(void)
{
	m_basicTempo = m_pRol->Header.BasicTempo;
	m_songDataIndex = 0;
	m_tick = 0;

	TempoIndex = 0;
	memset(NoteIndex,	0, sizeof(int) * MAXVOICE);
	memset(InstrIndex,	0, sizeof(int) * MAXVOICE);
	memset(VolumeIndex, 0, sizeof(int) * MAXVOICE);
	memset(PitchIndex,	0, sizeof(int) * MAXVOICE);
	CurrentTick = 0;
	ROLTempo = m_pRol->Header.BasicTempo;

}

int Rol::PlayEvent(void)
{
	int i, TickDelay;

    if (TempoIndex < m_pRol->TempoCount) OnTempoEvent();
	for (i = 0; i < m_pRol->VoiceCount; i++) {
		if (InstrIndex[i] < m_pRol->Voice[i].InstrCount) OnInstrEvent(i);
		if (PitchIndex[i] < m_pRol->Voice[i].PitchCount) OnPitchEvent(i);
		if (VolumeIndex[i] < m_pRol->Voice[i].VolumeCount) OnVolumeEvent(i);
        if (CurrentTick < m_pRol->Voice[i].TimeOfLastNote) OnNoteEvent(i);
    	else 	
		{
				NoteOff(i);
		}
    }

    TickDelay = 240 / m_pRol->Header.TicksPerBeat;
    CurrentTick++;

    if (CurrentTick >= m_pRol->TimeOfLastNote) Rewind();
	return TickDelay;
}

bool Rol::MatchBnk(MYADLIB_BNK *bnk)
{
    if (m_pRol == NULL) return false;

	LoadUsedInstData(m_pRol, bnk);
	m_pRol->BNKFile = bnk;

    return true;
}

bool Rol::Reset()
{
    if (m_pRol == NULL) return false;

	YM3812ResetChip(ym3812p);

	SndOutput(1, 0x20);		// Enable waveform select (bit 5)
    SetMode(!m_pRol->Header.SoundMode);

    for (int i = 0; i < m_pRol->VoiceCount; i++) {
    	SetVoiceVolume(i, 0);
        NoteOff(i);
    }

    Rewind();

    return true;
}

void Rol::LoadUsedInstData(TROLFile *R, MYADLIB_BNK *B)
{
	int i, diff;
    BYTE *p, *raw;

    raw = (BYTE*)B->raw;

    for (i = 0; i < R->UsedInstCount; i++) {
        char *tempptr1 = R->UsedInstData[i].InstName;
		int *tempptr2 = &R->UsedInstData[i].Instrument.instType;
        int searchTop = 0;
		int searchBottom = B->header.usedEntry - 1;
        int searchMiddle = searchBottom >> 1;
        do {
			char InstName[8 + 1];
			p = raw + B->header.recSeekPos + (searchMiddle * 12) + 3;
            strcpy(InstName, (char*)p);
            p += 9;
#ifdef WIN32
			diff = _stricmp(tempptr1, InstName);
#else
            diff = strcasecmp(tempptr1, InstName);
#endif
            if (diff != 0) {
                if (diff < 0) searchBottom = searchMiddle - 1;
                else searchTop = searchMiddle + 1;
                searchMiddle = (searchTop + searchBottom) >> 1;
            }
        } while (diff && searchTop <= searchBottom);

        if (diff == 0) {
        	short tempOffset1;
		    int j, tempOffset2;
            p = raw + B->header.recSeekPos + (searchMiddle * 12);
            memcpy(&tempOffset1, p, sizeof(short));
            p += sizeof(short);

            tempOffset2 = tempOffset1;
			p = raw + B->header.dataSeekPos + (tempOffset2 * 30);
            for (j = 0; j < 30; j++) *(tempptr2++) = *(p++);
        }
    }
}


//------------------------------	------------------------------------------------
void Rol::OnTempoEvent(void)
{
    int TimeOfEvents = m_pRol->Tempo[TempoIndex].TimeOfEvents;
    float TempoMultiplier = m_pRol->Tempo[TempoIndex].TempoMultiplier;

	if (TimeOfEvents == CurrentTick) {
		ROLTempo = m_pRol->Header.BasicTempo * TempoMultiplier;
        TempoIndex++;
    }
}
//------------------------------------------------------------------------------
void Rol::OnInstrEvent(int voice)
{
    int Index = InstrIndex[voice];
	int TimeOfEvents = m_pRol->Voice[voice].Instr[Index].TimeOfEvents;

    if (TimeOfEvents == CurrentTick) {
        int InstDataIndex = m_pRol->Voice[voice].InstrIndex[Index];
		int *paramArray = &m_pRol->UsedInstData[InstDataIndex].Instrument.op1.keyScaleLevel;

        SetVoiceTimbre(voice, paramArray);
        InstrIndex[voice]++;
    }
}
//------------------------------------------------------------------------------
void Rol::OnPitchEvent(int voice)
{
	int Index = PitchIndex[voice];
	int TimeOfEvents = m_pRol->Voice[voice].Pitch[Index].TimeOfEvents;

    if (TimeOfEvents == CurrentTick) {
	    float PitchVariation = m_pRol->Voice[voice].Pitch[Index].PitchVariation;
	    SetVoicePitch(voice, PitchVariation * 0x2000);
		PitchIndex[voice]++;
    }
}
//------------------------------------------------------------------------------
void Rol::OnVolumeEvent(int voice)
{
	int Index = VolumeIndex[voice];
	int TimeOfEvents = m_pRol->Voice[voice].Volume[Index].TimeOfEvents;

    if (TimeOfEvents == CurrentTick) {
	    float VolumeMultiplier = m_pRol->Voice[voice].Volume[Index].VolumeMultiplier;
	    SetVoiceVolume(voice, VolumeMultiplier * 127);	// 0x7F == 127
		VolumeIndex[voice]++;
    }
}
//------------------------------------------------------------------------------
void Rol::OnNoteEvent(int voice)
{
    int Index = NoteIndex[voice];
    int NoteDuration = m_pRol->Voice[voice].Note[Index].NoteDuration;

    if (NoteDuration == CurrentTick) {
	    int NoteNumber;

    	NoteOff(voice);
        NoteNumber = m_pRol->Voice[voice].Note[Index].NoteNumber;
        if (NoteNumber) NoteOn(voice, NoteNumber);
        NoteIndex[voice]++;
    }
}