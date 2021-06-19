#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QStyledItemDelegate>
#include <QGraphicsDropShadowEffect>

//#include <QApplication> // only for the timer
//#include <QtGui>
//#include <qobject.h>


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    loading(new LoadingWindow()),
    login(new LoginWindow()),
    client(new Client(this))
{
    //connect to server
    connect(client,&Client::connected, this,&MainWindow::connected);
    connect(client,&Client::serverError, this,&MainWindow::serverError);
    //login
    connect(login,&LoginWindow::attemptLogin, client,&Client::attemptLogin);
    connect(login,&LoginWindow::attemptSignup, client,&Client::attemptSignup);
    connect(client,&Client::loggedIn, this,&MainWindow::loggedIn);
    connect(client,&Client::loginError, this,&MainWindow::loginError);
    connect(client,&Client::informationRecieved, this,&MainWindow::startApplication);



    //initialization
    ui->setupUi(this);
    this->hide();
    login->hide();
    loading->show();
    //title bar buttons
    connect(ui->close_butt, SIGNAL(clicked()), SLOT(close()));
    connect(ui->minimize_butt, SIGNAL(clicked()), SLOT(showMinimized()));

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setColor(QColor(0,0,0,120));
    shadowEffect->setBlurRadius(25);
    shadowEffect->setOffset(3, 3);
    (ui->user_pic)->setGraphicsEffect(shadowEffect);

    QGraphicsDropShadowEffect* shadowEffect1 = new QGraphicsDropShadowEffect(this);
    shadowEffect1->setColor(QColor(0,0,0,120));
    shadowEffect1->setBlurRadius(25);
    shadowEffect1->setOffset(0, 3);
    (ui->new_conversation)->setGraphicsEffect(shadowEffect1);

    QGraphicsDropShadowEffect* shadowEffect2 = new QGraphicsDropShadowEffect(this);
    shadowEffect2->setColor(QColor(0,0,0,120));
    shadowEffect2->setBlurRadius(25);
    shadowEffect2->setOffset(0, 3);
    (ui->dictionary_2)->setGraphicsEffect(shadowEffect2);

    QGraphicsDropShadowEffect* shadowEffect3 = new QGraphicsDropShadowEffect(this);
    shadowEffect3->setColor(QColor(0,0,0,120));
    shadowEffect3->setBlurRadius(25);
    shadowEffect3->setOffset(0, 3);
    (ui->widget)->setGraphicsEffect(shadowEffect3);

    QGraphicsDropShadowEffect* shadowEffect4 = new QGraphicsDropShadowEffect(this);
    shadowEffect4->setColor(QColor(0,0,0,120));
    shadowEffect4->setBlurRadius(25);
    shadowEffect4->setOffset(0, 3);
    (ui->stackedWidget)->setGraphicsEffect(shadowEffect4);

    QPalette palette3 = (ui->username_enter)->palette();
    palette3.setColor(QPalette::PlaceholderText, QColor(255,255,255,255));
    (ui->username_enter)->setPalette(palette3);

    QPalette palette4 = (ui->textEdit)->palette();
    palette4.setColor(QPalette::PlaceholderText, QColor(255,255,255,255));
    (ui->textEdit)->setPalette(palette4);

    QPalette palette5 = (ui->enter_email)->palette();
    palette5.setColor(QPalette::PlaceholderText, QColor(255,255,255,255));
    (ui->enter_email)->setPalette(palette5);


    ui->pro_comboBox->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
    ui->pro_comboBox->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    ui->language_1st_com->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
    ui->language_1st_com->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    ui->language_2nd_com->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
    ui->language_2nd_com->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    ui->language_3rd_com->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
    ui->language_3rd_com->view()->window()->setAttribute(Qt::WA_TranslucentBackground);



    client->connectToServer();
    for(int i = 0; i < ui->user_list->count(); ++i)
    {
        QListWidgetItem* item = ui->user_list->item(i);
        item->setTextAlignment(Qt::AlignCenter);
    }

}

MainWindow::~MainWindow()
{
    delete loading;
    delete login;
    delete ui;
}


void MainWindow::startApplication(){
    loading->hide();
    this->show();
    //put all info on screen

}


void MainWindow::loginError(const QString &reason){
    login->displayError(reason);
}

void MainWindow::loggedIn(){
    login->hide();
    loading->show();
}

void MainWindow::connected(){
    loading->hide();
    login->show();
}

void MainWindow::serverError(QAbstractSocket::SocketError socketError){
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(this, tr("Error"), tr("The host refused the connection"));
        break;
    case QAbstractSocket::RemoteHostClosedError:
        QMessageBox::critical(this, tr("Error"), tr("The host closed the connection"));
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        QMessageBox::critical(this, tr("Error"), tr("The proxy refused the connection"));
        break;
    case QAbstractSocket::ProxyNotFoundError:
        QMessageBox::critical(this, tr("Error"), tr("Could not find the proxy"));
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::critical(this, tr("Error"), tr("Could not find the server"));
        break;
    case QAbstractSocket::SocketAccessError:
        QMessageBox::critical(this, tr("Error"), tr("You don't have permissions to execute this operation"));
        break;
    case QAbstractSocket::SocketResourceError:
        QMessageBox::critical(this, tr("Error"), tr("Too many connections opened"));
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        QMessageBox::critical(this, tr("Error"), tr("Proxy timed out"));
        break;
    case QAbstractSocket::NetworkError:
        QMessageBox::critical(this, tr("Error"), tr("Unable to reach the network"));
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        QMessageBox::critical(this, tr("Error"), tr("Operation not supported"));
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        QMessageBox::critical(this, tr("Error"), tr("Your proxy requires authentication"));
        break;
    case QAbstractSocket::ProxyProtocolError:
        QMessageBox::critical(this, tr("Error"), tr("Proxy comunication failed"));
        break;
    case QAbstractSocket::SocketTimeoutError:
        QMessageBox::warning(this, tr("Error"), tr("Operation timed out"));
        return;
    case QAbstractSocket::TemporaryError:
        QMessageBox::warning(this, tr("Error"), tr("A temporary error occured"));
        return;
    case QAbstractSocket::OperationError:
        QMessageBox::warning(this, tr("Error"), tr("Operation failed, please try again"));
        return;
    default:
        QMessageBox::critical(this, tr("Error"), tr("An unknown error occured"));
    }
    loading->hide();
    login->hide();
    this->close();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
    {
        //commented out because we have other variables for resizing!
//        if (event->button() == Qt::LeftButton) {
//            m_startPoint = frameGeometry().topLeft() - event->globalPos();
//        }

        //From Qt Documentation:
        //Reason why pos() wasn't working is because the global
        //position at time of event may be very different
        //This is why the mpos = event->pos(); line before was
        //possibly causing jumping behavior

         if (event->button() == Qt::LeftButton){
             //Coordinates have been mapped such that the mouse position is relative to the
             //upper left of the main window
             mpos = event->globalPos() - frameGeometry().topLeft();

             //At the moment of mouse click, capture global position and
             //lock the size of window for resizing
             global_mpos = event->globalPos();
             storeWidth = this->width();
             storeHeight= this->height();

             event->accept();

         }
    }

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //this->move(event->globalPos() + m_startPoint);

    //mapped mouse relative to upper left of window
    rs_mpos=event->globalPos()-frameGeometry().topLeft();//general position tracker for resizing
    QTextStream out(stdout);

    //How much of the corner is considered a "resizing zone"
    //I was experiencing jumping behavior with rs_size is 10 so
    //I recommend rs_size=50
    int rs_size=50;

    //Big if statement checks if your mouse is in the upper left,
    //upper right, lower left, and lower right
    bool resizeZone = (abs(rs_mpos.x()) < rs_size && abs(rs_mpos.y()) < rs_size) ||
            (abs(rs_mpos.x()) > this->width()-rs_size && abs(rs_mpos.y()) <rs_size) ||
            (abs(rs_mpos.x()) < rs_size && abs(rs_mpos.y())> this->height()-rs_size) ||
            (abs(rs_mpos.x()) > this->width()-rs_size && abs(rs_mpos.y())> this->height()-rs_size);
    if (resizeZone){
        this->setCursor(Qt::SizeBDiagCursor);
         //Below for debugging
         /*
         out << rs_mpos.x() << " , " << rs_mpos.y() << "\n";
         out << "window: " << this->width() << " , " << this->height() << "\n";
         out << "globalpos: " << event->globalPos().x() << " , "
             << event->globalPos().y() << "\n";
        */

        //Use 2x2 matrix to adjust how much you are resizing and how much you
        //are moving. Since the default coordinates are relative to upper left
        //You cannot just have one way of resizing and moving the window.
        //It will depend on which corner you are referring to

        //adjXfac and adjYfac are for calculating the difference between your
        //current mouse position and where your mouse was when you clicked.
        //With respect to the upper left corner, moving your mouse to the right
        //is an increase in coordinates, moving mouse to the bottom is increase
        //etc.
        //However, with other corners this is not so and since I chose to subtract
        //This difference at the end for resizing, adjXfac and adjYfac should be
        //1 or -1 depending on whether moving the mouse in the x or y directions
        //increases or decreases the coordinates respectively.

        //transXfac transYfac is to move the window over. Resizing the window does not
        //automatically pull the window back toward your mouse. This is what
        //transfac is for (translate window in some direction). It will be either
        //0 or 1 depending on whether you need to translate in that direction.

        //Initiate matrix
        int adjXfac=0;
        int adjYfac=0;
        int transXfac=0;
        int transYfac=0;

        //Upper left corner section
        if ( (abs(rs_mpos.x()) < rs_size && abs(rs_mpos.y()) < rs_size)){
        this->setCursor(Qt::SizeFDiagCursor);



             //Upper left. No flipping of axis, no translating window
             adjXfac=1;
             adjYfac=1;

             transXfac=0;
             transYfac=0;



        }
        //Upper right corner section
        else if(abs(rs_mpos.x()) > this->width()-rs_size &&
                abs(rs_mpos.y()) <rs_size){
            this->setCursor(Qt::SizeBDiagCursor);


            //upper right. Flip displacements in mouse movement across x axis
            //and translate window left toward the mouse
            adjXfac=-1;
            adjYfac=1;

            transXfac = 1;
            transYfac =0;

         }

         //Lower left corner section
         else if(abs(rs_mpos.x()) < rs_size &&
                 abs(rs_mpos.y())> this->height()-rs_size){
            this->setCursor(Qt::SizeBDiagCursor);

            //lower left. Flip displacements in mouse movement across y axis
            //and translate window up toward mouse
            adjXfac=1;
            adjYfac=-1;

            transXfac=0;
            transYfac=1;


          }
          //Lower right corner section
          else if(abs(rs_mpos.x()) > this->width()-rs_size &&
                  abs(rs_mpos.y())> this->height()-rs_size){
              this->setCursor(Qt::SizeFDiagCursor);

             //lower right. Flip mouse displacements on both axis and
             //translate in both x and y direction left and up toward mouse.
             adjXfac=-1;
             adjYfac=-1;

             transXfac=1;
             transYfac=1;
            }

       if (event->buttons()==Qt::LeftButton ){

           //Calculation of displacement. adjXfac=1 means normal displacement
           //adjXfac=-1 means flip over axis
           int adjXdiff = adjXfac*(event->globalPos().x() - global_mpos.x());

           int adjYdiff = adjYfac*(event->globalPos().y() - global_mpos.y());

           //if transfac is 1 then movepoint of mouse is translated
           QPoint movePoint(mpos.x() - transXfac*adjXdiff, mpos.y()-transYfac*adjYdiff);
           move(event->globalPos()-movePoint);
           resize(storeWidth-adjXdiff, storeHeight-adjYdiff);

           event->accept();


         }

    }

     //in any move event if it is not in a resize region use the default cursor
     else{

         this->setCursor(Qt::ArrowCursor);


          //simple move section
          if (event->buttons()==Qt::LeftButton &&
              resizeZone==false){
              move(event->globalPos() - mpos);
              event->accept();
          }


     }

}


void MainWindow::on_settingbutton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Profile);
}

void MainWindow::on_dictionary_2_clicked(){
   ui->stackedWidget->setCurrentWidget(ui->dictionary);
}


void MainWindow::on_signout_clicked()
{
   this->hide();
   login->show();
}

