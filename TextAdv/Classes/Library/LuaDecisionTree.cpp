#include "LuaDecisionTree.h"

//Implementation

CCLayer* LuaDecisionTree::runRecipe() {
	//Superclass initialization
	

	if ( !CCLayer::init() )
    {
        return NULL;
    }

	Recipe::runRecipe();

	//Initial variables
	m_response = 0;
	text = CCString::create("");
	bufferString = NULL;
	timeElapsed = 0;
	dialogForward = false;
	wrapNext = false;
	optionsOnScreen = 0;
	canMoveForward = false;
	gunsDown = false;
	
	//Add sprite plist file
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("big_louie.plist");
	
	//Add sprite frames
	this->addSpriteFrame(CCString::create("bl_bg.png") , 0 , TAG_BL_BG , true);
	this->addSpriteFrame(CCString::create("bl_door.png") , 1 , TAG_BL_DOOR , true);
	this->addSpriteFrame(CCString::create("bl_officer.png") , 2 , TAG_BL_OFFICER , false);
	this->addSpriteFrame(CCString::create("bl_louie.png") , 3 , TAG_BL_LOUIE , true);
	this->addSpriteFrame(CCString::create("bl_guy_1.png") , 3 , TAG_BL_GUY_1 , false);
	this->addSpriteFrame(CCString::create("bl_guy_2.png") , 2 , TAG_BL_GUY_2 , false);
	this->addSpriteFrame(CCString::create("bl_your_gun.png") , 4 , TAG_BL_YOUR_GUN , false);
	this->addSpriteFile(CCString::create("bl_gun_pointed_at_you.png") , 4 , TAG_BL_GUN_POINTED_AT_YOU , false);
	
		
	//Options Node
	optionsNode = CCNode::create();
	optionsNode->init();
	optionsNode->setPosition(ccp(0,0));
	optionsNode->setVisible(false);
	this->addChild(optionsNode , 6);
			
	//Text label
	textLabel = CCLabelTTF::create("THE HULK", "eurostile_30.fnt", 34);
	
	textLabel->setPosition(ccp(10,115));
	textLabel->setScale(0.65f);
	textLabel->setColor(ccc3(255,255,255));
	textLabel->setAnchorPoint(ccp(0,1));
	this->addChild(textLabel , 6);

	//Dialog background image
	CCSprite *dialog_bg = CCSprite::create("dialog_bg.png");
	dialog_bg->setPosition(ccp(240,61.5f));
	this->addChild(dialog_bg , 5);

	//Set callback pointer
	ldtRecipe = this;

	//Lua initialization
	lua_ = new mcLuaManager;
	
	//Lua function wrapper library
	static const luaL_reg scriptLib[] = 
	{
		{"desc", ldtDesc },	
		{"anim", ldtAnim },
		{"dialog", ldtDialog },
		{"logic", ldtLogic },
		{"dialogOption", ldtDialogOption },
		{"actionOption", ldtActionOption },	
		{"getResponse", ldtGetResponse },
		{"presentOptions", ldtPresentOptions },
		{NULL, NULL}
	};
	lua_->LuaOpenLibrary("scene",scriptLib);

	//Open Lua script
	m_sc = lua_->CreateScript();
	CCString *filePath = CCString::create("decision_tree.lua"); 
	m_sc->LoadFile(filePath->getCString());

	//Set initial update method counter
	lua_->Update(0);
	
	//Schedule step method
	this->schedule( schedule_selector(LuaDecisionTree::step));

	this->setTouchEnabled(true);

	return this;
}

/* Add sprite frame helper method */
void LuaDecisionTree::addSpriteFrame(CCString* name, int z , int tag , bool v ) {
	CCSprite *sprite = CCSprite::createWithSpriteFrameName(name->getCString());
	sprite->setPosition(ccp(240,160));
	sprite->setVisible(v);
	this->addChild(sprite , z , tag);
}

/* Add sprite file helper method */
void LuaDecisionTree::addSpriteFile(CCString* name, int z , int tag , bool v ) {
	CCSprite *sprite = CCSprite::create(name->getCString());
	sprite->setPosition(ccp(240,160));
	sprite->setVisible(v);
	this->addChild(sprite , z , tag);
}

void LuaDecisionTree::cleanRecipe() {
	free(lua_);
	lua_ = NULL;
	
	text->release();
	text = NULL;
	
	bufferString->release();
	bufferString = NULL;
	
	//this->cleanRecipe();
}

void LuaDecisionTree::step(float delta) {
	//Update Lua script runner
	lua_->Update(delta);
	
	//Increment elapsed time
	timeElapsed += delta;
		
	//If enough time has passed and there is still data in the buffer, we show another character
	if(bufferString != NULL)
	textLabel->setString(bufferString->getCString());
	else
		textLabel->setString("");
	canMoveForward = true;
	if(bufferString && timeElapsed > 0.002f && bufferString->length() > 0){
		//Reset elapsed time
		timeElapsed = 0.0f;

	
		//Add next character
		/*CCString *nextChar = [bufferString->substringToIndex(1) retain];	
		text = [NSString->stringWithFormat("%@%@", text, nextChar) retain];
	
		//Text wrapping
		if(text->length() > 0 && text->length()%40 == 0){
			wrapNext = true;
		}
		
		//Wrap at next word break
		if(wrapNext && [text->substringFromIndex(text.length-1) , " "]){
			text = [NSString->stringWithFormat("%@\n", text) retain];
			wrapNext = false;
		}
	
		//Chop off last letter in buffer
		if(bufferString->length() > 1){
			bufferString = [bufferString->substringFromIndex(1) retain];
		}else{
			bufferString = "";
		}
		
		//Finally, set text label
		textLabel->setString(text);*/
	}else{
		//Move dialog forward as necessary
		if(!bufferString || bufferString->length() == 0){
			if(dialogForward){
				dialogForward = false;
				bufferString->release();
				bufferString = NULL;
			}else{
				canMoveForward = true;
			}
		}
	}
}

/* Show text callback */
void LuaDecisionTree::showText(CCString* str) {
	//text = "";
	str->retain();
	bufferString = str;
}

/* Description callback */
void LuaDecisionTree::desc(CCString* str) {
	this->showText(str);
}

/* Animation callback */
void LuaDecisionTree::anim(CCString* str) {
	if(str->compare("Open door")){
		this->getChildByTag(TAG_BL_DOOR)->setVisible(false);
	}else if(str->compare("Enter officer")){
		this->getChildByTag(TAG_BL_OFFICER)->setVisible(true);
	}else if(str->compare("Pull guns")){
		this->getChildByTag(TAG_BL_YOUR_GUN)->setVisible(true);
		this->getChildByTag(TAG_BL_GUY_1)->setVisible(true);
		this->getChildByTag(TAG_BL_GUY_2)->setVisible(true);
	}else if(str->compare("Louie looks away")){
		CCSprite *sprite = (CCSprite*)this->getChildByTag(TAG_BL_LOUIE);		
		sprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bl_louie_look_away.png"));		
	}else if(str->compare("Louie looks at you")){
		CCSprite *sprite = (CCSprite*)this->getChildByTag(TAG_BL_LOUIE);		
		sprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bl_louie.png"));		
	}else if(str->compare("Louie scowls")){
		CCSprite *sprite = (CCSprite*)this->getChildByTag(TAG_BL_LOUIE);		
		sprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bl_louie_angry.png"));		
	}else if(str->compare("Officer shocked")){
		CCSprite *sprite = (CCSprite*)this->getChildByTag(TAG_BL_OFFICER);
		sprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bl_guy_1_no_gun.png"));		
	}else if(str->compare("Put guns down")){
		CCSprite *sprite1 = (CCSprite*)this->getChildByTag(TAG_BL_GUY_1);		
		sprite1->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bl_guy_1_no_gun.png"));
		CCSprite *sprite2 = (CCSprite*)this->getChildByTag(TAG_BL_GUY_2);
		sprite2->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bl_guy_2_no_gun.png"));
	}else if(str->compare("Pull gun on Louie")){
		CCSprite *sprite = (CCSprite*)this->getChildByTag(TAG_BL_YOUR_GUN);
		sprite->setPosition(ccp(140,160));
	}else if(str->compare("Pull gun on men")){
		CCSprite *sprite = (CCSprite*)this->getChildByTag(TAG_BL_YOUR_GUN);
		sprite->setFlipX(true);
		sprite->setPosition(ccp(340,160));
	}else if(str->compare("Gun pointed at you")){
		CCSprite *sprite = (CCSprite*)this->getChildByTag(TAG_BL_GUN_POINTED_AT_YOU);
		sprite->setVisible(true);
	}
}

/* Dialog callback */
void LuaDecisionTree::dialog(CCString* str) {
	this->showText(str);
}

/* Logic callback */
int LuaDecisionTree::logic(CCString* str) {
	int num = 0;
		
	if(str->compare("Put guns down")){
		gunsDown = true;
	}else if(str->compare("Are guns down?")){
		if(gunsDown){
			num = 1;
		}else{
			num = 0;
		}
	}else if(str->compare("You win")){
		this->showMessage(CCString::create("You WIN!!"));
	}
	
	return num;
}

/* Dialog option callback */
void LuaDecisionTree::dialogOption(CCString* str) {
	CCMenuItemFont::setFontName("Arial");
	CCMenuItemFont::setFontSize(16);
	
	optionsOnScreen += 1;
	
	//Add dialog option to screen
	//CCString *dialogStr = CCString::stringWithFormat("\"%@\"", str);
	CCString *dialogStr = CCString::createWithFormat("%s", str);
	CCMenuItemFont *optionItem = CCMenuItemFont::create(str->getCString() , this , menu_selector(LuaDecisionTree::selectOption));
							     
	optionItem->setTag(optionsOnScreen);
	optionItem->setPosition(ccp(10,135 - optionsOnScreen*20));
	optionItem->setAnchorPoint(ccp(0,1));
	
	CCMenu *menu = CCMenu::createWithItem(optionItem);
	menu->setPosition(ccp(0,0));
	optionsNode->addChild(menu);
}

/* Action option callback */
void LuaDecisionTree::actionOption(CCString* str) {
	CCMenuItemFont::setFontName("Arial");
	CCMenuItemFont::setFontSize(16);
	
	optionsOnScreen += 1;
	
	//Add action option to screen
	CCString *optionStr = CCString::createWithFormat("[%s)", str);
	CCMenuItemFont *optionItem = CCMenuItemFont::create(str->getCString() , this , menu_selector(LuaDecisionTree::selectOption));
	optionItem->setTag(optionsOnScreen);
	optionItem->setPosition(ccp(10,135 - optionsOnScreen*20));
	optionItem->setAnchorPoint(ccp(0,1));
	
	CCMenu *menu = CCMenu::createWithItem(optionItem);
	menu->setPosition(ccp(0,0));
	optionsNode->addChild(menu);
}

/* Present options callback */
void LuaDecisionTree::presentOptions() {
	//text = "";
	textLabel->setString("");
	optionsNode->setVisible(true);
}

/* Select option callback */
void LuaDecisionTree::selectOption(CCObject* pSender) {
	CCMenuItemFont *item = (CCMenuItemFont*)pSender;
	m_response = item->getTag();
	
	//Remove all children
/*	for(CCNode *n in optionsNode->children()){
		optionsNode->removeChild(n , true);
	}*/
	this->removeChild(optionsNode , true);
	
	//Re-add optionsNode
	optionsNode = CCNode::create();
	optionsNode->setPosition(ccp(0,0));
	optionsNode->setVisible(false);
	this->addChild(optionsNode , 6);
	
	optionsOnScreen = 0;
	
	//Resume the script
	m_sc->YieldResume();
}
void LuaDecisionTree::ccTouchesBegan(CCSet * pTouches,  CCEvent * event ) 
{
	
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	

	CCPoint location = touch->getLocationInView();
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);

	
	if(convertedLocation.y < 123.0f && canMoveForward){
		bufferString = CCString::create("");
		dialogForward = true;
		wrapNext = false;
		m_sc->YieldResume();
		canMoveForward = false;
	}
}
