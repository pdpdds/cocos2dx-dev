#ifndef _IMSMANAGER_H_
#define _IMSMANAGER_H_

#include "IIMSEventCallback.h"
#include <string>
#include "Ims.h"

class Bnk;
class Iss;
class Rol;
class Hangul;

struct SDL_AudioSpec;

class IMSManager
{
public:
	IMSManager(void);
	virtual ~IMSManager(void);

	bool Initialize();
	bool GetIMSFileInfo(std::string szName, IMS_HEADER& sIMSHeader);
	static bool read_text(const char* source_file, char** destination, int& length);
	bool LoadIMS(char* szImsFile);
	bool LoadRol(char* szRolFile, char* m_szBnkFile, char* szIssFile);
	void Run();
	void Stop();

	SDL_AudioSpec* GetAudioSpec(){return m_pSpec;}


	Ims* GetIms(){return m_pIms;}
	Iss* GetIss(){return m_pIss;}
	Rol* GetRol(){return m_pRol;}

	int GetPlayStatus();
	std::string ConvertLyrics(char* lyrics);	

	IIMSEventCallback* GetIMSEventCallback(){return m_pIMSEventCallback;}
	void SetIMSEventCallback(IIMSEventCallback* pCallback){m_pIMSEventCallback = pCallback;}

protected:
	

private:
	Ims* m_pIms;
	Bnk* m_pBnk;
	Iss* m_pIss;
	Rol* m_pRol;

	SDL_AudioSpec* m_pSpec;

	Hangul* m_pHangul;

	IIMSEventCallback* m_pIMSEventCallback;
};
#endif

