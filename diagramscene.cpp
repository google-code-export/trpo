#include "diagramscene.h"
#include "helpwindow.h"
#include "toolpanel.h"
#include "labeledit.h"
#include "alabelitem.h"
#include <QTextEdit>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsProxyWidget>
#include <QFontMetricsF>
#include <QApplication>

DiagramScene::DiagramScene(Diagram * d,QObject *parent) :
    QGraphicsScene(parent)
{
  m_diag=d;
  m_tooltype=TT_BLOCK;
  m_panel=NULL;
  //Compute size of default block
  //Get small size label
  QFont fnt=this->font();
  fnt.setPointSize(10);
  QFontMetrics mtr1(fnt);
  m_default_number_size=mtr1.boundingRect("9");
  QRectF & number_size=m_default_number_size;
  //Get size of default text
  QFontMetrics mtr2(this->font());
  QRect label_size=mtr2.boundingRect(DEFAULT_BLOCK_TEXT);
  m_default_block_size.setX(0);
  m_default_block_size.setY(0);
  m_default_block_size.setWidth(
                                ((label_size.width()>number_size.width())?
                                label_size.width():number_size.width())
                                +BLOCK_SPACE_X*2
                               );
  m_default_block_size.setHeight(
                                 label_size.height()+
                                 (number_size.height()+BLOCK_SPACE_Y)*2
                                );
  //Compute a size of default label
  QRect tmp_alabel=mtr2.boundingRect(DEFAULT_ALABEL_TEXT);
  m_alabel_block_size=QRectF(tmp_alabel.x(),tmp_alabel.y(),
                             tmp_alabel.width()+2, tmp_alabel.height()+2);
  //Sets a no edit state
  m_edit_state=TES_NONE;
  m_label_editor=NULL;
  m_label_editor_in_scene=NULL;
  //Sets a no dragging state
  m_dragstate=DS_NONE;
  m_draggingblock=NULL;
  m_resizingblockcorner=BC_LOWERLEFT;
  m_moving_label=NULL;
}

const QRectF & DiagramScene::getDefaultBlockNumberSize() const
{
    return this->m_default_number_size;
}
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos=event->buttonDownScenePos(event->button());
    //Do in case of nothing found
    if (this->items(pos).size()==0)
    {
      if (m_tooltype==TT_BLOCK && m_edit_state==TES_NONE)
          addBlock(event);
      if (m_tooltype==TT_ANNOTATION_LABEL && m_edit_state==TES_NONE)
          addAnnotationLabel(event);
    }
    //Propagate key pressing event
    else
    {
     bool isWidgetClicked=false;
     QList<QGraphicsItem *> lst=this->items(pos);
     for (int i=0;i<lst.size();i++)
     {
         if (lst[i]->type()==QGraphicsProxyWidget::Type)
             isWidgetClicked=true;
     }
     if (isWidgetClicked)
        this->QGraphicsScene::mousePressEvent(event);
     else
     {
       if (m_tooltype==TT_ERASER && m_edit_state==TES_NONE)
             processRemoving(lst);
       else if (m_tooltype==TT_SELECT) blockResizeMoveEnter(event);
       else this->QGraphicsScene::mousePressEvent(event);
     }
    }
}
void DiagramScene::keyPressEvent(QKeyEvent * event)
{
  bool handled=false;
  //In case when F1 pressed show help
  if (m_panel)
     handled=processKeyToolSelect(event);
  if (event->key()==Qt::Key_F1)
  {
      HelpWindow d;
      d.exec();
      handled=true;
  }
  //In case when Shift presset add panel if not present
  if (event->key()==Qt::Key_Shift)
  {
      //Compute panel position
      QPointF  panel_pos;
      QPoint local_pos=m_view->mapFromGlobal(QCursor::pos());
      panel_pos=m_view->mapToScene(local_pos);
      panel_pos.setX(panel_pos.x()-PANEL_WIDTH/2);
      panel_pos.setY(panel_pos.y()-PANEL_HEIGHT/2);

      //Check under- and over-flow
      if (panel_pos.x()<0)  panel_pos.setX(0);
      if (panel_pos.y()<0)  panel_pos.setY(0);
      if (panel_pos.x()+PANEL_WIDTH>this->width())
           panel_pos.setX(this->width()-PANEL_WIDTH);
      if (panel_pos.y()+PANEL_HEIGHT>this->height())
          panel_pos.setY(this->height()-PANEL_HEIGHT);

      if (m_panel==NULL)
      {
       //Add tool panel
       m_panel=new ToolPanel(m_view);
       m_panel_in_scene=this->addWidget(m_panel);
       //Set parameters and update
       m_panel->setGeometry(panel_pos.x(),panel_pos.y(),PANEL_WIDTH,PANEL_HEIGHT);
       m_panel->update();
      }
      else
      {
          //Move panel
          m_panel->move(panel_pos.x(),panel_pos.y());
          m_panel->update();
      }
      handled=true;
  }
  if (!handled)
  {
     QPoint viewpos=m_view->mapFromGlobal(QCursor::pos());
     QPointF scenepos=m_view->mapToScene(viewpos);
     QList<QGraphicsItem *> items=this->items(scenepos);
     for (int i=0;(i<items.size()) && !handled;i++)
     {
         if (items[i]->type()==BoxItem::USERTYPE)
         {
             static_cast<BoxItem *>(items[i])->keyPressEvent(event);
             handled=true;
         }
         if (items[i]->type()==ALabelItem::USERTYPE)
         {
             static_cast<ALabelItem *>(items[i])->keyPressEvent(event);
             handled=true;
         }
     }
  }
  if (!handled)
      this->QGraphicsScene::keyPressEvent(event);
}


void DiagramScene::setTool(ToolType t)
{
    Q_ASSERT( m_panel );
    m_tooltype=t;

    this->removeItem(m_panel_in_scene);

    m_panel=NULL;
    m_panel_in_scene=NULL;
}

bool DiagramScene::processKeyToolSelect(QKeyEvent * event)
{
    int       keys[6]={Qt::Key_1,Qt::Key_2,Qt::Key_3,Qt::Key_4,Qt::Key_5,Qt::Key_6};
    ToolType types[6]={TT_SELECT,TT_ERASER,TT_BLOCK,TT_ARROW,TT_ANNOTATION_LINE,
                       TT_ANNOTATION_LABEL};
    for (int i=0;i<6;i++)
    {
        if (event->key()==keys[i])
            this->setTool(types[i]);
    }
    return false;
}

QRectF DiagramScene::getDefaultBlockSize(const QPointF & pos)
{
  QRectF result;
  result.setX(pos.x()-m_default_block_size.width()/2);
  result.setY(pos.y()-m_default_block_size.height()/2);
  result.setWidth(m_default_block_size.width());
  result.setHeight(m_default_block_size.height());
  return result;
}

QRectF DiagramScene::getDefaultAnnotationLabelSize(const QPointF & pos)
{
  QRectF result;
  result.setX(pos.x()-m_alabel_block_size.width()/2);
  result.setY(pos.y()-m_alabel_block_size.height()/2);
  result.setWidth(m_alabel_block_size.width());
  result.setHeight(m_alabel_block_size.height());
  return result;
}

void DiagramScene::addBlock(QGraphicsSceneMouseEvent *event)
{
 if (m_diag->canAddBoxes())
 {
  QRectF size=getDefaultBlockSize(event->scenePos());
  if (m_diag->canBePlaced(size,(BoxItem*)NULL))
  {
   QVector<ArrowPoint *> list=m_diag->getNearArrowPoints(size);
   if (m_diag->canBePlacedAroundPoints(size,list))
   {
    BoxItem * b=new BoxItem(event->scenePos(),this);
    this->addItem(b);
    m_diag->addBox(b);
    b->attachAllPoints(list);
   }
  }
 }
}

void DiagramScene::addAnnotationLabel(QGraphicsSceneMouseEvent *event)
{
 QRectF size=getDefaultAnnotationLabelSize(event->scenePos());
 if (m_diag->canBePlaced(size,(ALabelItem*)NULL))
 {
     ALabelItem * a=new ALabelItem(size);
     this->addItem(a);
     m_diag->addAnnotationLabel(a);
 }
}

void DiagramScene::decrementBlockID(BoxItem * block)
{
 int previd=block->id();
 int newid=((block->id()==0)?DIAGRAM_MAX_BLOCKS:previd)-1;
 processChangeBlockID(block,previd,newid);
}

void DiagramScene::incrementBlockID(BoxItem * block)
{
 int previd=block->id();
 int newid=(block->id()==DIAGRAM_MAX_BLOCKS-1)?0:(previd+1);
 processChangeBlockID(block,previd,newid);
}

void DiagramScene::processChangeBlockID(BoxItem * block, char previd, char newid)
{
 m_diag->undoIfSwapped(block,previd);
 if (m_diag->getBlockByID(newid))
 {
        BoxItem * oldblock=m_diag->getBlockByID(newid);
        m_diag->addNewSwap(oldblock,previd,block,newid);
 }
 m_diag->setBlockID(block,newid);
}

void DiagramScene::processRemoving(const QList<QGraphicsItem *> & items)
 {
    for (int i=0;i<items.size();i++)
    {
        if (items[i]->type()==BoxItem::USERTYPE)
        {
            char id=static_cast<BoxItem *>(items[i])->id();
            m_diag->removeBlock(id);
            this->removeItem(items[i]);
        }
        if (items[i]->type()==ALabelItem::USERTYPE)
        {
            m_diag->removeAnnotationLabel(static_cast<ALabelItem *>(items[i]));
            this->removeItem(items[i]);
        }
    }
 }

void DiagramScene::toggleEditStateOff()
{
    this->m_edit_state=TES_NONE;
    this->m_label_editor->releaseKeyboard();
    this->removeItem(m_label_editor_in_scene);
    m_label_editor=NULL;
    m_label_editor_in_scene=NULL;
}


void  DiagramScene::blockResizeMoveEnter ( QGraphicsSceneMouseEvent * event )
{
   QList<QGraphicsItem *> lst=items(event->scenePos());
   bool isWidgetClicked=false;
   for (int i=0;i<lst.size();i++)
   {
       if (lst[i]->type()==QGraphicsProxyWidget::Type)
           isWidgetClicked=true;
   }
   if (isWidgetClicked || lst.size()==0)
      this->QGraphicsScene::mousePressEvent(event);
   else
   {
        for(int i=0;i<lst.size();i++)
        {
          if (lst[i]->type()==BoxItem::USERTYPE)
          {
                determineDraggingBoxAction(static_cast<BoxItem *>(lst[i]),
                                           event->scenePos()
                                           );
          }
          if (lst[i]->type()==ALabelItem::USERTYPE)
          {
              m_dragstate=DS_ALABEL_MOVE;
              QPointF pos=event->scenePos();
              ALabelItem * item=static_cast<ALabelItem *>(lst[i]);
              m_moving_label=item;
              QRectF       rct=item->boundingRect();
              m_blockmovingparams[0]=(pos.x()-rct.left())/rct.width();
              m_blockmovingparams[1]=(pos.y()-rct.top())/rct.height();
          }
        }
   }
}

#define CORNER_PRECISE 7
void DiagramScene::determineDraggingBoxAction(BoxItem * item,const QPointF & pos)
{
  bool handled=false;
  if (   pos.x()-item->boundingRect().x() < CORNER_PRECISE
         &&  pos.y()-item->boundingRect().y() < CORNER_PRECISE
         &&  !handled
     )
  {
     handled=true;
     m_dragstate=DS_BLOCK_RESIZE;
     m_draggingblock=item;
     m_resizingblockcorner=BC_UPPERLEFT;
  }
  //printf("Check for upper right: %d %d %d %d\n",(int)(item->boundingRect().right())
  //       ,pos.x(),pos.y(),item->boundingRect().y()
  //        );
  if (   item->boundingRect().right()-pos.x() < CORNER_PRECISE
         &&  pos.y()-item->boundingRect().y() < CORNER_PRECISE
         &&  !handled
     )
  {
     handled=true;
     m_dragstate=DS_BLOCK_RESIZE;
     m_draggingblock=item;
     m_resizingblockcorner=BC_UPPERRIGHT;
  }
  if (   pos.x()-item->boundingRect().x() < CORNER_PRECISE
         &&  item->boundingRect().bottom()-pos.y() < CORNER_PRECISE
         &&  !handled
     )
  {
     handled=true;
     m_dragstate=DS_BLOCK_RESIZE;
     m_draggingblock=item;
     m_resizingblockcorner=BC_LOWERLEFT;
  }
  //printf("Check for upper right: %d %d %d %d\n",(int)(item->boundingRect().right())
  //       ,pos.x(),item->boundingRect().bottom(),pos.y()
  //       );
  if (   item->boundingRect().right()-pos.x() < CORNER_PRECISE
         &&  item->boundingRect().bottom()-pos.y() < CORNER_PRECISE
         &&  !handled
     )
  {
     handled=true;
     m_dragstate=DS_BLOCK_RESIZE;
     m_draggingblock=item;
     m_resizingblockcorner=BC_LOWERRIGHT;
  }
  if (!handled)
  {
      m_dragstate=DS_BLOCK_MOVE;
      m_draggingblock=item;
      QRectF rct=item->boundingRect();
      m_blockmovingparams[0]=((qreal)pos.x()-rct.left())/rct.width();
      m_blockmovingparams[1]=((qreal)pos.y()-rct.top())/rct.height();
  }
}

void resizeLeft(QRectF & oldrect, QPointF & pos)
{
    if (pos.x()>=oldrect.right())
    {
        qreal rt=oldrect.right();
        oldrect.setWidth(pos.x()-oldrect.left());
        oldrect.setLeft(rt);
    } else oldrect.setLeft(pos.x());
}

void resizeRight(QRectF & oldrect, QPointF & pos)
{
    if (pos.x()<=oldrect.left())
    {
        qreal oldleft=oldrect.left();
        oldrect.setX(pos.x());
        oldrect.setWidth(oldleft-pos.x());
    } else oldrect.setWidth(pos.x()-oldrect.left());
}
void resizeLower(QRectF & oldrect, QPointF & pos)
{
    if (pos.y()<=oldrect.top())
    {
        qreal oldtop=oldrect.top();
        oldrect.setY(pos.y());
        oldrect.setHeight(oldtop-pos.y());
    } else oldrect.setHeight(pos.y()-oldrect.top());
}

void resizeUpper(QRectF & oldrect, QPointF & pos)
{
   if (pos.y()>=oldrect.bottom())
   {
      qreal oldbottom=oldrect.bottom();
      oldrect.setHeight(pos.y()-oldrect.top());
      oldrect.setY(oldbottom);
   } else oldrect.setY(pos.y());
}


void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_tooltype==TT_SELECT)
        blockResizeMoveLeave(event);
}

void  DiagramScene::blockResizeMoveLeave ( QGraphicsSceneMouseEvent * event )
{
    this->QGraphicsScene::mouseReleaseEvent(event);
    QPointF pos=event->scenePos();
    if (m_dragstate==DS_BLOCK_RESIZE)
    {
         QRectF oldrect=m_draggingblock->boundingRect();
        //Compute new rectangle
#define WHATDO(X,Y,Z)   if (m_resizingblockcorner == X) \
                        {   Y (oldrect,pos); Z (oldrect,pos);}
        WHATDO(BC_LOWERLEFT,resizeLower,resizeLeft);
        WHATDO(BC_LOWERRIGHT,resizeLower,resizeRight);
        WHATDO(BC_UPPERLEFT,resizeUpper,resizeLeft);
        WHATDO(BC_UPPERRIGHT,resizeUpper,resizeRight);
#undef  WHATDO
        QRectF labelsize=this->getDefaultBlockNumberSize();
        bool too_small=oldrect.height()<=labelsize.height() || oldrect.width()<=labelsize.width();
        bool can_placed=m_diag->canBePlaced(oldrect,m_draggingblock);
        if( !too_small && can_placed)
        {
         QVector<ArrowPoint *> lst=m_diag->getNearArrowPoints(oldrect);
         if (m_diag->canBePlacedAroundPoints(oldrect,lst))
         {
           m_draggingblock->clearPointReferences();
           m_draggingblock->setRect(oldrect);
           m_draggingblock->attachAllPoints(lst);
           this->update();
         }
        }
        m_dragstate=DS_NONE;
        m_draggingblock=NULL;
        m_resizingblockcorner=BC_LOWERLEFT;
    }
    if (m_dragstate==DS_BLOCK_MOVE)
    {
       QRectF oldrect=m_draggingblock->boundingRect();
       qreal x=pos.x()-m_blockmovingparams[0]*oldrect.width();
       qreal y=pos.y()-m_blockmovingparams[1]*oldrect.height();
       if (x<0) x=0;
       if (y<0) y=0;
       if (x+oldrect.width()>this->width()) x=this->width()-oldrect.width();
       if (y+oldrect.height()>this->height()) y=this->height()-oldrect.height();
       QRectF newrect(x,y,oldrect.width(),oldrect.height());
       bool can_placed=m_diag->canBePlaced(newrect,m_draggingblock);
       if(can_placed)
       {
          m_draggingblock->setRect(newrect);
          m_draggingblock->clearPointReferences();
          this->update();
       }
       m_dragstate=DS_NONE;
       m_draggingblock=NULL;
       m_resizingblockcorner=BC_LOWERLEFT;
    }
    if (m_dragstate==DS_ALABEL_MOVE)
    {
        QRectF oldrect=m_moving_label->boundingRect();
        qreal x=pos.x()-m_blockmovingparams[0]*oldrect.width();
        qreal y=pos.y()-m_blockmovingparams[1]*oldrect.height();
        if (x<0) x=0;
        if (y<0) y=0;
        if (x+oldrect.width()>this->width()) x=this->width()-oldrect.width();
        if (y+oldrect.height()>this->height()) y=this->height()-oldrect.height();
        QRectF newrect(x,y,oldrect.width(),oldrect.height());
        bool can_placed=m_diag->canBePlaced(newrect,m_moving_label);
        if(can_placed)
        {
           m_moving_label->setRect(newrect);
           this->update();
        }
        m_dragstate=DS_NONE;
        m_moving_label=NULL;
    }
}