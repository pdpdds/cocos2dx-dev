#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

class KeyboardNotificationLayer;

using namespace cocos2d;
using namespace extension;
using namespace std;

class LoginLayer : public cocos2d::CCLayer, public cocos2d::extension::CCEditBoxDelegate
{
public:
	
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(LoginLayer);
	void update(float dt);

	CCSize winSize;

	//멀티레이어를 위한 objc

	CCTextFieldTTF *ID_Field;

	std::string id_R;            //진짜 아이디가 담길 곳
	std::string password_R;      //진짜 비밀번호가 담길 곳

	//터치
	void ccTouchesBegan(cocos2d::CCSet* pTouch, cocos2d::CCEvent* pEvent);

	void keyBackClicked();  //종료키

	void Login_request(CCObject* pSender);
	void onHttpRequestCompleted(CCNode *sender, void *data);

	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);

private:
	cocos2d::CCLabelTTF* m_pTTFShowEditReturn;
	cocos2d::extension::CCEditBox* m_pEditName;
	cocos2d::extension::CCEditBox* m_pEditPassword;
};