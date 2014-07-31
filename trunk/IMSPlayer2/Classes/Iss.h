#ifndef _ISS_H_
#define _ISS_H_

#include <string>

typedef struct {
	char head_str[20];  // ó���� 'I' 'M' 'P'�� �����մϴ�.
	char reserved[10];  // �׳� ����� �� ����
	char writer[30];  // �ۻ簡
	char composer[30];  // �۰
	char singer[30];  // ����
	char editor[30];  // ISS ������
	unsigned short rec_count;
	unsigned short line_count;
} ISS_HEADER;

typedef struct {
	unsigned short kasa_tick;  // �� �κ��� ó���� ������ ƽ���� 8�� ���� ���̶��ϴ�
	char line;  // ���� ���� ��ȣ (0���� �����Դϴ�)
	char start_x;  // �ٿ��� ���۵� ���� ��ġ (0���� �����Դϴ�)
	char width_x;  // ��ĥ�� ���� �� (�翬�� �ѱ��� 2���� �Դϴ�)
} ISS_RECORD;

typedef struct {
	char script[64];
} ISS_SCRIPT;

typedef struct {
} MYADLIB_ISS;

class Iss
{
public:
	std::string m_fileName; // ��θ� ������ ���ϸ�
	int	m_fileSize; // raw ���۷� �о���� ���� ũ��

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
