#pragma once
#include "AGGameState.h"

class AGGameTitle : public AGGameState
{
public:
	AGGameTitle(void);
	virtual ~AGGameTitle(void);


	virtual bool OnEnter();
	virtual bool OnLeave();
};
