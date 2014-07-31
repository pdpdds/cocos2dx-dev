#pragma once

class IIMSEventCallback
{
public:
	virtual void CallbackLyrics(char* lyrics) = 0;
	virtual void CallbackPlayEnd() = 0;
};