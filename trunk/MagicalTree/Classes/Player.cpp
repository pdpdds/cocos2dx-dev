#include "Player.h"
#include "PlayerFSM.h"
#include "PlayerGround.h"
#include "PlayerVoid.h"
#include "PlayerRope.h"
#include "PlayerPlatform.h"
#include "GameLayer.h"

Player::Player(char* filename, GameLayer* pLayer)
: m_pLayer(pLayer)
{
	CCSprite::initWithFile("koalio_stand.png");
	velocity = ccp(0.0, 0.0);
	forwardMarch = 0;
	mightAsWellJump = false;

	m_pPlayerFSM = new PlayerFSM(this);
	m_pPlayerFSM->AddState(new PlayerGround(this));
	m_pPlayerFSM->AddState(new PlayerVoid(this));
	m_pPlayerFSM->AddState(new PlayerRope(this));
	m_pPlayerFSM->AddState(new PlayerPlatform(this));

	m_pPlayerFSM->ChangeState(PLAYER_VOID);
}

Player::~Player()
{
	delete m_pPlayerFSM;
}

void Player::update(float dt)
{
	m_pPlayerFSM->update(dt);
}

void Player::CheckCollison(GameLayer* pLayer)
{
	m_pPlayerFSM->CheckCollison(pLayer);
}

void Player::CheckCollisonWithItem(GameLayer* pLayer)
{
	CCArray* pArray = pLayer->_itemArray;

	CCObject* it = NULL;
	CCARRAY_FOREACH(pArray, it)
	{
		CCSprite *pItem = dynamic_cast<CCSprite*>(it);

		if (pItem != NULL)
		{
			CCRect rect = boundingBox();
			if (rect.containsPoint(pItem->getPosition()))
			{
				CCSprite * bonus = CCSprite::create("100.png");
				CCPoint move = pItem->getPosition();
				move.y = move.y + 20;
				CCActionInterval* myAction = CCMoveTo::create(2, move);

				bonus->setPosition(pItem->getPosition());

				pLayer->_gameMap->addChild(bonus, 20);
				//bonus->runAction(myAction, callfuncN_selector(Player::ItemCallback));
				CCAction * hide = CCSequence::create(myAction,
					CCCallFuncN::create(this, callfuncN_selector(Player::ItemCallback)), NULL);
				bonus->runAction(hide);

				pLayer->_gameMap->removeChild(pItem);
				pArray->removeObject(pItem);

				
			}

		}
	}
}

void Player::ResetPhysicsParam()
{
	forwardMarch = 0;
	mightAsWellJump = false;
	jumpingForforwardMarch = 0;
}

void Player::ItemCallback(CCNode * node)
{
	m_pLayer->_gameMap->removeChild((CCSprite*)node);
}