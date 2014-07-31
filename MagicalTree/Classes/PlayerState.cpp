#include "PlayerState.h"


PlayerState::PlayerState(Player* pOwner, int state)
: m_pOwner(pOwner)
, m_state(state)
{
}


PlayerState::~PlayerState()
{
}
