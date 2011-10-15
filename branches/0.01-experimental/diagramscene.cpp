#include "diagramscene.h"
#include "helpwindow.h"
#include "toolpanel.h"
#include <QTextEdit>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsProxyWidget>

DiagramScene::DiagramScene(Diagram * d,QObject *parent) :
    QGraphicsScene(parent)
{
  m_diag=d;
  m_tooltype=TT_BLOCK;
  m_panel=NULL;
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos=event->buttonDownScenePos(event->button());
    //Do in case of nothing found
    if (this->items(pos).size()==0)
    {

    }
    //Propagate key pressing event
    else
    {
        this->QGraphicsScene::mousePressEvent(event);
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