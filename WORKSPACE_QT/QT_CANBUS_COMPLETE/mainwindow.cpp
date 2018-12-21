#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objectgl.h"


#define DEFAULT_NODE "/dev/pcanusb32"
#define ID_1    0x01
#define ID_2    0x02
#define ID_3    0x03
#define ID_4    0x04
#define ID_MPU  0x11
#define O_RDWR		     02
//===============================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QWidget *zoneCentrale = new QWidget;
    setCentralWidget(zoneCentrale);
    m_layout = new QGridLayout();
    graphics_layout = new QGridLayout();
    m_layout->addLayout(graphics_layout,13,1,3,6);

    QGraphicsView *view = new QGraphicsView();
    scene = new Scene (0, 0, 80, 80,this);
    view->setScene(scene);
    view->resize(80,80);
    //view->show(); // out of layout

    LCD_dist = new QLCDNumber(5);
    LCD_dist->display("00000");
    LCD_dist->resize(80,80);
    LCD_dist->setPalette(Qt::black);
    //LCD_dist->show();  // out of layout

    LCD_vit = new QLCDNumber(5);
    LCD_vit->display("00000");
    LCD_vit->resize(80,80);
    LCD_vit->setPalette(Qt::black);
    //LCD_vit->show();  // out of layout

    LCD_pres = new QLCDNumber(5);
    LCD_pres->display("00000");
    LCD_pres->resize(80,80);
    LCD_pres->setPalette(Qt::black);
    //LCD_pres->show();  // out of layout

    LCD_lum = new QLCDNumber(5);
    LCD_lum->display("00000");
    LCD_lum->resize(80,80);
    LCD_lum->setPalette(Qt::black);
    //LCD_lum->show();  // out of layout

    pressionBar = new QProgressBar();
    pressionBar->resize(30,80);
    pressionBar->setMaximum(115);
    pressionBar->setMinimum(50);
    //pressionBar->show(); // out of layout

    dial = new QDial();
    dial->resize(80,80);
    dial->setEnabled(false);
    dial->setNotchesVisible(true);
    dial->setNotchTarget(10);
    dial->setWrapping(0);
    dial->setRange(0,255);
    dial->setPalette(Qt::black);
    //dial->show(); // out of layout


    slider_range = new QSlider();
    slider_range->setTickPosition(QSlider::TicksBothSides);
    slider_range->setMaximum(200);
    slider_range->resize(80,80);
    slider_range->setPalette(Qt::black);


    graphics_layout->addWidget(view,0,0);
    graphics_layout->addWidget(LCD_lum,1,0);
    graphics_layout->addWidget(slider_range,0,2);
    graphics_layout->addWidget(LCD_dist,1,2);
    graphics_layout->addWidget(pressionBar,0,3);
    graphics_layout->addWidget(LCD_pres,1,3);
    graphics_layout->addWidget(dial,0,4);
    graphics_layout->addWidget(LCD_vit,1,4);


    but_send = new QPushButton("Send Message");

/*  but_distance = new QCheckBox();
    but_luminosite = new QCheckBox();
    but_pression = new QCheckBox();
    but_acc = new QCheckBox();
    but_anemo = new QCheckBox(); */

    but_distance = new QRadioButton();
    but_luminosite = new QRadioButton();
    but_pression = new QRadioButton();
    but_acc = new QRadioButton();
    but_anemo = new QRadioButton();

    QLabel *label_to_send = new QLabel;
    label_to_send->setText("Message to Send");
    label_to_send->setFont(QFont("lucida",15,100,false));
    QLabel *label_received = new QLabel;
    label_received->setText("Message Received");
    label_received->setFont(QFont("lucida",15,100,false));

    QLabel *carte1 = new QLabel;
    carte1->setText("Carte 1");
    carte1->setFont(QFont("times",13,80,true));
    QLabel *carte2 = new QLabel;
    carte2->setText("Carte 2");
    carte2->setFont(QFont("times",13,80,true));
    QLabel *carte3 = new QLabel;
    carte3->setText("Carte 3");
    carte3->setFont(QFont("times",12,80,true));

    QLabel *label_pression = new QLabel;
    label_pression->setText("MPL115A Pression");

    QLabel *label_anemo = new QLabel;
    label_anemo->setText("Anenometer SOMFY");

    QLabel *label_distance = new QLabel;
    label_distance->setText("VL6180 Distance");

    QLabel *label_luminosite = new QLabel;
    label_luminosite->setText("VL6180 Luminosite");

    QLabel *label_acelerometre = new QLabel;
    label_acelerometre->setText("MPU9250 Acelerometre");

    //Mesures Capteur MPU9520
    QLabel *phi_received = new QLabel;
    phi_received->setText("Phi:");
    QLabel *psi_received = new QLabel;
    psi_received->setText("Psi:");
    QLabel *theta_received = new QLabel;
    theta_received->setText("Theta:");

    send_box_0 = new QLineEdit();
    send_box_1 = new QLineEdit();
    send_box_2 = new QLineEdit();
    send_box_3 = new QLineEdit();
    send_box_4 = new QLineEdit();
    send_box_5 = new QLineEdit();
    send_box_6 = new QLineEdit();
    send_box_7 = new QLineEdit();
    send_box_0->setText("0");
    send_box_1->setText("0");
    send_box_2->setText("0");
    send_box_3->setText("0");
    send_box_4->setText("0");
    send_box_5->setText("0");
    send_box_6->setText("0");
    send_box_7->setText("0");

    //MPL115A_ANEMO Pression
    receive_box_P_0 = new QLineEdit();
    receive_box_P_1 = new QLineEdit();
    receive_box_P_2 = new QLineEdit();
    receive_box_P_3 = new QLineEdit();
    receive_box_P_4 = new QLineEdit();
    receive_box_P_5 = new QLineEdit();
    receive_box_P_6 = new QLineEdit();
    receive_box_P_7 = new QLineEdit();

    //MPL115A_ANEMO Vitesse du vent
    receive_box_V_0 = new QLineEdit();
    receive_box_V_1 = new QLineEdit();
    receive_box_V_2 = new QLineEdit();
    receive_box_V_3 = new QLineEdit();
    receive_box_V_4 = new QLineEdit();
    receive_box_V_5 = new QLineEdit();
    receive_box_V_6 = new QLineEdit();
    receive_box_V_7 = new QLineEdit();

    //VL6180X Distance
    receive_box_D_0 = new QLineEdit();
    receive_box_D_1 = new QLineEdit();
    receive_box_D_2 = new QLineEdit();
    receive_box_D_3 = new QLineEdit();
    receive_box_D_4 = new QLineEdit();
    receive_box_D_5 = new QLineEdit();
    receive_box_D_6 = new QLineEdit();
    receive_box_D_7 = new QLineEdit();

    //VL6180X Luminosite
    receive_box_L_0 = new QLineEdit();
    receive_box_L_1 = new QLineEdit();
    receive_box_L_2 = new QLineEdit();
    receive_box_L_3 = new QLineEdit();
    receive_box_L_4 = new QLineEdit();
    receive_box_L_5 = new QLineEdit();
    receive_box_L_6 = new QLineEdit();
    receive_box_L_7 = new QLineEdit();

    //MPU9250 Acelerometre/Giroscope
    receive_box_G_0 = new QLineEdit();
    receive_box_G_1 = new QLineEdit();
    receive_box_G_2 = new QLineEdit();
    receive_box_G_3 = new QLineEdit();
    receive_box_G_4 = new QLineEdit();
    receive_box_G_5 = new QLineEdit();
    receive_box_G_6 = new QLineEdit();
    receive_box_G_7 = new QLineEdit();
    receive_phi = new QLineEdit();
    receive_psi = new QLineEdit();
    receive_theta = new QLineEdit();

    m_layout->addWidget(label_to_send,0,0);

    m_layout->addWidget(but_send,1,0);
    m_layout->addWidget(send_box_7,1,1);
    m_layout->addWidget(send_box_6,1,2);
    m_layout->addWidget(send_box_5,1,3);
    m_layout->addWidget(send_box_4,1,4);
    m_layout->addWidget(send_box_3,1,5);
    m_layout->addWidget(send_box_2,1,6);
    m_layout->addWidget(send_box_1,1,7);
    m_layout->addWidget(send_box_0,1,8);


    m_layout->addWidget(label_received,3,0);


    m_layout->addWidget(carte1,4,0);

    m_layout->addWidget(label_pression,5,0);
    m_layout->addWidget(receive_box_P_7,5,1);
    m_layout->addWidget(receive_box_P_6,5,2);
    m_layout->addWidget(receive_box_P_5,5,3);
    m_layout->addWidget(receive_box_P_4,5,4);
    m_layout->addWidget(receive_box_P_3,5,5);
    m_layout->addWidget(receive_box_P_2,5,6);
    m_layout->addWidget(receive_box_P_1,5,7);
    m_layout->addWidget(receive_box_P_0,5,8);
    m_layout->addWidget(but_pression,5,9);

    m_layout->addWidget(label_anemo,6,0);
    m_layout->addWidget(receive_box_V_7,6,1);
    m_layout->addWidget(receive_box_V_6,6,2);
    m_layout->addWidget(receive_box_V_5,6,3);
    m_layout->addWidget(receive_box_V_4,6,4);
    m_layout->addWidget(receive_box_V_3,6,5);
    m_layout->addWidget(receive_box_V_2,6,6);
    m_layout->addWidget(receive_box_V_1,6,7);
    m_layout->addWidget(receive_box_V_0,6,8);
    m_layout->addWidget(but_anemo,6,9);


    m_layout->addWidget(carte2,7,0);

    m_layout->addWidget(label_distance,8,0);
    m_layout->addWidget(receive_box_D_7,8,1);
    m_layout->addWidget(receive_box_D_6,8,2);
    m_layout->addWidget(receive_box_D_5,8,3);
    m_layout->addWidget(receive_box_D_4,8,4);
    m_layout->addWidget(receive_box_D_3,8,5);
    m_layout->addWidget(receive_box_D_2,8,6);
    m_layout->addWidget(receive_box_D_1,8,7);
    m_layout->addWidget(receive_box_D_0,8,8);
    m_layout->addWidget(but_distance,8,9);

    m_layout->addWidget(label_luminosite,9,0);
    m_layout->addWidget(receive_box_L_7,9,1);
    m_layout->addWidget(receive_box_L_6,9,2);
    m_layout->addWidget(receive_box_L_5,9,3);
    m_layout->addWidget(receive_box_L_4,9,4);
    m_layout->addWidget(receive_box_L_3,9,5);
    m_layout->addWidget(receive_box_L_2,9,6);
    m_layout->addWidget(receive_box_L_1,9,7);
    m_layout->addWidget(receive_box_L_0,9,8);
    m_layout->addWidget(but_luminosite,9,9);


    m_layout->addWidget(carte3,10,0);

    m_layout->addWidget(label_acelerometre,11,0);
    m_layout->addWidget(phi_received,12,1);
    m_layout->addWidget(psi_received,12,3);
    m_layout->addWidget(theta_received,12,5);
    m_layout->addWidget(receive_box_G_7,11,1);
    m_layout->addWidget(receive_box_G_6,11,2);
    m_layout->addWidget(receive_box_G_5,11,3);
    m_layout->addWidget(receive_box_G_4,11,4);
    m_layout->addWidget(receive_box_G_3,11,5);
    m_layout->addWidget(receive_box_G_2,11,6);
    m_layout->addWidget(receive_box_G_1,11,7);
    m_layout->addWidget(receive_box_G_0,11,8);
    m_layout->addWidget(receive_phi,12,2);
    m_layout->addWidget(receive_psi,12,4);
    m_layout->addWidget(receive_theta,12,6);
    m_layout->addWidget(but_acc,11,9);


    //debuger.show();

    zoneCentrale->setLayout(m_layout);

    openCANPort();

    timer_tick = new QTimer();
    connect( timer_tick, SIGNAL(timeout()), this, SLOT(onTimer_Tick()));

    connect(but_send,SIGNAL(clicked()),this,SLOT(onButSendClicked()));

    connect(but_distance,SIGNAL(clicked()),this,SLOT(changeToLum()));
    connect(but_luminosite,SIGNAL(clicked()),this,SLOT(changeToDist()));


    timer_tick -> start(2); // in ms
    
    /*// Set the window size
    int w=600;
    int h=800;
    this->resize(w,h);
    this->setWindowTitle("Object viewer");

    // Create a layout in the main window
    centralWidget = new QWidget(this);
    gridLayoutWidget = new QWidget(centralWidget);
    gridLayoutWidget->setGeometry(QRect(0, 0, this->width(), this->height()));
    gridLayout = new QGridLayout(gridLayoutWidget);

    // Create the openGL display for the map
    Object_GL = new ObjectOpenGL(gridLayoutWidget);
    Object_GL->setObjectName(QString::fromUtf8("ObjectOpenGL"));
    Object_GL->setGeometry(QRect(0, 0, this->width(), this->height()));

    // Insert the Open Gl display into the layout
    gridLayout->addWidget(Object_GL, 0, 0, 1, 1);
    setCentralWidget(centralWidget);

    // Create the menubar
    QMenu *FileMenu = menuBar()->addMenu("&File");
    FileMenu->addSeparator();
    FileMenu->addAction("Quit", qApp, SLOT (quit()), QKeySequence(tr("Ctrl+Q")));

    // Add menu items
    QMenu *ViewMenu = menuBar()->addMenu("&View");
    ViewMenu->addAction("Front view",       Object_GL, SLOT (FrontView()),  QKeySequence(tr("Ctrl+f")));
    ViewMenu->addAction("Rear view",        Object_GL, SLOT (RearView()),   QKeySequence(tr("Ctrl+e")));
    ViewMenu->addAction("Left view",        Object_GL, SLOT (LeftView()),   QKeySequence(tr("Ctrl+l")));
    ViewMenu->addAction("Right view",       Object_GL, SLOT (RightView()),  QKeySequence(tr("Ctrl+r")));
    ViewMenu->addAction("Top view",         Object_GL, SLOT (TopView()),    QKeySequence(tr("Ctrl+t")));
    ViewMenu->addAction("Bottom view",      Object_GL, SLOT (BottomView()), QKeySequence(tr("Ctrl+b")));
    FileMenu->addSeparator();
    ViewMenu->addAction("Isometric",        Object_GL, SLOT (IsometricView()), QKeySequence(tr("Ctrl+i")));

    // Timer (used for repainting the GL Window every 50 ms)
    //QTimer *timerDisplay = new QTimer();
    //timerDisplay->connect(timerDisplay, SIGNAL(timeout()),this, SLOT(onTimer_UpdateDisplay()));
    //timerDisplay->start(50);

    //


    //------------------------------------------------------------------

    openCANPort();



    timer_tick = new QTimer();
    timer_tick -> connect( timer_tick, SIGNAL(timeout()), this, SLOT(onTimer_Tick()));
    timer_tick -> start(1); // in ms


    timer_updateAffichage = new QTimer();
	timer_updateAffichage ->  connect( timer_updateAffichage, SIGNAL(timeout()), this, SLOT(onTimer_UpdateDisplay()));
    timer_updateAffichage -> start(50); // in ms
	*/

}
//===============================================================

MainWindow::~MainWindow()
{
    delete ui;
}

//===============================================================
void MainWindow::openCANPort()
{
    const char *szDevNode = DEFAULT_NODE;

    h=LINUX_CAN_Open(szDevNode, O_RDWR);
    if (!h)
        {
            printf("can't open %s\n", szDevNode);
        }
    CAN_Init(h, CAN_BAUD_500K,  CAN_INIT_TYPE_ST);         // BTR0BTR1	bitrate code in hex (default=0x1c (500 kbps))
    CAN_Status(h); // Clear Status

}
//===============================================================
void MainWindow::onButSendClicked()
{
    sendCANMessage();
}
//===============================================================
void MainWindow::sendCANMessage()
{
    TPCANMsg msgBuff;

    msgBuff.ID      =   0x2;
    msgBuff.MSGTYPE =   MSGTYPE_STANDARD;
    msgBuff.LEN     =   8;

    msgBuff.DATA[0] =   send_box_0->text().toInt();
    qDebug() << send_box_0->text();
    qDebug() << send_box_0->text().toInt();
    msgBuff.DATA[1] =   send_box_1->text().toInt();
    msgBuff.DATA[2] =   send_box_2->text().toInt();
    msgBuff.DATA[3] =   send_box_3->text().toInt();
    msgBuff.DATA[4] =   send_box_4->text().toInt();
    msgBuff.DATA[5] =   send_box_5->text().toInt();
    msgBuff.DATA[6] =   send_box_6->text().toInt();
    msgBuff.DATA[7] =   send_box_7->text().toInt();

    LINUX_CAN_Write_Timeout(h, &msgBuff,0);
}
//===============================================================
void MainWindow::sendCANMessage(char data)
{
    TPCANMsg msgBuff;

    msgBuff.ID      =   0x2 ;
    msgBuff.MSGTYPE =   MSGTYPE_STANDARD;
    msgBuff.LEN     =   1;
    msgBuff.DATA[0] =   data;
    LINUX_CAN_Write_Timeout(h, &msgBuff,0);
}
//===============================================================
void MainWindow::receiveCANMessage()
{
    LINUX_CAN_Read_Timeout(h, &pMsgBuff,1); // Timeout=1ms --> Non Bloquant
    int ID = pMsgBuff.Msg.ID;
    char capt = pMsgBuff.Msg.DATA[0];
    switch(ID){
        case 1:{
            //but_anemo->setCheckState(Qt::Checked);
            but_anemo->setChecked(true);
            receive_box_V_0->setText(QString::number((uint)pMsgBuff.Msg.DATA[0]));
            receive_box_V_1->setText(QString::number((uint)pMsgBuff.Msg.DATA[1]));
            receive_box_V_2->setText(QString::number((uint)pMsgBuff.Msg.DATA[2]));
            receive_box_V_3->setText(QString::number((uint)pMsgBuff.Msg.DATA[3]));
            receive_box_V_4->setText(QString::number((uint)pMsgBuff.Msg.DATA[4]));
            receive_box_V_5->setText(QString::number((uint)pMsgBuff.Msg.DATA[5]));
            receive_box_V_6->setText(QString::number((uint)pMsgBuff.Msg.DATA[6]));
            receive_box_V_7->setText(QString::number((uint)pMsgBuff.Msg.DATA[7]));
            //receive_box_V_4->setText("0");
            //receive_box_V_5->setText("0");
            //receive_box_V_6->setText("0");
            //receive_box_V_7->setText("0");
            dial->setValue((uint)pMsgBuff.Msg.DATA[0]);
            LCD_vit->display(QString::number((uint)pMsgBuff.Msg.DATA[0]));

            //debuger.display(pMsgBuff.Msg.DATA[7]);

            //but_pression->setCheckState(Qt::Checked);
            but_pression->setChecked(true);
            receive_box_P_0->setText(QString::number((uint)pMsgBuff.Msg.DATA[0]));
            receive_box_P_1->setText(QString::number((uint)pMsgBuff.Msg.DATA[1]));
            receive_box_P_2->setText(QString::number((uint)pMsgBuff.Msg.DATA[2]));
            receive_box_P_3->setText(QString::number((uint)pMsgBuff.Msg.DATA[3]));
            receive_box_P_4->setText(QString::number((uint)pMsgBuff.Msg.DATA[4]));
            receive_box_P_5->setText(QString::number((uint)pMsgBuff.Msg.DATA[5]));
            receive_box_P_6->setText(QString::number((uint)pMsgBuff.Msg.DATA[6]));
            receive_box_P_7->setText(QString::number((uint)pMsgBuff.Msg.DATA[7]));
            //receive_box_P_0->setText(QString::number((uint)pMsgBuff.Msg.DATA[4]));
            //receive_box_P_1->setText(QString::number((uint)pMsgBuff.Msg.DATA[5]));
            //receive_box_P_2->setText(QString::number((uint)pMsgBuff.Msg.DATA[6]));
            //receive_box_P_3->setText(QString::number((uint)pMsgBuff.Msg.DATA[7]));
            //receive_box_P_4->setText("0");
            //receive_box_P_5->setText("0");
            //receive_box_P_6->setText("0");
            //receive_box_P_7->setText("0");

            qreal pression = (uint)pMsgBuff.Msg.DATA[5] + (float)((uint)pMsgBuff.Msg.DATA[4] / 100 );
            qreal percent = 100-(100 * (pressionBar->maximum()-pression) / (pressionBar->maximum()-pressionBar->minimum()) );

            pressionBar->setFormat(QString::number((uint)pMsgBuff.Msg.DATA[5])+","+QString::number((uint)pMsgBuff.Msg.DATA[4])+" kPa ("+ QString::number(percent)+ "%)");

            pressionBar->setValue((uint)pMsgBuff.Msg.DATA[5]);
            pressionBar->setTextVisible(true);

            LCD_pres->display((double)pression);

            break;
        }
        case 2:{
            //switch(capt){
                //case 'D':{
                but_distance->setChecked(true);
                but_luminosite->setCheckable(true);
                but_luminosite->setChecked(false);
                receive_box_D_0->setText(QString::number((uint)pMsgBuff.Msg.DATA[0]));
                receive_box_D_1->setText(QString::number((uint)pMsgBuff.Msg.DATA[1]));
                receive_box_D_2->setText(QString::number((uint)pMsgBuff.Msg.DATA[2]));
                receive_box_D_3->setText(QString::number((uint)pMsgBuff.Msg.DATA[3]));
                receive_box_D_4->setText(QString::number((uint)pMsgBuff.Msg.DATA[4]));
                //receive_box_D_5->setText(QString::number((uint)pMsgBuff.Msg.DATA[5]));
                //receive_box_D_6->setText(QString::number((uint)pMsgBuff.Msg.DATA[6]));
                //receive_box_D_7->setText(QString::number((uint)pMsgBuff.Msg.DATA[7]));
                receive_box_D_5->setText("0");
                receive_box_D_6->setText("0");
                receive_box_D_7->setText("0");
                int result= (pMsgBuff.Msg.DATA[4]<<24 | pMsgBuff.Msg.DATA[3]<<16 | pMsgBuff.Msg.DATA[2]<<8 | pMsgBuff.Msg.DATA[1]);//pMsgBuff.Msg.DATA[1]<<8 | pMsgBuff.Msg.DATA[0];
                LCD_dist->display(result);
                slider_range->setValue(result);
               // break;
               // }
                //case 'L':{
                but_distance->setCheckable(true);
                but_distance->setChecked(false);
                but_luminosite->setChecked(true);
                receive_box_L_0->setText(QString::number((uint)pMsgBuff.Msg.DATA[0]));
                receive_box_L_1->setText(QString::number((uint)pMsgBuff.Msg.DATA[1]));
                receive_box_L_2->setText(QString::number((uint)pMsgBuff.Msg.DATA[2]));
                receive_box_L_3->setText(QString::number((uint)pMsgBuff.Msg.DATA[3]));
                receive_box_L_4->setText(QString::number((uint)pMsgBuff.Msg.DATA[4]));
                //receive_box_L_5->setText(QString::number((uint)pMsgBuff.Msg.DATA[5]));
                //receive_box_L_6->setText(QString::number((uint)pMsgBuff.Msg.DATA[6]));
                //receive_box_L_7->setText(QString::number((uint)pMsgBuff.Msg.DATA[7]));
                receive_box_L_5->setText("0");
                receive_box_L_6->setText("0");
                receive_box_L_7->setText("0");
                scene->actualiser(pMsgBuff.Msg.DATA[1],pMsgBuff.Msg.DATA[0]);
                int result1 = (pMsgBuff.Msg.DATA[4]<<24 | pMsgBuff.Msg.DATA[3]<<16 | pMsgBuff.Msg.DATA[2]<<8 | pMsgBuff.Msg.DATA[1]);//(pMsgBuff.Msg.DATA[1]<<8) + pMsgBuff.Msg.DATA[0];
                LCD_lum->display(result1);
                //break;
                //}
            //}
            break;
        }
        case 17:{
            but_acc->setCheckable(true);
            receive_box_G_0->setText(QString::number((uint)pMsgBuff.Msg.DATA[0]));
            receive_box_G_1->setText(QString::number((uint)pMsgBuff.Msg.DATA[1]));
            receive_box_G_2->setText(QString::number((uint)pMsgBuff.Msg.DATA[2]));
            receive_box_G_3->setText(QString::number((uint)pMsgBuff.Msg.DATA[3]));
            receive_box_G_4->setText(QString::number((uint)pMsgBuff.Msg.DATA[4]));
            receive_box_G_5->setText(QString::number((uint)pMsgBuff.Msg.DATA[5]));
            receive_box_G_6->setText(QString::number((uint)pMsgBuff.Msg.DATA[6]));
            receive_box_G_7->setText(QString::number((uint)pMsgBuff.Msg.DATA[7]));

            short Phi = ((short)pMsgBuff.Msg.DATA[3] << 8) | (short)pMsgBuff.Msg.DATA[2];
            short Theta = ((short)pMsgBuff.Msg.DATA[5] << 8) | (short)pMsgBuff.Msg.DATA[4] ;
            short Psi = ((short)pMsgBuff.Msg.DATA[7] << 8) | (short)pMsgBuff.Msg.DATA[6];

            receive_phi->setText(QString::number(Phi));
            receive_psi->setText(QString::number(Psi));
            receive_theta->setText(QString::number(Theta));

            break;
        }
    }

}
//===============================================================

void MainWindow::onTimer_Tick()
{
receiveCANMessage();
}

void MainWindow::changeToLum()
{
    /*
    if(but_luminosite->checkState() == Qt::Checked && but_distance->checkState() == Qt::Checked){
        qDebug() << "change dist to lum\n";

        //but_distance->setCheckable(false);
        but_distance->setCheckState(Qt::Checked);
        but_luminosite->setCheckable(true);
        but_luminosite->setCheckState(Qt::Unchecked);  
    }
    */
    but_distance->setChecked(false);
    but_luminosite->setChecked(true);
    sendCANMessage('D');
}

void MainWindow::changeToDist(){
    /*
    if(but_distance->checkState() == Qt::Checked && but_luminosite->checkState() == Qt::Checked){
        qDebug() << "change lum to dist\n";

        but_distance->setCheckable(true);
        but_distance->setCheckState(Qt::Unchecked);
        //but_luminosite->setCheckable(false);
        but_luminosite->setCheckState(Qt::Checked);

    }
    */
    but_distance->setChecked(true);
    but_luminosite->setChecked(false);
    sendCANMessage('L');
}



