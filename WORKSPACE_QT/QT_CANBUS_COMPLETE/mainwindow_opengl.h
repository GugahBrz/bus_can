#ifndef MAINWINDOW_OPENGL_H
#define MAINWINDOW_OPENGL_H

#include <iostream>
#include <QtWidgets/QMainWindow>
#include <qgridlayout.h>
#include <objectgl.h>
#include <QMenuBar>
#include <QMessageBox>


#include <libpcan.h>        // /usr/include/libpcan.h




class MainWindow_opengl : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor and destructor
    MainWindow_opengl(QWidget *parent = 0, int w=600, int h=400);
    ~MainWindow_opengl();
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


    HANDLE h1;
    TPCANRdMsg pMsgBuff;
    QTimer *timer_tick;
    QTimer *timer_updateAffichage;

    // Layout of the window
    QGridLayout             *gridLayout;
    QWidget                 *gridLayoutWidget;

    // Central widget (where the openGL window is drawn)
    QWidget                 *centralWidget;

    // OpenGL object
    ObjectOpenGL            *Object_GL;
};

#endif // MAINWINDOW_OPENGL_H
