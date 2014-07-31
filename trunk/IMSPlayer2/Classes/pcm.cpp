#include <stdlib.h>
#include "pcm.h"
#include "sdl.h"
#include "Ims.h"
#include "Iss.h"
#include "Rol.h"
#include "fmopl.h"
#include "outchip.h"
#include "IMSManager.h"
#ifdef WIN32
#include <Windows.h>
#endif

unsigned int song_tick[2][PREPARE_BUFFER_COUNT];
unsigned char *pcm_buffer[2][PREPARE_BUFFER_COUNT];
ISS_RECORD *lyrics_buffer[2][PREPARE_BUFFER_COUNT];

// ���� �������� ����
int m_playingBuffer=0;
// ���� �������� ������ �ε���
int m_playingBufferIndex=0;
// ������ ������ ���� ����
int m_lastLyricsLine= -1;
// ������ ������ ���� ���ڵ�
int m_lastLyricsRecord = 0;

// ���ְ� ����� ����
int m_endBuffer;
// ���ְ� ����� ������ �ε���
int m_endBufferIndex;

// �������� ����Ʈ �ε���
long m_playingItem = -1;

int m_volume = SDL_MIX_MAXVOLUME;
int m_playMode = STOPPED;
int m_repeatMode = REPEAT_ALL;

void init_prepare_pcm_buffer(void)
{
	m_playingBuffer=0;
	m_playingBufferIndex=0;

	m_lastLyricsLine=-1;
	m_lastLyricsRecord=0;

	m_endBuffer=-1;
	m_endBufferIndex=-1;
}

void alloc_prepare_pcm_buffer (int bufferIndex)
{
	for(int i=0; i<PREPARE_BUFFER_COUNT; i++)
	{
		if(pcm_buffer[bufferIndex][i])
		{
			memset(pcm_buffer[bufferIndex][i], 0, PREPARE_BUFFER_SIZE);
		}
		else
		{
			pcm_buffer[bufferIndex][i] = (unsigned char *)malloc(PREPARE_BUFFER_SIZE);
		}
		
	}
}

void free_prepare_pcm_buffer (int bufferIndex)
{
	for(int i=0; i<PREPARE_BUFFER_COUNT; i++)
	{
		if(pcm_buffer[bufferIndex][i])
		{
			free(pcm_buffer[bufferIndex][i]);
			pcm_buffer[bufferIndex][i]=NULL;
		}
	}
}

int MulDivA(int nNumber, int nNumerator,int nDenominator)
{
  return (nNumber*nNumerator+nDenominator/2)/nDenominator;
}

// ���� 10���� pcm ����Ÿ�� �غ��Ѵ�.
void prepare_next (void *userdata, int len, int bufferIndex, unsigned char sampleSize)
{
	if ( m_playMode == STOP || m_playMode == STOPPED ||
	  m_playMode == SONG_END ) return;

	IMSManager* pManager = (IMSManager*)userdata;

	Ims *m_ims = pManager->GetIms();

	for(int i=0; i<PREPARE_BUFFER_COUNT; i++)
	{
		static int remain_samples = 0;
		int to_write_len, samples;
		int delayTime = 0;
		short int *pcm_buffer_pos;
		pcm_buffer_pos = (short int *)pcm_buffer[bufferIndex][i];

		to_write_len = len / sampleSize;
		
		// ƽ�� �����Ѵ�.
		int tick = m_ims->GetCurrentTick();
		song_tick[bufferIndex][i] = tick;
				
		// ���� ���ڵ带 ã�´�.
		Iss *m_iss = pManager->GetIss();
		if ( m_iss->IsOpened() == true )
		{
			for(int x=m_lastLyricsRecord; x<m_iss->m_header.rec_count; x++)
			{
				// ���� ���Ͽ��� ������ tick�� 8�� ���� ���� �� �ִ�.
				if ( m_iss->m_record[x].kasa_tick >= tick/8 )
				{
					m_lastLyricsRecord = x;
					lyrics_buffer[bufferIndex][i] = &m_iss->m_record[x];
					break;
				}
			}
		}
		else
		{
			lyrics_buffer[bufferIndex][i] = NULL;
		}

		// �ֵ帳 PCM ����Ÿ�� ���� ���ۿ� �����Ѵ�.
		while (to_write_len > 0) 
		{
			if (remain_samples > 0) 
			{
				samples = remain_samples;
			}
			else 
			{
				// �д� ���ڼ�
				int basicTempo = m_ims->GetBasicTempo();

				delayTime = m_ims->PlayEvent();

				MYADLIB_IMS *ims = m_ims->m_ims;

				// �� ��� ������ �������� ����Ѵ�.
				samples = MulDivA(22050 * 60, 
						delayTime, basicTempo * ims->header.nTickBeat);
			}

			if (samples > to_write_len) 
			{
				// �ֵ帳 PCM ����Ÿ�� ���� ���ۿ� �����Ѵ�.
				YM3812UpdateOne(ym3812p, pcm_buffer_pos, to_write_len);
				remain_samples = samples - to_write_len;
				break;
			}
			else 
			{
				// �ֵ帳 PCM ����Ÿ�� ���� ���ۿ� �����Ѵ�.
				YM3812UpdateOne(ym3812p, pcm_buffer_pos, samples);
				remain_samples = 0;
			}

			pcm_buffer_pos = pcm_buffer_pos + samples;
			to_write_len = to_write_len - samples;
		}

		if ( m_playMode == STOP || m_playMode == STOPPED || 
				m_playMode == SONG_END)
		{
			// ����� ��ġ�� ����
			m_endBuffer = bufferIndex;
			m_endBufferIndex = i;
			return;
		}
	}
}

int RemainSamples = 0;
#define BUFFERSIZE				0xFFFF
#define BUFFERCOUNT				20	
#define WAVE_SAMPLING_RATE		44100
int				BufferIndex = 0;

// ���� 10���� pcm ����Ÿ�� �غ��Ѵ�.
/*void prepare_next2 (void *userdata, int len, int bufferIndex, unsigned char sampleSize)
{
	IMSManager* pManager = (IMSManager*)userdata;

	Rol *m_pRol = pManager->GetRol();

	for(int i=0; i<PREPARE_BUFFER_COUNT; i++)
	{
		int ToWrite, Samples;
		int TickDelay = 0;
		short int *BufferPos;

		//ToWrite = BUFFERSIZE / 2;
		ToWrite = len / sampleSize;

		BufferPos = (short int *)pcm_buffer[bufferIndex][i];

		while (ToWrite > 0) {
			if (RemainSamples > 0) Samples = RemainSamples;
			else {
				TickDelay = 0;

				TickDelay = m_pRol->PlayEvent();
			//	Samples = MulDivA(WAVE_SAMPLING_RATE * 60, TickDelay, m_pRol->GetBasicTempo() * 240);      
				Samples = MulDivA(22050 * 60, 
					TickDelay, m_pRol->ROLTempo * 240);
			}

			if (Samples > ToWrite) {
				YM3812UpdateOne(ym3812p, BufferPos, ToWrite);
				RemainSamples = Samples - ToWrite;
				break;
			} else {
				YM3812UpdateOne(ym3812p, BufferPos, Samples);
				RemainSamples = 0;
			}

			BufferPos = BufferPos + Samples;
			ToWrite = ToWrite - Samples;
		}

		if ( m_playMode == STOP || m_playMode == STOPPED || 
			m_playMode == SONG_END)
		{
			// ����� ��ġ�� ����
			m_endBuffer = bufferIndex;
			m_endBufferIndex = i;
			return;
		}
	}
}*/

// ���� 10���� pcm ����Ÿ�� �غ��Ѵ�.
void prepare_next2 (void *userdata, int len, int bufferIndex, unsigned char sampleSize)
{
	if ( m_playMode == STOP || m_playMode == STOPPED ||
	  m_playMode == SONG_END ) return;

	IMSManager* pManager = (IMSManager*)userdata;

	Rol *m_pRol = pManager->GetRol();

	for(int i=0; i<PREPARE_BUFFER_COUNT; i++)
	{
		static int remain_samples = 0;
		int to_write_len, samples;
		int delayTime = 0;
		short int *pcm_buffer_pos;
		pcm_buffer_pos = (short int *)pcm_buffer[bufferIndex][i];

		to_write_len = len / sampleSize;
		
		// ƽ�� �����Ѵ�.
		int tick = m_pRol->GetCurrentTick();
		song_tick[bufferIndex][i] = tick;
				
	// �ֵ帳 PCM ����Ÿ�� ���� ���ۿ� �����Ѵ�.
		while (to_write_len > 0) 
		{
			if (remain_samples > 0) 
			{
				samples = remain_samples;
			}
			else 
			{
				// �д� ���ڼ�
				int basicTempo = m_pRol->GetBasicTempo();

				delayTime = m_pRol->PlayEvent();

				// �� ��� ������ �������� ����Ѵ�.
				samples = MulDivA(22050 * 60,
						delayTime,  m_pRol->ROLTempo * 240);
			}

			if (samples > to_write_len) 
			{
				// �ֵ帳 PCM ����Ÿ�� ���� ���ۿ� �����Ѵ�.
				YM3812UpdateOne(ym3812p, pcm_buffer_pos, to_write_len);
				remain_samples = samples - to_write_len;
				break;
			}
			else 
			{
				// �ֵ帳 PCM ����Ÿ�� ���� ���ۿ� �����Ѵ�.
				YM3812UpdateOne(ym3812p, pcm_buffer_pos, samples);
				remain_samples = 0;
			}

			pcm_buffer_pos = pcm_buffer_pos + samples;
			to_write_len = to_write_len - samples;
		}

		if(remain_samples > 0)
		{
			int I = 0;
		}

		if ( m_playMode == STOP || m_playMode == STOPPED || 
				m_playMode == SONG_END)
		{
			// ����� ��ġ�� ����
			m_endBuffer = bufferIndex;
			m_endBufferIndex = i;
			return;
		}
	}
}
