#ifndef _ISS_H_
#define _ISS_H_

#include <string>

typedef struct {
	char head_str[20];  // 처음에 'I' 'M' 'P'로 시작합니다.
	char reserved[10];  // 그냥 예약된 빈 영역
	char writer[30];  // 작사가
	char composer[30];  // 작곡가
	char singer[30];  // 가수
	char editor[30];  // ISS 제작자
	unsigned short rec_count;
	unsigned short line_count;
} ISS_HEADER;

typedef struct {
	unsigned short kasa_tick;  // 이 부분이 처리될 연주중 틱값을 8로 나눈 값이랍니다
	char line;  // 가사 줄의 번호 (0부터 시작입니다)
	char start_x;  // 줄에서 시작될 문자 위치 (0부터 시작입니다)
	char width_x;  // 색칠할 문자 수 (당연히 한글은 2글자 입니다)
} ISS_RECORD;

typedef struct {
	char script[64];
} ISS_SCRIPT;

typedef struct {
} MYADLIB_ISS;

class Iss
{
public:
	std::string m_fileName; // 경로를 포함한 파일명
	int	m_fileSize; // raw 버퍼로 읽어들인 실제 크기

	char *raw;
	char *rawPtr;
	
	ISS_HEADER m_header;
	ISS_RECORD *m_record;
	ISS_SCRIPT *m_script;

	bool opened;

public:
	Iss();
	~Iss();
	bool Open(std::string file);
	char *GetWriter();
	char *GetComposer();
	std::string GetSinger();
	bool IsOpened();
};

#endif	//_ISS_H_
