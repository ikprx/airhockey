#include "Physics.h"

bool Physics::collisionDetection(Model * m1, Model * m2)
{
	bool collisionXMIN = (m1->transform.position.x + (m1->boundariesMAX.x * m1->transform.scale.x)) >= (m2->boundariesMIN.x * m2->transform.scale.y)  + m2->transform.position.x;
	bool collisionXMAX = (m1->transform.position.x + (m1->boundariesMIN.x * m1->transform.scale.x)) <= (m2->boundariesMAX.x * m2->transform.scale.y)  + m2->transform.position.x;
	
	bool collisionYMIN = (m1->transform.position.y + (m1->boundariesMAX.y * m1->transform.scale.y)) >= (m2->transform.position.y + (m2->boundariesMIN.y * m2->transform.scale.y));
	bool collisionYMAX = (m1->transform.position.y + (m1->boundariesMIN.y * m1->transform.scale.y)) <= (m2->boundariesMAX.y * m2->transform.scale.y)  + m2->transform.position.y;
        return collisionXMIN && collisionXMAX && collisionYMIN && collisionYMAX;
}