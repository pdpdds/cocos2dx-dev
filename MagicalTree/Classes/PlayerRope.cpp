#include "PlayerRope.h"
#include "Player.h"
#include "PlayerFSM.h"
#include "GameLayer.h"


PlayerRope::PlayerRope(Player* pPlayer)
: PlayerState(pPlayer, PLAYER_ROPE)
{
	m_dir = 0;
}

PlayerRope::~PlayerRope()
{
}

void PlayerRope::OnTouch(CCPoint& touchPoint)
{
	Player* pOwner = GetOwner();
	if (touchPoint.y - pOwner->getPosition().y > 0)
	{
		m_dir = 1;
	}
	else if (touchPoint.y - pOwner->getPosition().y < 0)
	{
		m_dir = -1;
	}
	else
	{
		m_dir = 0;
	}

	m_targetPos = touchPoint;
}

void PlayerRope::OnMoved(CCPoint& touchPoint)
{
	Player* pOwner = GetOwner();
	
}

void PlayerRope::OnTouchEnd(CCPoint& touchPoint)
{
	Player* pOwner = GetOwner();
	pOwner->forwardMarch = 0;
}

void PlayerRope::update(float dt)
{
	Player* pOwner = GetOwner();

	if (m_dir == 0)
		return;

	CCPoint aboveMove = ccp(0.0, 100.0);
	CCPoint aboveStep = ccpMult(aboveMove, dt);

	CCPoint& velocity = pOwner->GetVelocity();

	velocity = ccp(0, aboveMove.y * m_dir);

	CCPoint stepVelocity = ccpMult(velocity, dt);

	pOwner->SetDesiredPosition(ccpAdd(pOwner->getPosition(), stepVelocity));
	pOwner->setPosition(pOwner->desiredPosition);

	if (m_dir == 1 && m_targetPos.y < pOwner->getPosition().y)
	{
		pOwner->setPositionY(m_targetPos.y);
		m_dir = 0;
	}
	else if(m_dir == -1 && m_targetPos.y > pOwner->getPosition().y)
	{
		pOwner->setPositionY(m_targetPos.y);
		m_dir = 0;
	}
}

void PlayerRope::CheckCollison(GameLayer* pLayer)
{
	Player* pOwner = GetOwner();

	bool bIntersect = pLayer->CheckIntersect(pOwner->getPosition(), pLayer->_ropeLayer);

	if (bIntersect == false)
	{
		pOwner->GetFSM()->ChangeState(PLAYER_VOID);
		return;
	}

	if (m_dir == -1)
	{
		CCPoint belowTilePos = pOwner->getPosition();
		belowTilePos.y = belowTilePos.y - pLayer->_gameMap->getTileSize().height;

		if (pLayer->CheckIntersect(belowTilePos, pLayer->_toeholdLayer))
		{
			pOwner->GetFSM()->ChangeState(PLAYER_VOID);
			return;
		}
	}
}

void PlayerRope::OnEnter(PlayerState* pPrevState)
{
	Player* pOwner = GetOwner();
	m_dir = 0;
	m_targetPos = pOwner->getPosition();
}

void PlayerRope::OnExit()
{
	Player* pOwner = GetOwner();
	pOwner->ResetPhysicsParam();
}