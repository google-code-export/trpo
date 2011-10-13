#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QBoxLayout>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent * event);
private:
    Ui::MainWindow *ui;


    void showHelp();
};

#endif // MAINWINDOW_H
