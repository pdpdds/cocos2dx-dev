//
//  CollisionListener.h
//  MiniPool
//
//  Created by Roger Engelbert on 1/14/13.
//
//

#ifndef __MiniPool__CollisionListener__
#define __MiniPool__CollisionListener__

#include "Box2D/Box2D.h"
#include "Ball.h"
#include "Cue.h"


class CollisionListener : public b2ContactListener
{
    public :
    virtual void BeginContact(b2Contact* contact);
    //virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    //virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
};


#endif /* defined(__MiniPool__CollisionListener__) */
