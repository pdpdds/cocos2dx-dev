#pragma once
#include <string>

using namespace std;

std::string ws2s(const std::wstring& s);
std::string ConvertExt(std::string& szFileName, std::string strModExt);
std::string GetExt(std::string& szPassName);
std::wstring s2ws(const std::string& s);