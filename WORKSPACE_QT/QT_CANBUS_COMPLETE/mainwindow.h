#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scene.h"

#include <QMainWindow>

#include <iostream>
#include <QtWidgets/QMainWindow>
#include <qgridlayout.h>
#include <objectgl.h>
#include <QMenuBar>
#include <QMessageBox>

#include <QtCore>
#include <QDebug>

#include <QString>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <QRadioButton>
#include <QDockWidget>
#include <QGraphicsScene>
#include <QLCDNumber>
#include <QDial>
#include <QProgressBar>
#include <QSlider>


#include <fcntl.h>    // O_RDWR
#include <signal.h>
#include <unistd.h>   // exit

#include <libpcan.h>        // /usr/include/libpcan.h


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openCANPort();
    void sendCANMessage();
    void sendCANMessage(char);
    void receiveCANMessage();

public slots:

    void onButSendClicked();
         void onTimer_Tick();
         void changeToLum();
         void changeToDist();



private:

    HANDLE h;
    TPCANRdMsg pMsgBuff;

    Ui::MainWindow  *ui;
     QGridLayout     *m_layout,*graphics_layout;
     QPushButton     *but_send;
     QRadioButton    *but_distance, *but_luminosite,*but_pression,*but_acc, *but_anemo;

     QLineEdit       *receive_box_P_0,*receive_box_P_1,*receive_box_P_2,*receive_box_P_3 ;
     QLineEdit       *receive_box_P_4,*receive_box_P_5,*receive_box_P_6,*receive_box_P_7 ;

     QLineEdit       *receive_box_V_0,*receive_box_V_1,*receive_box_V_2,*receive_box_V_3 ;
     QLineEdit       *receive_box_V_4,*receive_box_V_5,*receive_box_V_6,*receive_box_V_7 ;

     QLineEdit       *receive_box_D_0,*receive_box_D_1,*receive_box_D_2,*receive_box_D_3 ;
     QLineEdit       *receive_box_D_4,*receive_box_D_5,*receive_box_D_6,*receive_box_D_7 ;

     QLineEdit       *receive_box_L_0,*receive_box_L_1,*receive_box_L_2,*receive_box_L_3 ;
     QLineEdit       *receive_box_L_4,*receive_box_L_5,*receive_box_L_6,*receive_box_L_7 ;

     QLineEdit       *receive_box_G_0,*receive_box_G_1,*receive_box_G_2,*receive_box_G_3 ;
     QLineEdit       *receive_box_G_4,*receive_box_G_5,*receive_box_G_6,*receive_box_G_7 ;

     QLineEdit       *receive_phi, *receive_psi, *receive_theta;

     QLineEdit       *send_box_0, *send_box_1,*send_box_2,*send_box_3;
     QLineEdit       *send_box_4, *send_box_5,*send_box_6,*send_box_7;

     QAction *_changeCheck;

     Scene *scene;
     QLCDNumber *LCD_dist, *LCD_vit, *LCD_lum, *LCD_pres;
     QDial *dial;
     QSlider *slider_range;

     QProgressBar *pressionBar;

     QLCDNumber debuger;


    QTimer *timer_tick;
};




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


#endif // MAINWINDOW_H
