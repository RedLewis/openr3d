#ifndef PHYSICS2D_H
#define PHYSICS2D_H

#include <Box2D/Box2D.h>
#define BOX2DSCALE 10.f

//TODO: Store scale un Physics2D and ContactListener
//TODO: Have a Collision2D instead of a general collision

class Scene;

class Physics2D
{
public:

    class ContactListener : public b2ContactListener
    {
    public:
        ContactListener();

        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
    };

    b2World world;
    ContactListener contactListener;

    Physics2D();
    void update(float deltaTime);
};

#endif // PHYSICS2D_H
