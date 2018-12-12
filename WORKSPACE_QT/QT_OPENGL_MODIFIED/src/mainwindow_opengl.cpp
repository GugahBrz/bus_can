#include "mainwindow.h"
#include "objectgl.h"


#define DEFAULT_NODE "/dev/pcanusb32"
#define ID_1    0x01
#define ID_2    0x02
#define ID_3    0x03
#define ID_4    0x04
#define ID_MPU  0x11
#define O_RDWR		     02
//=============================================================================================
// Constructor of the main window
// Create window properties, menu etc ...
//=============================================================================================
MainWindow::MainWindow(QWidget *parent,int w, int h)
    : QMainWindow(parent)
{        
    // Set the window size
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





}
//=============================================================================================
MainWindow::~MainWindow()
{}
//=============================================================================================
// On resize event, the items in the window are resized
//=============================================================================================
void MainWindow::resizeEvent(QResizeEvent *)
{
    Object_GL->resize(centralWidget->width(),centralWidget->height());
    gridLayoutWidget->setGeometry(QRect(0, 0, centralWidget->width(), centralWidget->height()));
}
//=============================================================================================
// Timer event : repain the Open Gl window
//=============================================================================================
void MainWindow::onTimer_UpdateDisplay()
{
    Object_GL->updateGL();
}
//=============================================================================================


void MainWindow::openCANPort()
{
    const char *szDevNode = DEFAULT_NODE;

    h1=LINUX_CAN_Open(szDevNode, O_RDWR);
    if (!h1)
        {
            printf("can't open %s\n", szDevNode);
        }
    if (h1){printf("francoisauger %s\n", szDevNode);};
    CAN_Init(h1, CAN_BAUD_500K,  CAN_INIT_TYPE_ST);         // BTR0BTR1	bitrate code in hex (default=0x1c (500 kbps))
    CAN_Status(h1); // Clear Status

}
//===============================================================
void MainWindow::receiveCANMessage()
{

    LINUX_CAN_Read_Timeout(h1, &pMsgBuff,1); // Timeout=1ms --> Non Bloquant

double_t PHI;
double_t TETA;
double_t PSI;

//probleme: how to get the data for each sensor?
    if (pMsgBuff.Msg.ID==ID_MPU){

        //problem: merge the different data into a usable int variable
            PHI = (pMsgBuff.Msg.DATA[3]<< 8) | pMsgBuff.Msg.DATA[2];
            TETA = (pMsgBuff.Msg.DATA[5]<< 8) | pMsgBuff.Msg.DATA[4];
            PSI = (pMsgBuff.Msg.DATA[7]<< 8) | pMsgBuff.Msg.DATA[6];
            qDebug() << "C++ Style Debug Message"<< PHI;

    }

    Object_GL->setAngles(PHI, TETA, PSI);
    //if (pMsgBuff.Msg.ID==ID_2){	gyroscope_box->setText(QString::number((uint)pMsgBuff.Msg.DATA[0])+QString::number((uint)pMsgBuff.Msg.DATA[1]));}
    //if (pMsgBuff.Msg.ID==ID_3){capteur_de_distance_box->setText(QString::number((uint)pMsgBuff.Msg.DATA[0])+QString::number((uint)pMsgBuff.Msg.DATA[1]));}
    //if (pMsgBuff.Msg.ID==ID_4){anemometre_box->setText(QString::number((uint)pMsgBuff.Msg.DATA[0])+QString::number((uint)pMsgBuff.Msg.DATA[1]));}


    //------------------------------------------------------------------

}

void MainWindow::onTimer_Tick()
{
receiveCANMessage();
}

