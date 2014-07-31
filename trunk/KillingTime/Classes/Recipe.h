#ifndef RECIPE_H
#define RECIPE_H
#include "cocos2d.h"
USING_NS_CC;
#include "GameHelper.h"

//Shared enums
enum {
	TAG_EFFECT_NODE = 0
};

//Recipe Interface
class Recipe: public CCLayer {
	
public:
	CCLabelBMFont *message;

	CCLayer* runRecipe();
void cleanRecipe();
void resetMessage();
void showMessage(CCString* m);
void appendMessage(CCString* m);
void appendMessageWithWrap(CCString* m);
void drawLayer();
};






#endif //RECIPE_H