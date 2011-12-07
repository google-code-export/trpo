#ifndef SAVELOAD_H
#define SAVELOAD_H

//class saveload
//{
//public:
//    saveload();
//};
#include <QString>
#include <QStringList>
#include <QRect>
#include <QRectF>
#include <QPointF>
#include <QPair>
#include "diagram.h"


//����� �����, ��������� �������� ���������� ��� ������������ � �������������� ����������
template<typename T >
class SaveLoad
{
  /*! ��������� ������ � ������
           \param[in] object ������ ��� ����������
           \return ������ � ���������� �������
   */
  static QString save(const T & object)
  {
      QString result=s;

      return result;
  }
  /*! ��������� ������ �� ������
           \param[in]  string ������ � ���������� �������
           \return  ��������������� ������
   */
  static T load(const QString & string)
  {
     T result=string;
      /*
      for (int i=escape_len-1;i>-1;i--)
          result.replace(escape_to[i],escape_from[i]);
      */
      return T;

  }
};

//��� ��������������� ������� ���������� ��������� ��� ���� int
template<>
class SaveLoad<int>
{
  /*! ��������� int � ������
           \param[in] object ������ ��� ����������
           \return ������ � ���������� �������
   */
  static QString save(const int & object);
//  {
//   return QString::number(object);
//  }
  /*! ��������� int �� ������
           \param[in]  string ������ � ���������� �������
           \return  ��������������� ������
   */
   static int load(const QString & string);
//   {
//     return string.toInt();
//   }
};



template<>
class SaveLoad<double>
{
    static QString save(const double & object);

    /*! ��������� int �� ������
             \param[in]  string ������ � ���������� �������
             \return  ��������������� ������
     */

     static double load(const QString & string);


/*! Converts string to double
 */

};

template<>
class SaveLoad<DiagramParent>
{
    static QString save(const DiagramParent & loc);

    /*! ��������� int �� ������
             \param[in]  string ������ � ���������� �������
             \return  ��������������� ������
     */

     static DiagramParent load(const QString & string);


/*! Converts string to double
 */

};
template<>
class SaveLoad <void *>

{
    static QString save( void * ptr);
   static void * load(const QString & string);
};
template<>
class SaveLoad <QRect>
{


   static QString save(const QRect & r);
  static QRect load(const QString & string);
};
template<>
class SaveLoad <QRectF>
{


   static QString save(const QRectF & r);
  static QRectF load(const QString & string);
};



template<>
class SaveLoad <QPointF>
{
   static QString save( const QPointF & p);
   static QPointF load( const QString & string);
};

//template<>
//class SaveLoad <QPair <Key, Value>>
//{/*
//   static QString save( const QPair & p);
//   static QPair load( const QString & string);*/
//};


//inline QString locationToString( DiagramParent & loc)
//{
//    QString result=QString::number(loc.diagramID(),10);
//    result+=";";
//    result+=QString::number(loc.blockID(),10);
//    return result;
//}

#endif // SAVELOAD_H
