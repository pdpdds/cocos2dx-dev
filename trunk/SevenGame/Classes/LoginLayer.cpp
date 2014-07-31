#include "LoginLayer.h"
#include "cocos-ext.h"
#include "SFProtobufPacket.h"
#include "SFPacketStore.pb.h"
#include "SevenGamePacketID.h"
#include "SFPacketID.h"
#include "NetworkFacade.h"

class KeyboardNotificationLayer;

using namespace cocos2d;
using namespace extension;
using namespace std;

#include "NetworkFacade.h"
USING_NS_CC;

CCScene* LoginLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	LoginLayer *layer = LoginLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LoginLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init())
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

	this->schedule(schedule_selector(LoginLayer::update));

	winSize = CCDirector::sharedDirector()->getWinSize();   //화면 사이즈
	this->setTouchEnabled(true);    //터치 가능(cocos2d-x)


	CCSize editBoxSize = CCSizeMake(visibleSize.width - winSize.width / 2, 60);


	//editbox
	{
		//아이디
		m_pEditName = CCEditBox::create(editBoxSize, CCScale9Sprite::create("orange_edit.jpg"));
		m_pEditName->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2 - 30, visibleOrigin.y + visibleSize.height / 2 + 50));
		m_pEditName->setFontName("Arial");
		m_pEditName->setFontSize(40);
		m_pEditName->setFontColor(ccGREEN);
		m_pEditName->setPlaceHolder("아이디:");
		m_pEditName->setPlaceholderFontColor(ccGREEN);
		m_pEditName->setMaxLength(8);
		m_pEditName->setReturnType(kKeyboardReturnTypeDone);
		//        m_pEditPassword->setInputFlag(kEditBoxInputFlagSensitive);
		//        m_pEditPassword->setInputMode(kEditBoxInputModeAny);
		m_pEditName->setDelegate(this);

		addChild(m_pEditName, 2);

		//비밀번호
		// middle
		m_pEditPassword = CCEditBox::create(editBoxSize, CCScale9Sprite::create("orange_edit.jpg"));
		m_pEditPassword->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2 - 30, visibleOrigin.y + visibleSize.height / 2 - 50));
		m_pEditPassword->setFont("Arial", 40);
		m_pEditPassword->setFontColor(ccGREEN);
		m_pEditPassword->setPlaceholderFontColor(ccGREEN);
		m_pEditPassword->setPlaceHolder("Password:");
		m_pEditPassword->setMaxLength(13);
		m_pEditPassword->setInputFlag(kEditBoxInputFlagPassword);
		m_pEditPassword->setInputMode(kEditBoxInputModeSingleLine);
		m_pEditPassword->setDelegate(this);
		addChild(m_pEditPassword);
	}

	//로그인 버튼 이미지
	{
		CCSprite *login_btn = CCSprite::create("login_btn.jpg");
		login_btn->setPosition(ccp(0, winSize.height / 2));
		login_btn->setScale(winSize.width / 11 / login_btn->getContentSize().width);
		login_btn->setTag(001);
		this->addChild(login_btn, 3);
	}

	//회원가입 버튼
	/*{
		CCSprite *join_btn = CCSprite::create("Member_Join.jpg");
		join_btn->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 150));
		join_btn->setTag(002);
		this->addChild(join_btn, 3);
	}*/

	return true;
}

void LoginLayer::update(float dt)
{
	NetworkFacade::GetInstance()->Poll();
}

void LoginLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox) {
	//    CCLog("editBox %p DidBegin !", editBox);
}
void LoginLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox) {
	//    CCLog("editBox %p DidEnd !", editBox);
}

void LoginLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text) {
	//    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}
void LoginLayer::editBoxReturn(CCEditBox* editBox) {
	//    CCLog("editBox %p was returned !");

	if (m_pEditName == editBox)
	{
		//        m_pTTFShowEditReturn->setString("Name EditBox return !");
	}
	//    else if (m_pEditPassword == editBox)
	//    {
	//        m_pTTFShowEditReturn->setString("Password EditBox return !");
	//    }
}

void LoginLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CCTouch *pTouch = (CCTouch *)pTouches->anyObject();
	CCPoint point = pTouch->getLocationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);

	string a = "123";

	CCSprite *login_btn = (CCSprite*)this->getChildByTag(001);
	CCRect login_rect = login_btn->boundingBox();
	if (login_rect.containsPoint(point)) {
		//CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.2, select::scene(), ccc3(255, 255, 255)));
		int i = 1;

		SFProtobufPacket<SFPacketStore::Login> PktLogin(CGSF::Login);
		//	PktLogin.SetOwnerSerial(g_engine->GetLocalID());
		PktLogin.GetData().set_username("cgsf");
		PktLogin.GetData().set_password("1234");
		NetworkFacade::GetInstance()->TCPSend(&PktLogin);
	}

	cout << id_R << endl;
	cout << password_R << endl;
}

void LoginLayer::keyBackClicked() {
	CCDirector::sharedDirector()->end();
}
