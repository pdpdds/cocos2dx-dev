#ifndef	__DEBUGDRAW_H__
#define	__DEBUGDRAW_H__

#include "cocos2d.h" 

typedef struct
{
    cocos2d::CCPoint pt1;
    cocos2d::CCPoint pt2;
    float r;
    float g;
    float b;
} DebugLine;

typedef struct
{
    cocos2d::CCPoint pt;
    float r;
    float g;
    float b;
} DebugPoint;

class DebugDraw : public cocos2d::CCNode
{
public:
    static DebugDraw* create();

    DebugDraw();
    ~DebugDraw();
    virtual void draw(void);

    void appendLine(cocos2d::CCPoint pt1, cocos2d::CCPoint pt2, float r = 1, float g = 1, float b = 1);
    void appendPoint(float x, float y, float r = 1, float g = 1, float b = 1);
    void appendPoint(cocos2d::CCPoint pt, float r = 1, float g = 1, float b = 1);

private:
    std::vector<DebugLine>* m_lines;
    std::vector<DebugPoint>* m_points;
};

#endif	// __DEBUGDRAW_H__