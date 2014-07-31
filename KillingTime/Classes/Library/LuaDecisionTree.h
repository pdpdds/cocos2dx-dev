#ifndef CH7_LUADECISIONTREE_H
#define CH7_LUADECISIONTREE_H

#include "cocos2d.h"
USING_NS_CC;

#include "mcLua.h"
#include "Recipe.h"
//#include "ShadowLabel.h"

enum {
	TAG_BL_BG = 0,
	TAG_BL_DOOR = 1,
	TAG_BL_GUY_1 = 2,
	TAG_BL_GUY_2 = 3,
	TAG_BL_LOUIE = 4,
	TAG_BL_OFFICER = 5,
	TAG_BL_YOUR_GUN = 6,
	TAG_BL_GUN_POINTED_AT_YOU = 7
};

//Interface
class LuaDecisionTree : public Recipe
{
	friend class HelloWorld;
public:
	class mcLuaManager * lua_;
	 NODE_FUNC(LuaDecisionTree);
	 
	void cleanRecipe();
	virtual void ccTouchesBegan(CCSet * touches,  CCEvent * event ) override;

	CC_SYNTHESIZE(mcLuaScript*, m_sc, Sc)
    CC_SYNTHESIZE(int, m_response, Response)
	
	CCLabelTTF *textLabel;
	CCString *text;
	CCString *bufferString;
	float timeElapsed;
	bool dialogForward;
	bool canMoveForward;
	bool wrapNext;
	
	int optionsOnScreen;
	CCNode *optionsNode;
	
	bool gunsDown;

	CCLayer* runRecipe();
void addSpriteFrame(CCString* name, int z , int tag , bool v );
void addSpriteFile(CCString* name, int z , int tag , bool v );
void step(float delta);
void desc(CCString* str);
void anim(CCString* str);
void dialog(CCString* str);
int logic(CCString* str);
void dialogOption(CCString* str);
void actionOption(CCString* str);
void presentOptions();

void selectOption(CCObject* pSender);

void LuaDecisionTree::showText(CCString* str) ;
};

//Callback pointer
static LuaDecisionTree *ldtRecipe = NULL;

//Static C functions
static int ldtDesc(lua_State * l) {
	ldtRecipe->desc(CCString::create(lua_tostring(l,1)));
	ldtRecipe->getSc()->YieldPause();
	return (lua_yield(l, 0));
}
static int ldtAnim(lua_State * l) {
	ldtRecipe->anim(CCString::create(lua_tostring(l,1)));
	return 0;
}
static int ldtDialog(lua_State * l) {
	ldtRecipe->dialog(CCString::create(lua_tostring(l,1)));
	ldtRecipe->getSc()->YieldPause();
	return (lua_yield(l, 0));
}
static int ldtLogic(lua_State * l) {
	int num = ldtRecipe->logic(CCString::create(lua_tostring(l,1)));
	lua_pushnumber(l,num);
	return 1;
}
static int ldtDialogOption(lua_State * l) {
	ldtRecipe->dialogOption(CCString::create(lua_tostring(l,1)));
	return 0;
}
static int ldtActionOption(lua_State * l) {
	ldtRecipe->actionOption(CCString::create(lua_tostring(l,1)));
	return 0;
}
static int ldtGetResponse(lua_State * l) {
	lua_pushnumber(l,ldtRecipe->getResponse());	//Push a number onto the stack
	return 1;	//We are returning 1 result
}
static int ldtPresentOptions(lua_State * l) {
	ldtRecipe->presentOptions();
	ldtRecipe->getSc()->YieldPause();
	return (lua_yield(l, 0));
}






#endif //CH7_LUADECISIONTREE_H