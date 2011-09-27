#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPropertyAnimation>

#include <QGraphicsOpacityEffect>

#include "utoolbarview.h"

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

private:
    Ui::MainWindow *ui;

    UToolbarView *toolbar;

    void keyPressEvent(QKeyEvent *event);

    void showHelp();
    void showStartupHint();
    void showToolbar();
};

#endif // MAINWINDOW_H
