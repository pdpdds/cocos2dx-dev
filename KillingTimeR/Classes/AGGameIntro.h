#pragma once
#include "AGGameState.h"

class AGGameIntro : public AGGameState
{
public:
	AGGameIntro(void);
	virtual ~AGGameIntro(void);

	virtual bool OnEnter();
	virtual bool OnLeave();
};
