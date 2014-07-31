#include "PlayerGround.h"
#include "Player.h"
#include "PlayerFSM.h"
#include "GameLayer.h"

PlayerGround::PlayerGround(Player* pPlayer)
: PlayerState(pPlayer, PLAYER_GROUND)
{
	m_dir = 0;
}


PlayerGround::~PlayerGround()
{
}

bool PlayerGround::IsValidGroundPos(CCPoint pos)
{
	Player* pOwner = GetOwner();
	GameLayer* pLayer = pOwner->GetGameLayer();
	int width = pLayer->_gameMap->getMapSize().width;
	int groundWidth = pLayer->_gameMap->getTileSize().width * width;

	if (pos.x > 0 && pos.x < groundWidth)
		return true;

	return false;
}


bool PlayerGround::CheckRopeInteraction(CCPoint& touchPoint)
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


void PlayerGround::OnTouch(CCPoint& touchPoint)
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
	else if(touchPoint.x < pOwner->getPosition().x)
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

void PlayerGround::OnMoved(CCPoint& touchPoint)
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

void PlayerGround::OnTouchEnd(CCPoint& touchPoint)
{
	Player* pOwner = GetOwner();
	pOwner->forwardMarch = 0;
}

/*void PlayerGround::update(float dt)
{
	Player* pOwner = GetOwner();

	CCPoint forwardMove = ccp(800.0, 0.0);
	CCPoint forwardStep = ccpMult(forwardMove, dt);

	CCPoint& velocity = pOwner->GetVelocity();

	velocity = ccp(velocity.x * 0.90, 0);

	if (pOwner->forwardMarch > 0) {
		velocity = ccpAdd(velocity, forwardStep);
	}
	else if (pOwner->forwardMarch < 0) {
		velocity = ccpAdd(velocity, forwardStep * -1);
	}
	

	CCPoint jumpForce = ccp(0.0, 310.0);
	float jumpCutoff = 150.0;

	if (pOwner->mightAsWellJump) {
		velocity = ccpAdd(velocity, jumpForce);
		//SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");
	}
	else if (!pOwner->mightAsWellJump && velocity.y > jumpCutoff) {
		velocity = ccp(velocity.x, jumpCutoff);
	}

	CCPoint minMovement = ccp(-120.0, -450.0);
	CCPoint maxMovement = ccp(120.0, 250.0);
	velocity = ccpClamp(velocity, minMovement, maxMovement);

	CCPoint stepVelocity = ccpMult(velocity, dt);

	if (pOwner->mightAsWellJump == true)
	{
		pOwner->GetFSM()->ChangeState(PLAYER_VOID);
	}

	pOwner->SetDesiredPosition(ccpAdd(pOwner->getPosition(), stepVelocity));

	if (pOwner->desiredPosition.x < pOwner->getContentSize().width / 2)
		pOwner->desiredPosition.x = pOwner->getContentSize().width / 2;
	if (pOwner->desiredPosition.x > 320 - pOwner->getContentSize().width / 2)
		pOwner->desiredPosition.x = 320 - pOwner->getContentSize().width / 2;
}*/

void PlayerGround::CheckCollison(GameLayer* pLayer)
{
	Player* p = GetOwner();
	CCArray *tiles = pLayer->getSurroundingTilesAtPosition(p->getPosition(), pLayer->_toeholdLayer);

	bool bGround = false;

	CCObject* it = NULL;
	CCARRAY_FOREACH(tiles, it)
	{
		CCDictionary *dic = dynamic_cast<CCDictionary*>(it);
		CCRect pRect = p->boundingBox();

		CCInteger* pValue = (CCInteger*)dic->objectForKey("gid");

		if (pValue->getValue() && pValue->getValue() != -1) {
			CCFloat* pValue2 = (CCFloat*)dic->objectForKey("x");
			CCFloat* pValue3 = (CCFloat*)dic->objectForKey("y");

			CCRect tileRect = CCRectMake(pValue2->getValue(),
				pValue3->getValue(),
				pLayer->_gameMap->getTileSize().width,
				pLayer->_gameMap->getTileSize().height);

			if (pRect.intersectsRect(tileRect)) {

				CCRect intersection = CCRectMake(max(pRect.getMinX(), tileRect.getMinX()), max(pRect.getMinY(), tileRect.getMinY()), 0, 0);
				intersection.size.width = min(pRect.getMaxX(), tileRect.getMaxX()) - intersection.getMinX();
				intersection.size.height = min(pRect.getMaxY(), tileRect.getMaxY()) - intersection.getMinY();
				//CCRect intersection = CCRectIntersection(pRect, tileRect);
				int tileIndx = tiles->indexOfObject(dic);

				if (tileIndx == 0) {
					p->SetDesiredPosition(ccp(p->desiredPosition.x, p->desiredPosition.y + intersection.size.height));
					bGround = true;
				}
				else if (tileIndx == 1) {
					//tile is directly above player
					//	p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y - intersection.size.height);
					//	p->velocity = ccp(p->velocity.x, 0.0); //////Here
				}
				else if (tileIndx == 2) {
					//tile is left of player
							p->desiredPosition = ccp(p->desiredPosition.x + intersection.size.width, p->desiredPosition.y);
				}
				else if (tileIndx == 3) {
					//tile is right of player
						p->desiredPosition = ccp(p->desiredPosition.x - intersection.size.width, p->desiredPosition.y);
				}
				else if (tileIndx == 8)
				{
					int i = 1;
				}
				else {
					/*if (intersection.size.width > intersection.size.height) {
					//tile is diagonal, but resolving collision vertially
					p->velocity = ccp(p->velocity.x, 0.0); //////Here
					float resolutionHeight;
					if (tileIndx > 5) {
					resolutionHeight = intersection.size.height;
					p->onGround = true; //////Here
					}
					else {
					resolutionHeight = -intersection.size.height;
					}
					p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y + resolutionHeight);

					}
					else {
					float resolutionWidth;
					if (tileIndx == 6 || tileIndx == 4) {
					resolutionWidth = intersection.size.width;
					}
					else {
					resolutionWidth = -intersection.size.width;
					}
					p->desiredPosition = ccp(p->desiredPosition.x + resolutionWidth, p->desiredPosition.y);
					}*/
				}
			}
		}
	}
	p->setPosition(p->desiredPosition);

	if (bGround == false)
	{
		p->forwardMarch = 0;
		p->jumpingForforwardMarch = 0;
		p->GetFSM()->ChangeState(PLAYER_VOID);
	}
}

void PlayerGround::update(float dt)
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

void PlayerGround::OnEnter(PlayerState* pPrevState)
{
	Player* pOwner = GetOwner();
	m_targetPos = pOwner->getPosition();

	m_dir = 0;
	pOwner->velocity = ccp(0, 0);

}

void PlayerGround::OnExit()
{

}