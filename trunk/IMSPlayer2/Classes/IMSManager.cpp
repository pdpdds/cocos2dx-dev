#include "IMSManager.h"
#include "SDL.h"
#include "Bnk.h"
#include "Ims.h"
#include "Iss.h"
#include "Hangul.h"
#include "pcm.h"
#include "adlib.h"
#include "fmopl.h"
#include "outchip.h"
#include <iostream>
#include <iostream>
#include "rol.h"
#include <string>
#include "StringUtil.h"

using namespace std;

//#pragma comment(lib, "SDL2.lib")

void CallbackIms(void *userdata, Uint8 *audio, int len);
void CallbackRol(void *userdata, Uint8 *audio, int len);

IMSManager::IMSManager(void)
{
}

IMSManager::~IMSManager(void)
{
	m_pIms->Reset();

	delete m_pIms;
	delete m_pBnk;

	if (m_pIss)
		delete m_pIss;
	
	delete m_pRol;

	delete m_pHangul;
	
	YM3812Shutdown(ym3812p);
	
	free_prepare_pcm_buffer (0);
	free_prepare_pcm_buffer (1);
}

bool IMSManager::Initialize()
{
	m_pIMSEventCallback = NULL;
	if(SDL_Init(SDL_INIT_AUDIO) < 0) 
	{
		return false;
	}

	ym3812p = YM3812Init(3579545L, 22050);
	SoundWarmInit();

	m_pIms = new Ims();
	m_pBnk = new Bnk();
	m_pIss = new Iss();
	m_pRol = new Rol();

	m_pHangul = new Hangul();
	m_pHangul->SangYongCode();
	m_pHangul->SetHanFontRoom(0);
	m_pHangul->SetEngFontRoom(0);
	m_pHangul->SetSpecialFontRoom(0);
	m_pHangul->SetSpecialCharOn();
	m_pHangul->SetCtable(0);

	m_pSpec = new SDL_AudioSpec;

	m_pSpec->freq = 22050;	// 1초당 22050개 샘플 (보통 게임에서 많이 사용)
	m_pSpec->format = AUDIO_S16SYS; 	// 16bit
	m_pSpec->channels = 1;	// mono
	m_pSpec->samples = 512;	// buffer
	// len 4096이 넘어간다.
	// len = samples * channels * format (16bit <-- 2byte)
	//m_spec.callback = callback;		
	m_pSpec->userdata = this;
	m_pSpec->callback = CallbackIms;

	/*if(SDL_OpenAudio(m_pSpec, NULL) < 0) 
	{
		return false;
	}*/

	return true;
}

bool IMSManager::GetIMSFileInfo(std::string szName, IMS_HEADER& sIMSHeader)
{
	int x;
	FILE *fp;
	
	fp = fopen(szName.c_str(), "rb");
	if (fp == 0) 		
	{			
		return false;
	}

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);

	// 파일 사이즈가 1MB를 넘어서면 에러로 간주함
	if (fileSize > 1024*1024) {
		fclose(fp); 
		return FALSE;
	}

	
	// dump whole
	fseek(fp, 0, SEEK_SET);
	fread((unsigned char *)&sIMSHeader, 
			sizeof(IMS_HEADER), sizeof(unsigned char), fp);	
	fclose(fp);

	return true;
}

bool IMSManager::read_text(const char* source_file, char** destination, int& length)
{
	// Open the file
	SDL_RWops *file;
	file = SDL_RWFromFile(source_file, "r");

	if (file == 0)
		return false;

	size_t file_length = SDL_RWseek(file, 0, SEEK_END);
	(*destination) = new char[file_length + 1];
	// Reset seek to beginning of file and read text
	SDL_RWseek(file, 0, SEEK_SET);
	int n_blocks = SDL_RWread(file, (*destination), 1, file_length);
	if (n_blocks <= 0)
	{

	}
	SDL_RWclose(file);

	(*destination)[file_length] = '\0';
	length = file_length;
	return true;
}

bool IMSManager::LoadIMS(char* szImsFile)
{
	if(szImsFile == 0)
		return false;

	string szFileName = szImsFile;
	//string szFileName = ws2s(szImsFile);
	//wstring strFilePath, strFileName;
	//int nFind = strFullPath.rfind(L"\\") + 1;
	//strFilePath = strFullPath.substr(0, nFind);
	//strFileName = strFullPath.substr(nFind, strFullPath.length() - nFind);

	string szBnkName = ConvertExt(szFileName, "bnk");
	string szIssName = ConvertExt(szFileName, "iss");

	if(GetExt(szFileName).compare("ims") != 0)
		if(GetExt(szFileName).compare("IMS") != 0)
	{			
		return false;
	}

	m_pSpec->callback = CallbackIms; 

	if(false == m_pBnk->Open("1.bnk"))
	{
		if(false == m_pBnk->Open("standard.bnk"))
			return false;
	}

	if(false == m_pIms->Open("1.ims"))
	{
		return false;
	}

	m_pIms->MatchBnk(m_pBnk->GetBnk());
	
	//char *title = ims->GetTitleJohab();
	
	if(m_pIss)
		delete m_pIss;

	/*m_pIss = new Iss();
	
	if (m_pIss->Open("1.iss") == true )
	{
		char *writer = m_pIss->GetWriter();
		//parent->m_issViewer->OutTextXy(0, 16, writer);
		char *composer = m_pIss->GetComposer();
		//parent->m_issViewer->OutTextXy(0, 32, composer);
	}*/

	m_pIms->Reset();

	init_prepare_pcm_buffer();

	alloc_prepare_pcm_buffer(0);
	alloc_prepare_pcm_buffer(1);

	
	m_playMode = PLAYING;
	prepare_next(this, PREPARE_BUFFER_SIZE, 0, m_pSpec->channels * (m_pSpec->format == AUDIO_U8 ? 1 : 2));
		

	return true;
}

bool IMSManager::LoadRol(char* szRolFile, char* szBnkFile, char* szIssFile)
{
	if(szRolFile == 0 || szBnkFile == 0)
		return false;

	m_pSpec->callback = CallbackRol;

	std::string szBnk = szBnkFile;
	if(false == m_pBnk->Open(szBnk))
	{
		if(false == m_pBnk->Open("Standard.bnk"))
			return false;
	}

	std::string szRol = szRolFile;

	if(false == m_pRol->Open(szRol))
	{
		return false;
	}

	m_pRol->MatchBnk(m_pBnk->GetBnk());
		
	if(SDL_OpenAudio(m_pSpec, NULL) < 0) 
	{
		return false;
	}

	m_pRol->Reset();

	init_prepare_pcm_buffer();

	alloc_prepare_pcm_buffer(0);
	alloc_prepare_pcm_buffer(1);

	
	m_playMode = PLAYING;
	prepare_next2(this, PREPARE_BUFFER_SIZE, 0, m_pSpec->channels * (m_pSpec->format == AUDIO_U8 ? 1 : 2));

	return true;
}

void IMSManager::Run()
{
	SDL_PauseAudio(0);
}

void IMSManager::Stop()
{
	SDL_PauseAudio(1);
}

int IMSManager::GetPlayStatus()
{
	return m_playMode;
}

#define CP_JOHAP 1361

/*
void ConvertCodePage(CHAR *string, int maxlen)
{
	int len;
	WCHAR *temp;

	len = MultiByteToWideChar( CP_JOHAP, MB_PRECOMPOSED, string, -1, NULL, 0 );

	temp = (WCHAR *)GlobalAlloc( GMEM_FIXED, len * sizeof(WCHAR) );

	if( !temp )
		return;

	MultiByteToWideChar( CP_JOHAP, MB_PRECOMPOSED, string, -1, temp, len );

	WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, temp, len, string, maxlen, NULL, NULL );

	GlobalFree( temp );
}*/

std::string IMSManager::ConvertLyrics(char* lyrics)
{
	if ( strlen(lyrics) == 0 ) return std::string("");

	int pos=0;	/* string position */

	std::string szFullLyrics;
	
	while (1)
	{
		if ( pos >= strlen(lyrics) ) break;

		/*
		if (str[pos]=='/') 
		{
			//textattr_process(str,&pos);
		}
		else 
		*/
		/*if ( pos >= _from && pos <= _to )
		{
			_fg = _rangefg;
			_bg = _rangebg;
		} else {
			_fg = _normalfg;
			_bg = _normalbg;
		}*/

		if (lyrics[pos]=='\t') {
			pos++;
		} else if (!(lyrics[pos]&0x80)) {
			szFullLyrics += *(lyrics+pos);
			pos++;
		} else if (m_pHangul->IsSpecial(lyrics, pos)) {
			pos+=2;
		}/* else if (graphokflag && IsGraphic(str, pos)) {
			pos+=2;
		} else if (hanjaokflag && IsHanja(str, pos)) {
			pos+=2;
		}*/ else {
			//hangulout_process(rgba, str, pos);

			char hanChar[3] = {0,};
			hanChar[0] = *(lyrics+pos);
			hanChar[1] = *(lyrics+pos + 1);

		//	ConvertCodePage(hanChar, 2);
			szFullLyrics += hanChar;
			pos+=2;
		}
	};

	//printf("%s\n", szFullLyrics.c_str());
	return szFullLyrics;
}

void CallbackIms(void *userdata, Uint8 *audio, int len)
{
	IMSManager* pManager = (IMSManager*)userdata;
	// 연주할 버퍼를 얻는다.
	Uint8 *buffer = pcm_buffer[m_playingBuffer]
		[m_playingBufferIndex];
	
	// 현재 연주중인 틱
	int tick = song_tick[m_playingBuffer][m_playingBufferIndex];

	// 프로그레스바 업데이트
	//self->m_slider->SetValue((float)tick);
				
	// 가사 업데이트
	ISS_RECORD *record = lyrics_buffer[m_playingBuffer][m_playingBufferIndex];
	if ( record != NULL )
	{
		int line = record->line;

		// 속도를 위해 이미 출력했던 라인의 가사는 출력하지 않는다.
		// 문제점은 정확한 가사 추적이 안된다.
		if ( m_lastLyricsLine != line )
		{
			m_lastLyricsLine = line;

			int from = 0;
			int start_x = record->start_x;
			int to = start_x - record->width_x;

			Iss *m_iss = pManager->GetIss();
			char *lyrics = m_iss->m_script[line].script;

			if(lyrics != NULL)
			{
				//ConvertCodePage(lyrics, 64);
				
				std::string  szLyrics = pManager->ConvertLyrics(lyrics);
				if(pManager->GetIMSEventCallback())
				{ 				
					pManager->GetIMSEventCallback()->CallbackLyrics((char*)szLyrics.c_str());
				}
				//printf("%s\n", lyrics);
			}
		}
	}
			

			

	// FFT 연산후 이퀼라이저를 그린다.
	//self->FFT((void *)buffer, len);
	
	// 사운드 카드로 pcm 데이타를 보내어 소리를 낸다.
	SDL_MixAudio(audio, buffer, len, m_volume);

	// 마지막까지 연주가 되었는지 검사한다.
	if ( m_endBuffer == m_playingBuffer &&
		m_endBufferIndex == m_playingBufferIndex )
	{
		SDL_PauseAudio(1);
		m_playMode=STOP;
		pManager->GetIMSEventCallback()->CallbackPlayEnd();
		return;
	}

	SDL_AudioSpec* pSpec = pManager->GetAudioSpec();
	
	// 다음 버퍼를 미리 준비한다.
	if ( m_playingBufferIndex == 0 )
	{
		if ( m_playingBuffer == 0 )
			prepare_next(userdata, PREPARE_BUFFER_SIZE, 1, pSpec->channels * (pSpec->format == AUDIO_U8 ? 1 : 2));
		else
			prepare_next(userdata, PREPARE_BUFFER_SIZE, 0, pSpec->channels * (pSpec->format == AUDIO_U8 ? 1 : 2));
	}

	m_playingBufferIndex++;

	// 버퍼의 마지막이 연주 되었으면 다음 버퍼로 교체한다.
	if ( m_playingBufferIndex == PREPARE_BUFFER_COUNT )
	{
		m_playingBufferIndex=0;

		if (m_playingBuffer == 0 )
			m_playingBuffer = 1;
		else
			m_playingBuffer = 0;
	}
}
#define BUFFERSIZE				0xFFFF

void CallbackRol(void *userdata, Uint8 *audio, int len)
{
		IMSManager* pManager = (IMSManager*)userdata;
	// 연주할 버퍼를 얻는다.
	Uint8 *buffer = pcm_buffer[m_playingBuffer]
		[m_playingBufferIndex];
	
	// 현재 연주중인 틱
	//int tick = song_tick[m_playingBuffer][m_playingBufferIndex];

	
	
	// 사운드 카드로 pcm 데이타를 보내어 소리를 낸다.
	SDL_MixAudio(audio, buffer, len, m_volume);

	// 마지막까지 연주가 되었는지 검사한다.
	if ( m_endBuffer == m_playingBuffer &&
		m_endBufferIndex == m_playingBufferIndex )
	{
		m_playMode=STOP;
		return;
	}

	SDL_AudioSpec* pSpec = pManager->GetAudioSpec();
	
	// 다음 버퍼를 미리 준비한다.
	if ( m_playingBufferIndex == 0 )
	{
		if ( m_playingBuffer == 0 )
			prepare_next2(userdata, PREPARE_BUFFER_SIZE, 1, pSpec->channels * (pSpec->format == AUDIO_U8 ? 1 : 2));
		else
			prepare_next2(userdata, PREPARE_BUFFER_SIZE, 0, pSpec->channels * (pSpec->format == AUDIO_U8 ? 1 : 2));
	}

	m_playingBufferIndex++;

	// 버퍼의 마지막이 연주 되었으면 다음 버퍼로 교체한다.
	if ( m_playingBufferIndex == PREPARE_BUFFER_COUNT )
	{
		m_playingBufferIndex=0;

		if (m_playingBuffer == 0 )
		{
			m_playingBuffer = 1;	
			//memset(pcm_buffer[1][0], 1, PREPARE_BUFFER_SIZE);
		}
		else
		{
			m_playingBuffer = 0;
			//memset(pcm_buffer[0][0], 0, PREPARE_BUFFER_SIZE);
		}
	}
}
