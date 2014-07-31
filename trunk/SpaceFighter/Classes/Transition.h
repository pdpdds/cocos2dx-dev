#ifndef _TRANSITION_H_
#define _TRANSITION_H_

#define TRANSITION_DURATION (1.2f)

class FadeWhiteTransition : public CCTransitionFade 
{
public:
    static CCTransitionScene* create(float t, CCScene* s)
    {
        return CCTransitionFade::create(t, s, ccBLACK); 
    }
};

#endif 
