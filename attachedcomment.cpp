#include "attachedcomment.h"
#include <assert.h>

void AttachedComment::paint(QPainter *p)
{
 //!< Do not implement, this class cannot be drawn
 assert("Do not draw attached comment!" && false);
}

QRectF AttachedComment::boundingRect() const
{
    return QRectF();
}

void AttachedComment::save(QDomDocument * /* doc */,
               QDomElement *  /* element */)
{
    //!< TODO: Implement this later
}

void AttachedComment::load(QDomElement * /* element */,
               QMap<void *, Serializable *> & /* addressMap */ )
{
    //!< TODO: Implement this later
}

void AttachedComment::resolvePointers(QMap<void *, Serializable *> &
                          /* adressMap */)
{
    //!< TODO: Implement this later
}