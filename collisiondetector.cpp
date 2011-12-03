#include "collisiondetector.h"

CollisionDetector::CollisionDetector()
{
    m_map.insert(QPair<ShapeType,ShapeType>(ST_RECTANGLE,ST_RECTANGLE),&CollisionDetector::testRectangles);
    m_map.insert(QPair<ShapeType,ShapeType>(ST_RECTANGLE,ST_LINE     ),&CollisionDetector::testRectangleAndLine);
    m_map.insert(QPair<ShapeType,ShapeType>(ST_LINE     ,ST_LINE     ),&CollisionDetector::testLines);
    m_map.insert(QPair<ShapeType,ShapeType>(ST_COMPLEX  ,ST_RECTANGLE),&CollisionDetector::testComplexAndOther);
    m_map.insert(QPair<ShapeType,ShapeType>(ST_COMPLEX  ,ST_LINE     ),&CollisionDetector::testComplexAndOther);
    m_map.insert(QPair<ShapeType,ShapeType>(ST_COMPLEX  ,ST_COMPLEX  ),&CollisionDetector::testComplexAndOther);
}

bool CollisionDetector::testRectangles(CollisionObject * o1, CollisionObject *o2)
{
    return collides(o1->collisionRect(),o2->collisionRect());
}

bool CollisionDetector::testRectangleAndLine(CollisionObject *o1, CollisionObject *o2)
{
    return collides(o1->collisionRect(),o2->collisionLine());
}

bool CollisionDetector::testLines(CollisionObject *o1, CollisionObject *o2)
{
    return collides(o1->collisionLine(),o2->collisionLine());
}

bool CollisionDetector::testComplexAndOther(CollisionObject *o1, CollisionObject *o2)
{
   bool result=false;
   CollisionObject * cur=o1->firstObject();
   while(cur && !result)
   {
       result=result || test(cur,o2);
       cur=o1->nextObject();
   }
   return result;
}

bool CollisionDetector::test(CollisionObject * o1, CollisionObject * o2)
{
    QPair<ShapeType,ShapeType> me(o1->collisionShape(),o2->collisionShape());
    QPair<ShapeType,ShapeType> rev(o2->collisionShape(),o1->collisionShape());
    bool result=false;
    if (m_map.contains(me))
    {
        result=(this->*(m_map[me]))(o1,o2);
    }
    else
    {
        if (m_map.contains(rev))
        {
          result=(this->*(m_map[rev]))(o2,o1);
        }
    }
    return result;
}
