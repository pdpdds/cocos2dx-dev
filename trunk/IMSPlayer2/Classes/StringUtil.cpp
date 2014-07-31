#include "StringUtil.h"
/*
std::string ws2s(const std::wstring& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0); 
    char* buf = new char[len];
    WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0); 
    std::string r(buf);
    delete[] buf;
    return r;
}*/

std::string ConvertExt(std::string& szFileName, std::string strModExt)
{
	string strReName;
	int nExt = szFileName.rfind(".") + 1;

	strReName = szFileName.substr(0, nExt);
	//strReName += strFilePath.substr(nName, nExt - nName);
	strReName += strModExt;

	return strReName;
}

std::string GetExt(std::string& szPassName)
{
	return szPassName.substr(szPassName.find_last_of(".") + 1);
}
/*
std::wstring s2ws(const std::string& s)
{
    int len;

    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}*/