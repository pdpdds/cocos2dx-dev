/*///////////////////////////////////////////////////////////////////////////////////////////////////////////

//Source http://code.google.com//p//igk-game-2012//source//browse//igk-game//Classes//Input.h?r=bf1af807264551dd77bf885e12d8444ae397c65f
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>

#include <stdlib.h>

class Input
{
public:
        Input()                         {}
        virtual ~Input()        {}

        static Input* instance();

        void init(HINSTANCE appInstance, bool isExclusive = true);
        void release();

        bool keyDown(int key)    { return  keys[key];  }
        bool keyUp(int key)      { return (!keys[key] && tempKeys[key]); }
		bool keyLeft(int key)    { return  keys[key];  }
        bool keyRight(int key)      { return (!keys[key] && tempKeys[key]); }
        bool keyPressed(int key) { return (keys[key]  && tempKeys[key]); }

        char getChar() 
        {
                for (int i = ' '; i < '~'; i++) 
                {
                        if (keyPressed(i))
                                return (char)i;
                }
                return '\0';
        }

        void update();

private:
        bool keys[256];
        bool tempKeys[256];
};