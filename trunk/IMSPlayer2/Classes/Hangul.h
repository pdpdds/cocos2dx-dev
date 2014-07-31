#ifndef _HANGUL_H_
#define _HANGUL_H_

extern char _dml[4][22];
extern char _dfm[4][20][2];
extern char _dmf[4][22][2];
extern char sangyongtable[3][33];

#define WXDC			0
#define CONSOLE			1

/* font format */
#define HAN_8x4x4       0
#define HAN_6x2x1       1
#define HAN_10x4x4      2
#define HAN_2x1x2       3
#define ENG_256         0
#define SPECIAL_CODE    212

/* hangul type & english type */
typedef struct htype {
	int firs;
	int mids;
	int lass;
} HTYPE;

typedef struct etype {
	int engs;
} ETYPE;

typedef struct {
	unsigned lasc :5;
	unsigned midc :5;       /* HAN CODE FORM */
	unsigned firc :5;
	unsigned dummy:1;
} hcode;

typedef struct {
	unsigned char schar;    /* HAN BYTE FORM */
	unsigned char fchar;
} hchar;


class Hangul
{
public:
	char *fir;		/* first 	*/
	char *mid;		/* mid   	*/
	char *las;		/* las   	*/
	char *spe; 		/* special font */
	char *eng;		/* english 	*/
	
	HTYPE htypes;
	ETYPE etypes;

	union {
		hcode c;
		hchar b;
	} han;
	

	char *Dml;               /* convert table ( middle-last  ) */
	char *Dfm;               /* convert table ( first-middle ) */
	char *Dmf;               /* convert table ( middle-first ) */

	char *code_table[3];
	
	/* general x,y position */
	int _xpos;
	int _ypos;
	int _totalwidth;

	/* flags */
	char engflag;
	char specialenable;    /* special character not enabled */
	char graphokflag;      /* graphic character not enabled */
	char hanjaokflag;      /* hanjaokflag not enabled       */
	char textattrflag;     /* textattribute not enabled     */

	int putmodeflag;

	int _from;
	int _to;

public:
	Hangul();
	~Hangul();

	void SetSpecialCharOn();
	void SetSpecialCharOff();

	void SetCtable(int method);
	void SangYongCode();

	void ConvertLyrics(char *str);

	//bool LoadHanFont(wxString file, int htype);
	//void LoadEngFont(wxString file,int etype);
	//void LoadSpecialFont(wxString file,int stype);

	void SetHanFontRoom(int htype);
	void SetEngFontRoom(int etype);
	void SetSpecialFontRoom(int stype);
	void FreeHanRoom();
	void FreeEngRoom();
	void FreeSpecialRoom();

	//int GetFontType(wxString file);

	void complete_hangul(int mode, char *dest, char *src);
	void specialout_process(unsigned char *rgba, char *str,int pos);
	void englishout_process(unsigned char *rgba, char *str,int pos);
	void hangulout_process(unsigned char *rgba, char *str, int pos);

	int IsGraphic(char *str,int pos);
	int IsHanja(char *str,int pos);
	int IsSpecial(char *str,int pos);

	void PutHan(unsigned char *rgba, char *buffer);
	void PutEng(unsigned char *rgba, char *buffer);
};

#endif	//__BNK_H_
