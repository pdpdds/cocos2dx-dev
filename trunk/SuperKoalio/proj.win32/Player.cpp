#include "Player.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

Player::Player(char* filename)
{
	CCSprite::initWithFile("koalio_stand.png");
	velocity = ccp(0.0, 0.0);
	onGround = false;
	forwardMarch = false;
	mightAsWellJump = false;
}


Player::~Player()
{
	
}

CCRect Player::collisionBoundingBox()
{
	
	//CCRect collisionBox = CCRectInset(this->boundingBox, 3, 0);
	//collisionBox = CGRectOffset(collisionBox, 0, -2);
	CCPoint diff = ccpSub(desiredPosition, this->getPosition());
	//CCRect returnBoundingBox = CGRectOffset(collisionBox, diff.x, diff.y);
	return this->boundingBox();

}

void Player::update(float dt)
{
	CCPoint gravity = ccp(0.0, -450.0);
	CCPoint gravityStep = ccpMult(gravity, dt);

	
	CCPoint jumpForce = ccp(0.0, 310.0);
	float jumpCutoff = 150.0;

	if (mightAsWellJump && onGround) {
		velocity = ccpAdd(velocity, jumpForce);
		SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");
	}
	else if (!mightAsWellJump && velocity.y > jumpCutoff) {
		velocity = ccp(velocity.x, jumpCutoff);
	}

	
	CCPoint forwardMove = ccp(800.0, 0.0);
	CCPoint forwardStep = ccpMult(forwardMove, dt);

	velocity = ccp(velocity.x * 0.90, velocity.y); //2

	if (forwardMarch) {
		velocity = ccpAdd(velocity, forwardStep);
	} //3

	CCPoint minMovement = ccp(0.0, -450.0);
	CCPoint maxMovement = ccp(120.0, 250.0);
	velocity = ccpClamp(velocity, minMovement, maxMovement);

	velocity = ccpAdd(velocity, gravityStep);
	CCPoint stepVelocity = ccpMult(velocity, dt);

	desiredPosition = ccpAdd(this->getPosition(), stepVelocity);
}