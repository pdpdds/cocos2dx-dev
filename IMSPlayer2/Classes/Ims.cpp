#include "adlib.h"
#include "outchip.h"
#include "Ims.h"
#include "pcm.h"
#include "fmopl.h"
#include "IMSManager.h"

Ims::Ims () 
{
	m_ims = 0;
	Init();
}

void Ims::Init()
{
	m_basicTempo = 120;		// 현재 빠르기
	m_songDataTotalIndex = 0;
	m_songDataIndex = 0;
	m_tick = 0;
}

Ims::~Ims ()
{
	free(m_ims->songData);
	free(m_ims->raw);
	free(m_ims->instIndex);
	free(m_ims);
}

int Ims::GetCurrentTick()
{
	return m_tick;
}

int Ims::GetBasicTempo()
{
	return m_basicTempo;
}

char* Ims::GetTitle()
{
	char title[30];
	//han_conv(0, m_ims->header.szTuneName, title);

	//wchar_t wszBuffer[30];
	// cp949 -> unicode
	//MultiByteToWideChar(CP_ACP, 0, title, -1, wszBuffer, 30);
	// unicode -> utf-8
	//WideCharToMultiByte(CP_UTF8, 0, wszBuffer, -1, title, 30, NULL, NULL);

	//return wxString(wszBuffer, wxConvUTF8);
	return "dfdsfd";
}

char *Ims::GetTitleJohab()
{
	return m_ims->header.szTuneName;
}

long Ims::GetTotalTick()
{
	return m_ims->header.nTotalTick;
}

long Ims::GetDuration()
{
	int basicTempo = GetBasicTempo();
	int totalTicks = m_ims->header.nTotalTick;
	int tickBeat = m_ims->header.nTickBeat;
	int ticksPerMin = basicTempo * tickBeat;
	int ticksPerSec = ticksPerMin / 60;
	int totalSec = totalTicks/ticksPerSec;
	return totalSec;
}

bool Ims::Open(std::string file)
{
	int x;
	FILE *fp;

	if(m_ims)
	{
		delete m_ims->raw;
		delete m_ims->songData;
		delete m_ims->instIndex;
		delete m_ims;
	}

	m_ims = (MYADLIB_IMS *)malloc(sizeof(MYADLIB_IMS));
	if (m_ims == NULL) return NULL;

	memset(m_ims, 0, sizeof(MYADLIB_IMS));
	
	m_fileName = file;
	
	//char path[1024];
	//strcpy( path, (const char*)file.mb_str(wxConvUTF8) ); 

	char* file_contents = NULL;

	if (IMSManager::read_text(m_fileName.c_str(), &file_contents, m_fileSize) != true)
	{
		return false;
	}

	if (m_fileSize > 1024 * 1024)
	{
		return false;
	}

	m_ims->raw = (unsigned char *)file_contents;
	if (m_ims->raw == NULL) {
		return NULL;
	}

	m_ims->rawPtr = m_ims->raw;
	
	// read header
	memcpy(&m_ims->header, m_ims->rawPtr, sizeof(IMS_HEADER));
    m_ims->rawPtr += sizeof(IMS_HEADER);
    m_ims->rawPtr++;

	// read song data
	m_ims->songData = (unsigned char *)malloc(m_ims->header.cbDataSize);
	if (m_ims->songData == NULL) return NULL;

	memcpy(m_ims->songData, m_ims->rawPtr, m_ims->header.cbDataSize);
    m_ims->rawPtr += m_ims->header.cbDataSize;

	m_songDataTotalIndex = m_ims->header.cbDataSize;

	// read instrument count
	m_ims->rawPtr++;
	unsigned short int instCount;
    memcpy(&instCount, m_ims->rawPtr, sizeof(unsigned short int));
    m_ims->instCount = instCount;
    m_ims->rawPtr += sizeof(unsigned short int);
	
	// read instrument names
	m_ims->instIndex = (IMS_INST *)malloc(
			sizeof(IMS_INST) * m_ims->instCount);
	if (m_ims->instIndex == NULL) return NULL;

    for (x = 0; x < m_ims->instCount; x++) 
	{
	    memcpy(m_ims->instIndex[x].name, m_ims->rawPtr, 9);
        m_ims->rawPtr += 9;
    }

	return true;
}

void Ims::Rewind(void)
{
	m_basicTempo = m_ims->header.nBasicTempo;
	m_songDataIndex = 0;
	m_tick = 0;
}

int Ims::PlayEvent(void)
{
	static unsigned char stCode;
	unsigned char curCode;
	int delay, delayTime = 0;
	int voice, note, volume, index;
	int *paramArray;
	unsigned short int pitchBend;
    int instIndexInBnk;

again:
	curCode = m_ims->songData[m_songDataIndex];

	//fflush(stdout);
	//return (nElapsed * MAX_SLIDER_PROGRESS / m_cbImsDataSize);

	// 0x7F == 127
	if (curCode > 0x7F) 
	{	
		m_songDataIndex++;
		stCode = curCode;
	} 
	else 
		curCode = stCode;

	voice = curCode & 0x0F;	// 오른쪽 4비트를 얻는다 (채널 번호)

	// 왼쪽 4비트에 이벤트 종류가 저장되어 있다
	switch (curCode & 0xF0) 
	{	
		case 0x80: // note off
			note = m_ims->songData[m_songDataIndex];
			volume = m_ims->songData[m_songDataIndex + 1];

			NoteOff(voice);						
			SetVoiceVolume(voice, volume);
			NoteOn(voice, note);
			m_songDataIndex += 2;
			break;

		case 0x90: // note on
			note = m_ims->songData[m_songDataIndex];
			volume = m_ims->songData[m_songDataIndex + 1];

			NoteOff(voice);
			if (volume) {
				SetVoiceVolume(voice, volume);
				NoteOn(voice, note);
			}
			m_songDataIndex += 2;
			break;

		case 0xA0: // Set volume
			volume = m_ims->songData[m_songDataIndex];
			SetVoiceVolume(voice, volume);
			m_songDataIndex++;
			break;

		case 0xC0: // Set Instrument
			index = m_ims->songData[m_songDataIndex];
			instIndexInBnk = m_ims->instIndex[index].index;

			// 악기가 있다면..
			if ( instIndexInBnk >= 0 && 
					instIndexInBnk < m_ims->m_bnk->header.totalEntry)
			{
				paramArray = 
					&m_ims->m_bnk->instRecord32[instIndexInBnk].op1.keyScaleLevel;
				SetVoiceTimbre(voice, paramArray);
			}
			else
			{
				/*
				printf("%d/%d\n", instIndexInBnk, 
						m_ims->m_bnk->header.totalEntry);
				fflush(stdout);
				*/
			}

			m_songDataIndex++;
			break;

		case 0xE0: // Set Pitch
			memcpy(&pitchBend, &m_ims->songData[m_songDataIndex], 
					sizeof(unsigned short int));
			pitchBend = pitchBend / 2;

			SetVoicePitch(voice, pitchBend);
			m_songDataIndex += sizeof(unsigned short int);
			break;

		case 0xF0: // Set Tempo
			m_songDataIndex += 2;
			m_basicTempo = (m_ims->header.nBasicTempo * 
					m_ims->songData[m_songDataIndex]);
			m_basicTempo += m_ims->header.nBasicTempo * 
				m_ims->songData[m_songDataIndex + 1] / 128;
			m_songDataIndex += 3;
			break;
	}

	while (1) 
	{
		delay = m_ims->songData[m_songDataIndex];
		m_songDataIndex++;

		// IMS finish code
		if (m_ims->songData[m_songDataIndex] == 0xFC) 
		{
			if ( m_repeatMode == REPEAT_THIS )
			{
				Rewind();
			}
			else
			{
				m_playMode=SONG_END;
				break;
			}

            delayTime += delay;
        	return delayTime;
        }

		if (delay == 0xF8) 
			delayTime += 240;
		else 
			break;
	}

	delayTime += delay;

	if (delayTime == 0) 
		goto again;
	else 
		m_tick += delayTime;

	return delayTime;
}

bool Ims::MatchBnk(MYADLIB_BNK *bnk)
{
    if (m_ims == NULL) return false;

	std::string notFoundInstList = "";

	for (int i = 0; i < m_ims->instCount; i++) 
	{
		bool exist = false;
		for (int j = 0; j < bnk->header.totalEntry; j++) 
		{
		//strcmpi
#ifdef WIN32
			if (_strcmpi(bnk->nameRecord[j].name, m_ims->instIndex[i].name) == 0)
#else
			if (strcasecmp(bnk->nameRecord[j].name, m_ims->instIndex[i].name) == 0)
#endif			
			{
				m_ims->instIndex[i].index = bnk->nameRecord[j].index;
				exist=true;
				break;
			}
		}

		if ( exist == false )
		{
			const char* instname = m_ims->instIndex[i].name;
			std::string string = instname;

			notFoundInstList = notFoundInstList + string;
			notFoundInstList = notFoundInstList + "\n";
		}
	}
	
    m_ims->m_bnk = bnk;

	if ( notFoundInstList.size() > 0 )
	{
		//wxMessageBox(wxT("Not found instrument(s) in bank.\n\n") + 
				//notFoundInstList, wxT("IMS Player"), wxICON_ERROR);

		return false;
	}

    return true;
}

bool Ims::Reset()
{
    if (m_ims == NULL) return false;

	Init();

	YM3812ResetChip(ym3812p);

	SndOutput(1, 0x20);	// Enable waveform select (bit 5)
	SetMode(m_ims->header.nSoundMode);

	for (int i=0; i<MAX_VOICE; i++) 
	{
		NoteOff(i);
		SetVoiceVolume(i, 0);
	}

    Rewind();

    return true;
}
