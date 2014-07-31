#include "PlayerPlatform.h"
#include "Player.h"
#include "PlayerFSM.h"
#include "GameLayer.h"

PlayerPlatform::PlayerPlatform(Player* pPlayer)
: PlayerState(pPlayer, PLAYER_PLATFORM)
{
	m_dir = 0;
}


PlayerPlatform::~PlayerPlatform()
{
}

bool PlayerPlatform::IsValidGroundPos(CCPoint pos)
{
	/*Player* pOwner = GetOwner();
	GameLayer* pLayer = pOwner->GetGameLayer();
	int width = pLayer->_gameMap->getMapSize().width;
	int groundWidth = pLayer->_gameMap->getTileSize().width * width;

	if (pos.x > 0 && pos.x < groundWidth)
		return true;*/

	return true;
}


bool PlayerPlatform::CheckRopeInteraction(CCPoint& touchPoint)
{
	Player* pOwner = GetOwner();
	GameLayer* pLayer = pOwner->GetGameLayer();

	bool bIntersect = pLayer->CheckIntersect(touchPoint, pLayer->_ropeLayer);
	bool bIntersect2 = pLayer->CheckIntersect(pOwner->getPosition(), pLayer->_ropeLayer);

	if (touchPoint.y < pOwner->getPosition().y)
	{
		CCPoint belowTilePos = pOwner->getPosition();
		belowTilePos.y = belowTilePos.y - pLayer->_gameMap->getTileSize().height;

		if (bIntersect && pLayer->CheckIntersect(belowTilePos, pLayer->_ropeLayer))
		{
			pOwner->setPosition(belowTilePos);
			pOwner->SetDesiredPosition(belowTilePos);
			return true;
		}
	}
	//주인공과 로프가 겹쳐 있는 상태에서 로프를 클릭.. 로프액션 상태로 전이
	else if (bIntersect == true && bIntersect2 == true)
	{
		return true;
	}

	return false;
}


void PlayerPlatform::OnTouch(CCPoint& touchPoint)
{
	Player* pOwner = GetOwner();
	GameLayer* pLayer = pOwner->GetGameLayer();

	if (IsValidGroundPos(touchPoint) == false)
		return;

	if (true == CheckRopeInteraction(touchPoint))
	{
		pOwner->ResetPhysicsParam();
		pOwner->GetFSM()->ChangeState(PLAYER_ROPE);
		return;
	}



	m_targetPos = pOwner->getPosition();

	if (touchPoint.x > pOwner->getPosition().x)
	{
		m_dir = 1;
	}
	else if (touchPoint.x < pOwner->getPosition().x)
	{
		m_dir = -1;
	}
	else
	{
		m_dir = 0;
	}

	if (touchPoint.y - pOwner->getPosition().y > 50)
	{
		pOwner->mightAsWellJump = true;

		if (touchPoint.x - pOwner->getPosition().x > 40)
			pOwner->jumpingForforwardMarch = 1;
		else if (touchPoint.x - pOwner->getPosition().x < -40)
			pOwner->jumpingForforwardMarch = -1;
		else
			pOwner->jumpingForforwardMarch = 0;

	}

	m_targetPos.x = touchPoint.x;
}

void PlayerPlatform::OnMoved(CCPoint& touchPoint)
{
	Player* pOwner = GetOwner();

	if (touchPoint.x > pOwner->getPosition().x)
	{
		pOwner->forwardMarch = 1;
	}
	else if (touchPoint.x < pOwner->getPosition().x)
	{
		pOwner->forwardMarch = -1;
	}
	else
	{
		pOwner->forwardMarch = 0;
	}

	m_targetPos = touchPoint;
}

void PlayerPlatform::OnTouchEnd(CCPoint& touchPoint)
{
	Player* pOwner = GetOwner();
	pOwner->forwardMarch = 0;
}

void PlayerPlatform::CheckCollison(GameLayer* pLayer)
{
	Player* p = GetOwner();

	CCSprite *platform = (CCSprite*)p->_curPlatform;

	CCSize platform_size = platform->getContentSize();
	CCPoint platform_pos = platform->getPosition();

	int min_x = platform_pos.x - platform_size.width / 2 - 10;
	int max_x = platform_pos.x + platform_size.width / 2 + 10;

	CCPoint pos = p->getPosition();

	float min_y = platform_pos.y + (platform_size.height + p->getContentSize().height) / 2 - kPlatformTopPadding;

	if (pos.x > max_x ||
		pos.x < min_x)
	{
		p->GetFSM()->ChangeState(PLAYER_VOID);
		return;
	}

	p->setPosition(p->desiredPosition);

}

void PlayerPlatform::update(float dt)
{
	Player* pOwner = GetOwner();

	if (m_targetPos.x == pOwner->getPosition().x)
		return;

	CCPoint forwardMove = ccp(800.0, 0.0);
	CCPoint forwardStep = ccpMult(forwardMove, dt);

	CCPoint& velocity = pOwner->GetVelocity();
	//velocity.x = 0;

	if (m_dir == 1)
	{
		if (m_targetPos.x - pOwner->getPosition().x > 0)
		{
			velocity = ccpAdd(velocity, forwardStep);
		}
		else
		{
			m_dir = 0;
			pOwner->setPositionX(m_targetPos.x);
		}
	}
	else if (m_dir == -1)
	{
		if (m_targetPos.x - pOwner->getPosition().x < 0)
		{
			velocity = ccpAdd(velocity, forwardStep * -1);
		}
		else
		{
			m_dir = 0;
			pOwner->setPositionX(m_targetPos.x);
		}
	}
	else
	{
		velocity.x = 0;
	}


	CCPoint minMovement = ccp(-120.0, -450.0);
	CCPoint maxMovement = ccp(120.0, 250.0);
	velocity = ccpClamp(velocity, minMovement, maxMovement);

	CCPoint stepVelocity = ccpMult(velocity, dt);

	if (pOwner->mightAsWellJump == true)
	{
		CCPoint jumpForce = ccp(0.0, 310.0);
		velocity = ccpAdd(velocity, jumpForce);
		SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");

		pOwner->GetFSM()->ChangeState(PLAYER_VOID);
	}

	pOwner->SetDesiredPosition(ccpAdd(pOwner->getPosition(), stepVelocity));

	if (pOwner->desiredPosition.x < pOwner->getContentSize().width / 2)
		pOwner->desiredPosition.x = pOwner->getContentSize().width / 2;
	if (pOwner->desiredPosition.x > 320 - pOwner->getContentSize().width / 2)
		pOwner->desiredPosition.x = 320 - pOwner->getContentSize().width / 2;
}

void PlayerPlatform::OnEnter(PlayerState* pPrevState)
{
	Player* pOwner = GetOwner();
	m_targetPos = pOwner->getPosition();

	m_dir = 0;
	pOwner->velocity = ccp(0, 0);
	pOwner->mightAsWellJump = false;

}

void PlayerPlatform::OnExit()
{

	Player* pOwner = GetOwner();
	pOwner->_curPlatform = 0;
}