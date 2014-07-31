#include "DebugDraw.h"

using namespace cocos2d;

DebugDraw* DebugDraw::create()
{
    DebugDraw* draw = new DebugDraw();
    draw->autorelease();
    return draw;
}

DebugDraw::DebugDraw()
{
    m_lines = new std::vector<DebugLine>();
    m_points = new std::vector<DebugPoint>();
}

DebugDraw::~DebugDraw()
{
    delete m_lines;
    delete m_points;
}

void DebugDraw::draw(void)
{
    int c = m_lines->size();
    for (int i = 0; i < c; i++)
    {
        DebugLine line = m_lines->at(i);
        glColor4f(line.r, line.g, line.b, 1);
        ccDrawLine(line.pt1, line.pt2);
    }

    c = m_points->size();
    for (int i = 0; i < c; i++)
    {
        DebugPoint pt = m_points->at(i);
        glColor4f(pt.r, pt.g, pt.b, 1);
        ccDrawPoint(pt.pt);
    }
}

void DebugDraw::appendLine(CCPoint pt1, CCPoint pt2, float r, float g, float b)
{
    DebugLine line;
    line.pt1 = pt1;
    line.pt2 = pt2;
    line.r = r;
    line.g = g;
    line.b = b;
    m_lines->push_back(line);
}

void DebugDraw::appendPoint(float x, float y, float r, float g, float b)
{
    appendPoint(ccp(x, y), r, g, b);
}

void DebugDraw::appendPoint(CCPoint pt, float r, float g, float b)
{
    DebugPoint dp;
    dp.pt = pt;
    dp.r = r;
    dp.g = g;
    dp.b = b;
    m_points->push_back(dp);
}