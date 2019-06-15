#include "Recipe.h"

//Recipe Implementation


CCLayer* Recipe::runRecipe() {

	//Enable touches
	//this->setTouchEnabled(true);

	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	//Message
	message = CCLabelBMFont::create();
	message->initWithString("" , "eurostile_30.fnt");
	message->setPosition(ccp(10,screenSize.height-35));
	message->setAnchorPoint(ccp(0,1));
	message->setScale(0.5f);
	message->setColor(ccc3(255,255,255));
	this->addChild(message , 10);

	//Remove message after 5 seconds
	this->runAction(CCSequence::create(CCDelayTime::create(5.0f), CCCallFunc::create(this, callfunc_selector(Recipe::resetMessage)), NULL));
	
	return NULL;
}
void Recipe::cleanRecipe() {
	this->removeAllChildrenWithCleanup(true);
}
/* Reset message callback */
void Recipe::resetMessage() {
	message->setString("");
}
void Recipe::showMessage(CCString* m) {
	/*this->stopAllActions();
	CCLOG("%@",m);
	m = GameHelper::sanitizeString(m);
	message->setString(m);
	this->runAction([CCSequence , [CCDelayTime , 500.0f), 
		CCCallFunc->actionWithTarget(this , @selector(resetMessage)), nil]];*/
}
void Recipe::appendMessage(CCString* m) {
	this->stopAllActions();
	CCLOG("%@",m);
	//m = GameHelper::sanitizeString(m);
	message->setString(m->getCString());
}
void Recipe::appendMessageWithWrap(CCString* m) {
	this->stopAllActions();
	
	//Check for bad characters
	//m = GameHelper::sanitizeString(m);
	
	/*CCS *str = m->mutableCopy();

	for(int i=0; i<str->length(); i+=40){
		while(i < str->length() && str->characterAtIndex(i) != 32){
			i++;
		}
		if(i >= str->length()){
			break;
		}
		str->insertString("\n" , i);
	}*/
	
	message->setString(m->getCString());
}
void Recipe::drawLayer() {
	//ABSTRACT
}


