#pragma once
#include <map>

class AGGameState;

using namespace std;

class AGGameFSM
{
	typedef std::map<int, AGGameState*> GameStateMap;
public:
	AGGameFSM(void);
	virtual ~AGGameFSM(void);

	bool AddState(int GameState);
	bool ChangeState(int State);

private:
	GameStateMap m_GameStateMap;
	AGGameState* m_pCurrentState;
};
