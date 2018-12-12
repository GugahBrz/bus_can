#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QtWidgets/QMainWindow>
#include <qgridlayout.h>
#include <objectgl.h>
#include <QMenuBar>
#include <QMessageBox>


#include <libpcan.h>        // /usr/include/libpcan.h




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor and destructor
    MainWindow(QWidget *parent = 0, int w=600, int h=400);
    ~MainWindow();
    void openCANPort();
    void receiveCANMessage();

    bool                    connect();



public slots:
         void onTimer_Tick();
         // Redraw the scene
         void                    onTimer_UpdateDisplay();


protected slots:


protected:

    // Overload of the resize event
    void                    resizeEvent(QResizeEvent *);

private:


//by francois auger------------------------------------
    HANDLE h1;
    TPCANRdMsg pMsgBuff;
    QTimer *timer_tick;
    QTimer *timer_updateAffichage;
//by hugo le ster------------------------------------

    // Layout of the window
    QGridLayout             *gridLayout;
    QWidget                 *gridLayoutWidget;

    // Central widget (where the openGL window is drawn)
    QWidget                 *centralWidget;

    // OpenGL object
    ObjectOpenGL            *Object_GL;
};

#endif // MAINWINDOW_H
