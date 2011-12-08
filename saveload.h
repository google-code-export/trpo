#ifndef SAVELOAD_H
#define SAVELOAD_H

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
public:
  /*! ��������� ������ � ������
           \param[in] object ������ ��� ����������
           \return ������ � ���������� �������
   */
  static QString save(const T & /*object*/)
  {
     return QString();
  }
  /*! ��������� ������ �� ������
           \param[in]  string ������ � ���������� �������
           \return  ��������������� ������
   */
  static T load(const QString & string)
  {
      return T();
  }
};

//��� ��������������� ������� ���������� ��������� ��� ���� int
template<>
class SaveLoad<int>
{
public:
    /*! ��������� int � ������
           \param[in] object ������ ��� ����������
           \return ������ � ���������� �������
   */
  static QString save(const int & object);
  /*! ��������� int �� ������
           \param[in]  string ������ � ���������� �������
           \return  ��������������� ������
   */
   static int load(const QString & string);
};



template<>
class SaveLoad<double>
{
public:
    static QString save(const double & object);
    /*! ��������� int �� ������
             \param[in]  string ������ � ���������� �������
             \return  ��������������� ������
     */

     static double load(const QString & string);
};

template<>
class SaveLoad<DiagramParent>
{
public:
    static QString save(const DiagramParent & loc);
    /*! ��������� int �� ������
        \param[in]  string ������ � ���������� �������
        \return  ��������������� ������
     */
    static DiagramParent load(const QString & string);
};

template<>
class SaveLoad <void *>
{
public:
   static QString save( void * ptr);
   static void * load(const QString & string);
};

template<typename T>
class SaveLoad<T*>
{
public:
    static QString save( T * ptr)               { return SaveLoad<void*>::save(ptr);}
    static void * load(const QString & string)  { return (T*)(SaveLoad<void*>::load(string));}

};

template<>
class SaveLoad <QRect>
{
public:
  static QString save(const QRect & r);
  static QRect load(const QString & string);
};

template<>
class SaveLoad <QRectF>
{
public:
   static QString save(const QRectF & r);
   static QRectF load(const QString & string);
};



template<>
class SaveLoad <QPointF>
{
public:
   static QString save( const QPointF & p);
   static QPointF load( const QString & string);
};


template<>
class SaveLoad <QSize>
{
public:
   static QString save(const QSize & s);
   static QSize load(const QString & string);
};


template<typename T1, typename T2>
class SaveLoad <QPair <T1,T2> >
{
 public:
   static QString save( const QPair<T1,T2>  & p)
   {
     QString result1=save(p.first) ;
     QString result2=save(p.second) ;
     QString result;
     result.append(result1);
     result.append("@");
     result.append(result2);
     return result;
   }
   static QPair<T1,T2> load( const QString & string)
   {
       QPair<T1,T2> tmpPair;
       QString str1 =   string.section('@', 0, 0);
       QString str2 =   string.section('@', 1, 1);

       tmpPair.first=str1;
       tmpPair.second=str2;
       return tmpPair;
   }
};



template<typename T>
class SaveLoad <QVector <T> >
{
    public:


    static QString save( const QVector <T> & v)
    {
      QString tmpVec;
      for (int i=0;i<v.size();i++)
      {
            tmpVec.append(SaveLoad<T>::save(v[i]));
            if (i!=v.size()-1)
            tmpVec.append("@");
      };
      return tmpVec;
    }
    static QVector <T> load( const QString & string)
    {
      QVector<QString> vecloadTmp;
      QStringList tmp=string.split("@");
       for (int i=0;i<tmp.size();i++)
       {
          vecloadTmp.append(tmp[i]);
       }
       return  vecloadTmp;
    }
};

template<typename T>
QString save(const T & obj) { return SaveLoad<T>::save(obj); }

template<typename T>
T load(const QString & string) { return SaveLoad<T>::load(string); }


#endif // SAVELOAD_H
