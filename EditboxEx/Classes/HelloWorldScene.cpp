#include "HelloWorldScene.h"

using namespace cocos2d;
using namespace cocos2d::extension;


CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    
    HelloWorld *layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) )
    {
        return false;
    }

    /////////////////////////////

/*
    CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
    CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();

    
    m_pTTFShowEditReturn = CCLabelTTF::create("No edit control return!", "", 30);
    m_pTTFShowEditReturn->setColor(ccc3(0, 0, 0));
    m_pTTFShowEditReturn->setPosition(ccp(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y + visibleSize.height*4/5+20));
    addChild(m_pTTFShowEditReturn);

    
    CCSize editBoxSize = CCSizeMake(visibleSize.width - 100, 60);
    
    // top
    m_pEditName = CCEditBox::create(editBoxSize, CCScale9Sprite::create("extensions/green_edit.png"));
    m_pEditName->setPosition(ccp(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height*3/5+20));
    m_pEditName->setFontColor(ccRED);
    m_pEditName->setPlaceHolder("Name:");
    m_pEditName->setMaxLength(8);
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
    m_pEditName->setDelegate(this);
    addChild(m_pEditName);
    
    // middle
    m_pEditPassword = CCEditBox::create(editBoxSize, CCScale9Sprite::create("extensions/orange_edit.png"));
    m_pEditPassword->setPosition(ccp(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height*2/5+20));
    m_pEditPassword->setFontColor(ccGREEN);
    m_pEditPassword->setPlaceHolder("Password:");
    m_pEditPassword->setMaxLength(6);
    m_pEditPassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPassword->setInputMode(kEditBoxInputModeSingleLine);
    m_pEditPassword->setDelegate(this);
    addChild(m_pEditPassword);
    
    // bottom
    m_pEditEmail = CCEditBox::create(CCSizeMake(editBoxSize.width, editBoxSize.height), CCScale9Sprite::create("extensions/yellow_edit.png"));
    m_pEditEmail->setPosition(ccp(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height*1/5+20));
    m_pEditEmail->setPlaceHolder("Email:");
    m_pEditEmail->setInputMode(kEditBoxInputModeEmailAddr);
    m_pEditEmail->setDelegate(this);
    addChild(m_pEditEmail);
*/

    CCSize editBoxSize = CCSizeMake(300, 60);
    
    // top
    m_pEditName = CCEditBox::create(editBoxSize, CCScale9Sprite::create("extensions/orange_edit.png"));
    m_pEditName->setPosition(ccp(240, 250));
    m_pEditName->setFontColor(ccGREEN);
    m_pEditName->setPlaceHolder("Name:");
    m_pEditName->setMaxLength(8);
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
    m_pEditName->setDelegate(this);
    addChild(m_pEditName);
    
    // middle
    m_pEditPassword = CCEditBox::create(editBoxSize, CCScale9Sprite::create("extensions/green_edit.png"));
    m_pEditPassword->setPosition(ccp(240, 150));
    m_pEditPassword->setFontColor(ccRED);
    m_pEditPassword->setPlaceHolder("Password:");
    m_pEditPassword->setMaxLength(6);
    m_pEditPassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPassword->setInputMode(kEditBoxInputModeSingleLine);
    m_pEditPassword->setDelegate(this);
    addChild(m_pEditPassword);
    
    // bottom
    m_pEditEmail = CCEditBox::create(CCSizeMake(editBoxSize.width, editBoxSize.height), CCScale9Sprite::create("extensions/yellow_edit.png"));
    m_pEditEmail->setPosition(ccp(240, 50));
    m_pEditEmail->setPlaceHolder("Email:");
    m_pEditEmail->setInputMode(kEditBoxInputModeEmailAddr);
    m_pEditEmail->setDelegate(this);
    addChild(m_pEditEmail);

    return true;
}

void HelloWorld::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void HelloWorld::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void HelloWorld::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void HelloWorld::editBoxReturn(CCEditBox* editBox)
{
    CCLog("editBox %p was returned !");
    
//    if (m_pEditName == editBox)
//    {
//        m_pTTFShowEditReturn->setString("Name EditBox return !");
//    }
//    else if (m_pEditPassword == editBox)
//    {
//        m_pTTFShowEditReturn->setString("Password EditBox return !");
//    }
//    else if (m_pEditEmail == editBox)
//    {
//        m_pTTFShowEditReturn->setString("Email EditBox return !");
//    }
}






