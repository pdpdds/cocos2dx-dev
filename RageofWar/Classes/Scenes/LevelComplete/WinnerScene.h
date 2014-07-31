#ifndef _WINNER_SCENE_H_
 #define _WINNER_SCENE_H_
 
 #include "cocos2d.h" 

 class WinnerLayer : public cocos2d::CCLayerColor
 {
 public:
     WinnerLayer():_label(NULL) {};
    virtual ~WinnerLayer();
    bool init();
    NODE_FUNC(WinnerLayer);
	void ExplosionAnimation(cocos2d::CCPoint position,const char* imgName );
    void WinnerDone();

    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);
};

class WinnerScene : public cocos2d::CCScene
{
public:
    WinnerScene():_layer(NULL) {};
    ~WinnerScene();
    bool init();
    NODE_FUNC(WinnerScene);

    CC_SYNTHESIZE_READONLY(WinnerLayer*, _layer, Layer);
};

#endif // _GAME_OVER_SCENE_H_