#pragma once
#include "AGGameFSM.h"

class AGGameMgr
{
	

public:
	AGGameMgr(void);
	virtual ~AGGameMgr(void);

	bool Initialize();
	bool Run();

protected:

private:
	AGGameFSM m_GameFSM;
};
