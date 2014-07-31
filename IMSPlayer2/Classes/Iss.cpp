#include "iss.h"
#include <stdlib.h>

Iss::Iss () 
{
	opened=false;
}

Iss::~Iss ()
{
	if ( opened == true )
	{
		free(m_record);
		free(m_script);
		free(raw);
	}
}

bool Iss::Open(std::string  file)
{
	FILE *fp;
	char signature[6+1];
	int i;

	m_fileName = file;

	//char path[1024];
	//strcpy( path, (const char*)file.mb_str(wxConvUTF8) ); 

	fp = fopen(file.c_str(), "rb");
	if (fp == 0) 
		return false;
	
	fseek(fp, 0, SEEK_END);
	m_fileSize = ftell(fp);

	raw = (char *)malloc(m_fileSize);
	if (raw == NULL) 
	{
		fclose(fp); 
		return false;
	}

	fseek(fp, 0, SEEK_SET);
	fread((char *)(raw), m_fileSize, sizeof(char), fp);
	rawPtr = raw;
	fclose(fp);

	// header
	memcpy(&m_header, rawPtr, sizeof(ISS_HEADER));
    rawPtr += sizeof(ISS_HEADER);
	
	// record
	m_record = (ISS_RECORD *)malloc(
			sizeof(ISS_RECORD) * m_header.rec_count);
	if (m_record == NULL) return NULL;

	//printf("-----raw_ptr: %d\n", rawPtr-raw);
    
	for (unsigned int x = 0; x < m_header.rec_count; x++) 
	{
		memcpy(&m_record[x].kasa_tick, rawPtr, sizeof(unsigned short));
        rawPtr += sizeof(unsigned short);
		memcpy(&m_record[x].line, rawPtr, sizeof(char));
        rawPtr += sizeof(char);
		memcpy(&m_record[x].start_x, rawPtr, sizeof(char));
        rawPtr += sizeof(char);
		memcpy(&m_record[x].width_x, rawPtr, sizeof(char));
        rawPtr += sizeof(char);
    }

	// script
	m_script = (ISS_SCRIPT *)malloc(
			sizeof(ISS_SCRIPT) * m_header.line_count);
	if (m_script == NULL) return NULL;
    
	for (unsigned int x = 0; x < m_header.line_count; x++) 
	{
		memcpy(&m_script[x], rawPtr, sizeof(ISS_SCRIPT));
        rawPtr += sizeof(ISS_SCRIPT);
    }

	/*
	// ----
	for (unsigned int x = 0; x < m_header.rec_count; x++) 
		printf("--%d-- kasa_tick: %d --- kasa_line: %d --\n", 
				x, m_record[x].kasa_tick, m_record[x].line);

	for (unsigned int x = 0; x < m_header.line_count; x++) 
	{
		char title[70];
		han_conv(0, m_script[x].script, title);
		//printf("--%d-- %s --\n", x, m_script[x].script);
		printf("--%d-- %s --\n", x, title);
	}

	//printf("\n-%d-line_count-- %d", sizeof(ISS_HEADER), m_header.line_count);
	//printf("\n-%d-rec_count-- %d", sizeof(ISS_RECORD), m_header.rec_count);
	

	//printf("----ISS_SCRIPT: %d--\n", sizeof(ISS_SCRIPT));

	//fflush(stdout);
	*/
	
	opened=true;

	return true;
}

// 작사가
char *Iss::GetWriter()
{
	return (char *)m_header.writer;
}

// 작곡가
char *Iss::GetComposer()
{
	return (char *)m_header.composer;
}

// 가수
std::string Iss::GetSinger()
{
	char singer[30];
	//han_conv(0, m_header.singer, singer);

	wchar_t wszBuffer[30];
	// cp949 -> unicode
	//MultiByteToWideChar(CP_ACP, 0, singer, -1, wszBuffer, 30);
	// unicode -> utf-8
	//WideCharToMultiByte(CP_UTF8, 0, wszBuffer, -1, singer, 30, NULL, NULL);

	return std::string(singer);
}

bool Iss::IsOpened()
{
	return opened;
}
