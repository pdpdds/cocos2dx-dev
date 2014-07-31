#pragma once
#include "IIMSEventCallback.h"

class IMSEventCallback : public IIMSEventCallback
{
public:
	IMSEventCallback(void);
	virtual ~IMSEventCallback(void);

	virtual void CallbackLyrics(char* lyrics) ;
	virtual void CallbackPlayEnd() ;
};

