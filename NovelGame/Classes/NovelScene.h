#ifndef __NovelGame__NovelScene__
#define __NovelGame__NovelScene__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class NovelScene : public cocos2d::CCLayer
{

    bool isMenuSelect;   
    bool isShowTextLog;    
    unsigned char* m_novelJsonFile;    
    int m_textIndex;
    
    //다음 텍스트를 처리
    void nextNovelJson();
    
    //선택지 버튼의 생성
    void makeSelectSpriteButton(string str1, int next1Id, string str2, int next2Id);
    //선택지를 선택할시의 콜백
    void menuSelectCallback(CCObject *pSender);
    
    //텍스트를 표시한다
    void dispText(string text);
    //이름을 표시한다
    void dispName(string name);
    
    void makeActorImage(const char* imageFilePath, int dict);
    void removeActorImage(int dict);
    
    CCFiniteTimeAction* changeBackgroundAnimation(string imgFilePath);
    void changeBackground(CCNode *pSender, void* node);
    
    void logMenuSelectCallback(CCObject *pSender);
    void showTextLog(int showTextIndex);
    void hideTextLog();
protected:
    
    enum kTag
    {
        kTag_Background = 1,
        kTag_TextLayer = 100,
        kTag_TextLayer_textLabel = 101,
        kTag_TextLayer_name = 200,
        kTag_TextLayer_nameTextLabel = 201,
        
        kTag_ActorDict     = 1000,
        kTag_ActorDictLeft,
        kTag_ActorDictCenter,
        kTag_ActorDictRight,
                
        kTag_MenuSelect   = 10000,
        kTag_MenuSelect1,
        kTag_MenuSelect2,
               
        kTag_TextLogLayer = 20000,
               
        kTag_MenuItem_log = 30000,
    };
    
    enum kZOrder
    {
        kZOrder_Background = 1,
        kZOrder_Actor,
        kZOrder_TextLayer,
        kZOrder_MenuSelect,
        kZOrder_TextLogLayer,
        kZOrder_MenuItem,
    };
    
    enum kNovelType
    {
        kTextType = 1,
        kSelectItem = 2,
        kActorImageShow = 3,
        kActorImageHide = 4,
        kBackgroundShow = 5,
    };
    
public:
    NovelScene();
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(NovelScene);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__NovelGame__NovelScene__) */
