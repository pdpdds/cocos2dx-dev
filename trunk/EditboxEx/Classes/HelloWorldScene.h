#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"


class HelloWorld : public cocos2d::CCLayerColor, public cocos2d::extension::CCEditBoxDelegate
{
public:
    virtual bool init();

    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(HelloWorld);

    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);

private:
//    cocos2d::CCLabelTTF* m_pTTFShowEditReturn;
    cocos2d::extension::CCEditBox* m_pEditName;
    cocos2d::extension::CCEditBox* m_pEditPassword;
    cocos2d::extension::CCEditBox* m_pEditEmail;
};

#endif // __HELLOWORLD_SCENE_H__
