#pragma once

class AGGameState
{
public:
	AGGameState(void);
	virtual ~AGGameState(void);

	virtual bool OnEnter() = 0;
	virtual bool OnLeave() = 0;
};
