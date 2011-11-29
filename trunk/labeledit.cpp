#include "labeledit.h"
#include "diagramscene.h"
#include "box.h"
#include "alabelitem.h"

LabelEdit::LabelEdit(DiagramScene * scene, ::Box * item,QWidget *parent ) :
    QPlainTextEdit(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    m_scene=scene;
    m_boxitem=item;
    m_alabelitem=NULL;
}

LabelEdit::LabelEdit(DiagramScene * scene, ALabelItem * item,QWidget *parent ) :
    QPlainTextEdit(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    m_scene=scene;
    m_boxitem=NULL;
    m_alabelitem=item;
}

void LabelEdit::keyPressEvent(QKeyEvent *e)
{
   if (e->key()==Qt::Key_Return &&
       (e->modifiers() & Qt::ControlModifier) !=0
      )
   {
    if (m_boxitem)
    { m_boxitem->updateString(this->toPlainText());   }
    if (m_alabelitem)
    { m_alabelitem->trySetText(this->toPlainText());  }
    m_scene->toggleEditStateOff();
   }
   else
   {
     this->QPlainTextEdit::keyPressEvent(e);
   }

}
