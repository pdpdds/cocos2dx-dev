#include "PlayerVoid.h"
#include "Player.h"
#include "GameLayer.h"
#include "PlayerFSM.h"

PlayerVoid::PlayerVoid(Player* pPlayer)
: PlayerState(pPlayer, PLAYER_VOID)
{
}


PlayerVoid::~PlayerVoid()
{
}

void PlayerVoid::OnTouch(CCPoint& touchPoint)
{

}

void PlayerVoid::OnMoved(CCPoint& touchPoint)
{

}

void PlayerVoid::OnTouchEnd(CCPoint& touchPoint)
{
	Player* pOwner = GetOwner();
	pOwner->forwardMarch = 0;
}

void PlayerVoid::update(float dt)
{
	Player* pOwner = GetOwner();
	CCPoint& velocity = pOwner->GetVelocity();
	CCPoint gravity = ccp(0.0, -450.0);
	CCPoint gravityStep = ccpMult(gravity, dt);
	

	CCPoint forwardMove = ccp(0.0, 0.0);

	if (pOwner->jumpingForforwardMarch > 0)
	{
		forwardMove = ccp(800.0, 0.0);	
	}
	else if (pOwner->jumpingForforwardMarch < 0)
	{
		forwardMove = ccp(-800.0, 0.0);
	}

	velocity = ccp(forwardMove.x, velocity.y); //2


	CCPoint minMovement = ccp(-120.0, -450.0);
	CCPoint maxMovement = ccp(120.0, 250.0);
	velocity = ccpClamp(velocity, minMovement, maxMovement);

	velocity = ccpAdd(velocity, gravityStep);
	CCPoint stepVelocity = ccpMult(velocity, dt);

	pOwner->desiredPosition = ccpAdd(pOwner->getPosition(), stepVelocity);

	if (pOwner->desiredPosition.x < pOwner->getContentSize().width / 2)
		pOwner->desiredPosition.x = pOwner->getContentSize().width / 2;
	if (pOwner->desiredPosition.x > 320 - pOwner->getContentSize().width / 2)
		pOwner->desiredPosition.x = 320 - pOwner->getContentSize().width / 2;

	GameLayer* pLayer = pOwner->GetGameLayer();
	bool bIntersect = pLayer->CheckIntersect(pOwner->getPosition(), pLayer->_ropeLayer);

	if (bIntersect == true && velocity.x != 0)
	{

		pOwner->ResetPhysicsParam();
		int tileWidth = pLayer->_gameMap->getTileSize().width;
		int pos = pOwner->getPosition().x;
		pOwner->desiredPosition.x = ((pos / tileWidth) * (tileWidth+1)) + (tileWidth / 2);
		pOwner->setPosition(pOwner->desiredPosition);
		pOwner->GetFSM()->ChangeState(PLAYER_ROPE);
		return;
	}

	if (velocity.y < 0)
	{
		CCArray* pArray = pOwner->GetGameLayer()->_platformArray;

		CCObject* it = NULL;
		CCARRAY_FOREACH(pArray, it)
		{
			
			
			CCSprite *platform = dynamic_cast<CCSprite*>(it);


			CCSize platform_size = platform->getContentSize();
			CCPoint platform_pos = platform->getPosition();

			int max_x = platform_pos.x - platform_size.width / 2 - 10;
			int min_x = platform_pos.x + platform_size.width / 2 + 10;

			CCPoint pos = pOwner->getPosition();

			float min_y = platform_pos.y + (platform_size.height + pOwner->getContentSize().height) / 2 - kPlatformTopPadding;

			if (pos.x > max_x &&
				pos.x < min_x &&
				pos.y > platform_pos.y &&
				pos.y < min_y)
			{
				pOwner->_curPlatform = platform;
				pOwner->GetFSM()->ChangeState(PLAYER_PLATFORM);
				return;
			}
		}
	}
}

void PlayerVoid::OnEnter(PlayerState* pPrevState)
{
	Player* pOwner = GetOwner();
	pOwner->mightAsWellJump = false;
}

void PlayerVoid::OnExit()
{
	Player* pOwner = GetOwner();
	pOwner->ResetPhysicsParam();
}

void PlayerVoid::CheckCollison(GameLayer* pLayer)
{
	Player* p = GetOwner();
	CCArray *tiles = pLayer->getSurroundingTilesAtPosition(p->getPosition(), pLayer->_toeholdLayer);

	//p->onGround = false;

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
					//tile is directly below player
					p->SetDesiredPosition(ccp(p->desiredPosition.x, p->desiredPosition.y + intersection.size.height));

					if (p->velocity.y <= 0)
					{

						p->velocity = ccp(p->velocity.x, 0.0); //////Here

						if (p->GetFSM()->GetState() != PLAYER_GROUND)
							p->GetFSM()->ChangeState(PLAYER_GROUND);
					}
				}
				else if (tileIndx == 1) {
					//tile is directly above player
					//	p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y - intersection.size.height);
					//	p->velocity = ccp(p->velocity.x, 0.0); //////Here
				}
				else if (tileIndx == 2) {
					//tile is left of player
					if (p->velocity.y <= 0)
					{
						//p->jumpingForforwardMarch = 0;
					//	p->desiredPosition = ccp(p->desiredPosition.x + intersection.size.width, p->desiredPosition.y);
					}
				}
				else if (tileIndx == 3) {
					//tile is right of player
					if (p->velocity.y <= 0)
					{
					//	p->jumpingForforwardMarch = 0;
					//	p->desiredPosition = ccp(p->desiredPosition.x - intersection.size.width, p->desiredPosition.y);
					}
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
}