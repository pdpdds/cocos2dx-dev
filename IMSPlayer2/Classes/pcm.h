#pragma once
#include "iss.h"

#define PREPARE_BUFFER_COUNT	20
#define PREPARE_BUFFER_SIZE		512*2

extern unsigned int song_tick[2][PREPARE_BUFFER_COUNT];
extern unsigned char *pcm_buffer[2][PREPARE_BUFFER_COUNT];
extern ISS_RECORD *lyrics_buffer[2][PREPARE_BUFFER_COUNT];

#define STOP		0
#define PLAYING		1
#define STOPPED		2
#define SONG_END	3

#define REPEAT_NO		0	// 지금 곡 연주하고 중지
#define REPEAT_ALL		1	// 지금 곡 연주 끝나면 다음곡으로..
#define REPEAT_THIS		2	// 지금 곡 반복 연주


extern int m_volume;
extern int m_playMode;
extern int m_repeatMode;

extern int m_playingBuffer;
extern int m_playingBufferIndex;

extern int m_endBuffer;
// 연주가 종료된 버퍼의 인덱스
extern int m_endBufferIndex;

extern int m_lastLyricsLine;
// 마지막 보여준 가사 레코드
extern int m_lastLyricsRecord;

void init_prepare_pcm_buffer(void);
void alloc_prepare_pcm_buffer (int bufferIndex);
void free_prepare_pcm_buffer (int bufferIndex);
void prepare_next (void *userdata, int len, int bufferIndex, unsigned char sampleSize);
void prepare_next2 (void *userdata, int len, int bufferIndex, unsigned char sampleSize);

extern int				BufferIndex;
