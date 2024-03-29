#include "moverange.h"
#include "compare.h"
#include <math.h>
MoveRange::MoveRange(qreal x, qreal y,qreal w, qreal h): QRectF(x,y,w,h)
{
}

MoveRange createHorizontalRange(qreal x1, qreal x2, qreal y)
{
    qreal min=x1, max=x2;
    if (x1>x2) { max=x1; min=x2; }
    return MoveRange(min,y,max-min,0);
}

MoveRange createVerticalRange(qreal y1, qreal y2, qreal x)
{
    qreal min=y1, max=y2;
    if (y1>y2) { max=y1; min=y2; }
    return MoveRange(x,min,0,max-min);
}




bool MoveRange::isWithin(const QPointF & p)
{
  if (  (p.x()>=left() || fuzzyCompare(p.x(),left()))
        && (p.x()<=right() || fuzzyCompare(p.x(),right()))
         && (p.y()>=top() || fuzzyCompare(p.y(),top()))
         && (p.y()<=bottom() || fuzzyCompare(p.y(),bottom()))
       )
      return true;
  if (fuzzyCompare(top(),bottom()) )
  {
    return fuzzyCompare(top(),p.y()) &&   left()<=p.x() && right()>=p.x();
  }
  if (fuzzyCompare(left(),right()) )
  {
    return fuzzyCompare(left(),p.x()) &&   top()<=p.y() && bottom()>=p.y();
  }
  return false;
}

void MoveRange::reduce(const MoveRange & range)
{
  if    (fuzzyCompare(top(),bottom()) )
  {
     if (range.left()>left()) setLeft(range.left());
     if (range.right()<right()) setRight(range.right());
  }
  if (fuzzyCompare(left(),right()) )
  {
      if (range.top()>top()) setTop(range.top());
      if (range.bottom()<bottom()) setBottom(range.bottom());
  }
}
