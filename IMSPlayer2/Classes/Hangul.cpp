#include "Hangul.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#endif

// http://lazysoul.com/tc/32

// 종성의 벌수를 결정
char _dml[4][22]=
{ 
	{0,0,2,0,2,1,2,1,2,3,0,2,1,3,3,1,2,1,3,3,1,1},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,2,0,2,1,2,1,2,3,0,2,1,3,3,1,2,1,3,3,1,1},
	{0,1,1,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,0,0,1,1} 
};

// 중성의 벌수를 결정
char _dfm[4][20][2]=
{ 
	{1,3,0,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,0,2,1,3,1,3,1,3},
	{0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,3,0,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,0,2,1,3,1,3,1,3},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} 
};
// 초성의 벌수를 결정
char _dmf[4][22][2]=
{ 
	{0,0,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,1,6,3,7,3,7,3,7,1,6,2,6,4,7,4,7,4,7,2,6,1,6,3,7,0,5},
	{0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,2,3,4,5,4,5,4,5,2,3,3,3,4,5,4,5,4,5,3,3,2,3,4,5,0,1},
	{0,0,0,5,0,5,0,5,0,5,0,5,0,5,0,5,0,5,1,6,3,8,3,8,3,8,1,6,2,7,4,9,4,9,4,9,2,7,1,6,3,8,0,5},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0} 
};

char sangyongtable[3][33] = 
{
	// chosung
	{ 
		0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
		0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
		0x0F, 0x10, 0x11, 0x12, 0x13, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 
	},
	// jungsung
	{
	   0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
	   0x00, 0x00, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
	   0x00, 0x00, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
	   0x00, 0x00, 0x12, 0x13, 0x14, 0x15, 0x00, 0x00, 0x02
	},
	// jongsung
	{
	   0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
	   0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
	   0x0F, 0x10, 0x00, 0x11, 0x12, 0x13, 0x14, 0x15,
	   0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x00, 0x00, 0x01
	
	} 
};

Hangul::Hangul ()
{
	_xpos=0;
	_ypos=0;
	_totalwidth=0;
	
	engflag=0;
	specialenable=0;    /* special character not enabled */
	graphokflag=0;      /* graphic character not enabled */
	hanjaokflag=0;      /* hanjaokflag not enabled       */
	textattrflag=0;     /* textattribute not enabled     */
}

Hangul::~Hangul ()
{
	FreeHanRoom();
	FreeEngRoom();
	FreeSpecialRoom();
}

void Hangul::SetSpecialCharOn()
{
	specialenable=1;
}

void Hangul::SetSpecialCharOff()
{
	specialenable=0;
}

/* SANGYONG Code Format */
void Hangul::SangYongCode()
{
	code_table[0]=&sangyongtable[0][0];
	code_table[1]=&sangyongtable[1][0];
	code_table[2]=&sangyongtable[2][0];
}

void Hangul::SetHanFontRoom(int htype)
{
	int hsizef[4]={ 8*20*32,6*20*32,10*20*32,2*20*32 };
	int hsizem[4]={ 4*22*32,2*22*32, 4*22*32,1*22*32 };
	int hsizel[4]={ 4*28*32,1*28*32, 4*28*32,2*28*32 };

	fir=(char *)malloc((size_t)hsizef[htype]);
	mid=(char *)malloc((size_t)hsizem[htype]);
	las=(char *)malloc((size_t)hsizel[htype]);
}

void Hangul::SetEngFontRoom(int etype)
{
	int esize [1]={ 256*16 };  /* 16 is dummy */

	eng=(char *)malloc((size_t)esize [etype]);
}

void Hangul::SetSpecialFontRoom(int stype)
{
	int ssize = 128*32;

	stype&=0xaa;
	spe=(char *)malloc((size_t)ssize);
}

void Hangul::FreeHanRoom()
{
	free(fir);
	free(mid);
	free(las);

	fir=mid=las=NULL;
}
void Hangul::FreeEngRoom()
{
	free(eng);

	eng=NULL;
}
void Hangul::FreeSpecialRoom()
{
	free(spe);

	spe=NULL;
}

/*bool Hangul::LoadHanFont(wxString file, int htype)
{
	int hsizef[4]={ 8*20*32,6*20*32,10*20*32,2*20*32 };
	int hsizem[4]={ 4*22*32,2*22*32, 4*22*32,1*22*32 };
	int hsizel[4]={ 4*28*32,1*28*32, 4*28*32,2*28*32 };
	int handle;

	htypes.firs=hsizef[htype];
	htypes.mids=hsizem[htype];
	htypes.lass=hsizel[htype];

	char buf[1024];
	strcpy( buf, (const char*)file.mb_str(wxConvUTF8) );
	
	handle=open(buf, O_RDWR|O_BINARY);

	read(handle,fir,hsizef[htype]);
	read(handle,mid,hsizem[htype]);
	read(handle,las,hsizel[htype]);

	close(handle);

	SetCtable(htype);
}

void Hangul::LoadEngFont(wxString file,int etype)
{
	int esize [1]={ 256*16 };
	int handle;

	etypes.engs=esize[etype];

	char buf[1024];
	strcpy( buf, (const char*)file.mb_str(wxConvUTF8) );

	handle=open(buf, O_RDWR|O_BINARY);
	read(handle,eng,esize[etype]);
	close(handle);
}

void Hangul::LoadSpecialFont(wxString file,int stype)
{
	int ssize=128*32;
	int handle;
	
	char buf[1024];
	strcpy( buf, (const char*)file.mb_str(wxConvUTF8) );

	stype&=0xaa;	// warning을 위한 코드이며 아무 의미 없음
	handle=open(buf, O_RDWR|O_BINARY);
	read(handle,spe,ssize);
	close(handle);
}*/

void Hangul::SetCtable(int method)
{
	Dml=&_dml[method][0];
	Dfm=&_dfm[method][0][0];
	Dmf=&_dmf[method][0][0];
}

/*int Hangul::GetFontType(wxString file)
{
	FILE *fp;
	long size;

	char buf[1024];
	strcpy( buf, (const char*)file.mb_str(wxConvUTF8) );

	fp=fopen(buf, "r");

	fseek(fp,0,2);
	size=ftell(fp);

	fclose(fp);

	if (size<4000) return  HAN_2x1x2;
	if (size<5000) return  ENG_256;
	if (size<8000) return  HAN_6x2x1;
	if (size<12000) return HAN_8x4x4;

	return HAN_10x4x4;
}*/

void Hangul::complete_hangul(int mode, char *dest, char *src)
{
	// copy
	if(mode==1)
	{
		for(int loop=0;loop<32;loop++)
			*(dest+loop)=*(src+loop);
	}
	else
	{
		for(int loop=0;loop<32;loop++)
			*(dest+loop)|=*(src+loop);
	}
}

void Hangul::englishout_process(unsigned char *rgba, char *str,int pos)
{
	char ebuffer[16];

	engflag=1;
	memcpy(ebuffer,eng+((unsigned char)str[pos])*16,16);
	//attribute_process(ebuffer,16);
	//puteng(_xpos,_ypos,ebuffer,hgGetTFcolor());
	//PutEng(dc, rgba, ebuffer);
	_xpos+=8;
}

void Hangul::specialout_process(unsigned char *rgba, char *str,int pos)
{
	//printf("%d---\n", ((unsigned char)str[pos+1]-128)*32);
	//fflush(stdout);
	char buffer[32];

	memcpy(buffer,spe+((unsigned char)str[pos+1]-128)*32,32);
	//attribute_process(buffer,32);
	//puthan(_xpos,_ypos,buffer,hgGetTFcolor());
	//PutHan(dc, rgba, buffer);
	_xpos+=16;
}

void Hangul::hangulout_process(unsigned char *rgba, char *str, int pos)
{
	int loop,fcode,mcode,lcode;
	int f1=0,f2=0,f3=0;
	int hflag;
	char hbuffer[32];
	memset(hbuffer, 0x00, 32);

	han.b.fchar=str[pos];
	han.b.schar=str[pos+1];

	fcode = code_table[0][han.c.firc];
	mcode = code_table[1][han.c.midc];
	lcode = code_table[2][han.c.lasc];

	f3=Dml[mcode];
	f2=Dfm[fcode*2+(lcode!=0)];
	f1=Dmf[mcode*2+(lcode!=0)];

	hflag = 1;
	engflag=0;

	if (fcode){
		complete_hangul(1, hbuffer,fir+(f1*16+f1*4+fcode)*32);
		hflag = 0;
	}
	if (mcode){
		complete_hangul(hflag, hbuffer,mid+(f2*22+mcode)*32);
		hflag = 0;
	}
	if (lcode){
		complete_hangul(hflag, hbuffer,las+(f3*32-f3*4+lcode)*32);
		hflag = 0;
	}

	//wchar_t wszBuffer[256];
	// cp949 -> unicode
	//MultiByteToWideChar(CP_ACP, 0, hbuffer, -1, wszBuffer, 256);
	//wprintf(L"%s", wszBuffer);
	//attribute_process(hbuffer,32);
	//puthan(_xpos,_ypos,hbuffer,hgGetTFcolor());
	//PutHan(dc, rgba, hbuffer);
 	_xpos+=16;

}

void Hangul::PutHan(unsigned char *rgba, char *buffer)
{
	/*int c=0;
	for ( int i = 0 ; i < 16 ; i++ )		// 16 Lines per Font/Char
	{
		for ( int j = 0 ; j < 2 ; j++ )		// 16 x 16 (2 Bytes)
		{
			for( int loop=0; loop<8; loop++ )
			{
				int x = ((loop+(j*8))*4)+(_xpos*4);
				int y = i*_totalwidth*4;
				c = x+y;

				if( buffer[i*2+j] & (0x80>>loop)) 
				{
					rgba[c] = _fg.Red();
					rgba[c+1] = _fg.Green();
					rgba[c+2] = _fg.Blue();
					rgba[c+3] = _fg.Alpha();
				}
				else                                         
				{
					rgba[c] = _bg.Red();
					rgba[c+1] = _bg.Green();
					rgba[c+2] = _bg.Blue();
					rgba[c+3] = _bg.Alpha();
				}
			}
		}
	}*/
}

void Hangul::PutEng(unsigned char *rgba, char *buffer)
{
	/*int c=0;
	for ( int i = 0 ; i < 16 ; i++ )		// 16 Lines per Font/Char
	{
		for( int loop=0; loop<8; loop++ )
		{
			int x = (loop*4)+(_xpos*4);
			int y = i*_totalwidth*4;
			c = x+y;

			if( buffer[i] & (0x80>>loop)) 
			{
				rgba[c] = _fg.Red();
				rgba[c+1] = _fg.Green();
				rgba[c+2] = _fg.Blue();
				rgba[c+3] = _fg.Alpha();
			}
			else                                         
			{
				rgba[c] = _bg.Red();
				rgba[c+1] = _bg.Green();
				rgba[c+2] = _bg.Blue();
				rgba[c+3] = _bg.Alpha();
			}
		}
	}*/
}

int Hangul::IsSpecial(char *str,int pos)
{
	if ((unsigned char)str[pos]==SPECIAL_CODE && (unsigned char)str[pos]>=0x80)
		return true;
	else
		return false;
}

int Hangul::IsHanja(char *str,int pos)
{
	unsigned char hc1,hc2;

	hc1=str[pos];
	hc2=str[pos+1];

	if ((hc1>=0xe0 && hc1<=0xfa) &&
	   ( (hc2>=0x31 && hc2<=0x7e) ||
		 (hc2>=0x91 && hc2<=0xfe) ) )
		return true;
	else   
		return false;
}
int Hangul::IsGraphic(char *str,int pos)
{
	unsigned fontID[]={ 0xd480,0xd931,0xda31,0xdb31,0xdc31,0xdd31,0xde31 };
	unsigned char hc1,hc2;
	int      i;

	for (i=0;i<7;i++) {
		hc1=(fontID[i]>>8)&0xff;
		hc2=(fontID[i]&0xff);
		if (((unsigned char)str[pos])==hc1 && ((unsigned char)str[pos+1])>=hc2)
			 return true;
	}
	return false;
}

void Hangul::ConvertLyrics(char *str)
{
	if ( strlen(str) == 0 ) return;

	int pos=0;	/* string position */
	int flag;
	
	_totalwidth=strlen(str)*8;
	unsigned char *rgba = (unsigned char *)malloc(_totalwidth*16*4);

	while (1)
	{
		if ( pos >= strlen(str) ) break;

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

		if (str[pos]=='\t') {
			_xpos=_xpos+(8-(_xpos/8)%8)*8;
			pos++;
		} else if (!(str[pos]&0x80)) {
			englishout_process(rgba, str, pos);
			printf("%c", *(str+pos));
			pos++;
		} else if (specialenable && IsSpecial(str, pos)) {
			specialout_process(rgba, str, pos);
			pos+=2;
		} else if (graphokflag && IsGraphic(str, pos)) {
			//graphout_process(str,pos);
			pos+=2;
		} else if (hanjaokflag && IsHanja(str, pos)) {
			//hanjaout_process(str,pos);
			pos+=2;
		} else {
			hangulout_process(rgba, str, pos);
			pos+=2;
		}
	};

	//wxBitmap *bitmap = RGBAtoBitmap(rgba, _totalwidth, 16);
	//dc.DrawBitmap(*bitmap, x, y, true);
	//delete bitmap;
	
	if(pos>0 && rgba != NULL)
	{
		free(rgba);
		rgba=NULL;
	}
}
