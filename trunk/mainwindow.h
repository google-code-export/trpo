/*! \file   MainWindow
    \author Mamontov
    Describes a main window of project
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "diagramset.h"
#include "diagram.h"

//User interface
namespace Ui {
    class MainWindow;
}

class ToolSceneData;

/*! Class of main window of project
 */
class MainWindow : public QMainWindow {
    Q_OBJECT


private:
      /*! A tool items in table
       */
      QVector<ToolSceneData *>  m_tool_table_items;
      /*! Path for object
       */
      QString * m_path;
      /*! Diagram data
       */
      DiagramSet * m_set;
      /*! Whether we should delete an objects
       */
      bool      m_own;
public:
    /*! Creates a new window
        \param[in] parent object
     */
    explicit MainWindow(QWidget *parent = 0,
                        DiagramSet * set = 0,
                        const DiagramParent & p=DiagramParent());
    /*! Selects a tool by data
        \param[in] tool data
     */
    void selectTool(ToolSceneData * toolData);

    /*! Changes a set of data
        \param[in] set of data
     */
    void changeSet(DiagramSet * set);
    /*! Destructor
     */
    ~MainWindow();

protected:
    /*! Retranslate event
        \param[in] e event params
     */
    void changeEvent(QEvent *e);
    /*! Event, that occurs, when user presses the key
        \param[in] event paremeters
     */
    void keyPressEvent(QKeyEvent *event);
    

    
protected slots:
    void open();
    void save();
    void saveAs();
    void exportDiagram();
    /*! Shows a help dialog
     */
    void showHelp();
private:
    Ui::MainWindow *ui;  //!< UI
};

#endif // MAINWINDOW_H
