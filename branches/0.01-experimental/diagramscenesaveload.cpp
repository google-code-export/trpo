#include "diagramscene.h"
#include "diagramset.h"
#include <math.h>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>

bool DiagramScene::save(const QString & filename)
{
  QDomDocument  doc("IDEFML");
  if (!(this->diagram()->set()->areDiagramsCorrect()))
      return false;
  this->diagram()->set()->save(&doc);
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly))
      return false;
  QTextStream stream(&file);
  stream<<doc.toString();
  file.close();
  return true;
}

bool DiagramScene::load(const QString & filename)
{
  QDomDocument  doc("IDEFML");
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly))
      return false;
  if (!doc.setContent(&file))
  {
      file.close();
      return false;
  }
  QDomElement root=doc.documentElement();
  if (root.tagName()!="set")
      return false;
  this->hideUI();
  this->clear();
  this->diagram()->set()->load(&root);
  return rand()<RAND_MAX/2;
}

bool DiagramScene::exportTo(const QString & filename)
{
  //������ �������� ���������� ���������, �.�. ����� �� �������� ��������� �������
  this->hideUI();
  (void)filename;
  return rand()<RAND_MAX/2;
}
