#pragma once
#include "AGGameState.h"

class AGGameMain : public AGGameState
{
public:
	AGGameMain(void);
	virtual ~AGGameMain(void);

	virtual bool OnEnter();
	virtual bool OnLeave();
};
