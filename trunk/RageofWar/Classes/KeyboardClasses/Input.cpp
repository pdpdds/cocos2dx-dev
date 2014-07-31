
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////

//Source http://code.google.com/p/igk-game-2012/source/browse/igk-game/Classes/Input.cpp?r=bf1af807264551dd77bf885e12d8444ae397c65f
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
#include "Input.h"

#define check(a) a

Input* Input::instance() {
        static Input* sharedInstance;
        if(!sharedInstance) {
                sharedInstance = new Input();
        }
        return sharedInstance;
}

void Input::init(HINSTANCE appInstance, bool isExclusive)
{
        // wyczyszczenie stanu klawiszy
        for(int i = 0; i < 256; i++) {
                keys[i] = tempKeys[i] = false;
        }
}

void Input::update()
{
        // update klawiszy
        for (int i = 0; i < 256; i++) 
        {
                bool state  = static_cast<bool>(GetAsyncKeyState(i));
                tempKeys[i] = (state != keys[i]);
                keys[i]     = state;
        }
}
