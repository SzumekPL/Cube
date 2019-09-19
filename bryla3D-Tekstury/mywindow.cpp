#include "mywindow.h"
#include <cmath>
#include <QDebug>
#include "ui_mywindow.h"
#include <iostream>

using namespace std;

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{

    ui->setupUi(this);

    szer = ui->Frame->width();
    wys = ui->Frame->height();
    poczX = ui->Frame->x();
    poczY = ui->Frame->y();

    img = new QImage(szer,wys,QImage::Format_RGB32);
    copy = new QImage(szer,wys,QImage::Format_RGB32);

    downSide  = new QImage(":/down.jpg");    //down
    backSide  = new QImage(":/back.jpg");    //back
    leftSide  = new QImage(":/left.jpg");    //left
    frontSide = new QImage(":/front.jpg");   //front
    upSide    = new QImage(":/up.jpg");      //up
    rightSide = new QImage(":/right.jpg");   //right

    czysc();
    wykonaj();
}

// Definicja destruktora
MyWindow::~MyWindow()
{
    delete ui;
}



// Funkcja "odmalowujaca" komponent
void MyWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(poczX,poczY,*img);
}

void MyWindow::czysc()
{
    unsigned char *ptr;
    ptr = img->bits();
    int i,j;
    for(i=0; i<wys; i++)
    {
        for(j=0; j<szer; j++)
        {
            ptr[szer*4*i + 4*j] = 0; // B
            ptr[szer*4*i + 4*j + 1] = 0; // G
            ptr[szer*4*i + 4*j + 2] = 0; // R
        }
    }
}

void MyWindow::rysujPiksel(int x, int y, int r, int g, int b)
{
    unsigned char *ptr;
    ptr = img->bits();
    if(x >= 0 && y >= 0 && x < szer && y < wys){
        ptr[szer * 4 * y + 4 * x] = uchar(b);
        ptr[szer * 4 * y + 4 * x + 1] = uchar(g);
        ptr[szer * 4 * y + 4 * x + 2] = uchar(r);
    }
}

void MyWindow::wykonaj()
{
    czysc();
    Cube kostka(250,250,-100,100);
    kostka.rotate(obrotOX,obrotOY,obrotOZ);
    kostka.slope(pochylenieOX,pochylenieOY,pochylenieOZ);
    kostka.scale(skalowanieOX,skalowanieOY,skalowanieOZ);
    kostka.move(przesuniecieOX,przesuniecieOY,przesuniecieOZ);

    if(kostka.checkFront())
    {
        pair<int,int> *punkt = kostka.Front();
        Teksturowanie(make_pair(0,0),make_pair(500,0),make_pair(0,500),punkt[0],punkt[1],punkt[2],frontSide->bits());
        Teksturowanie(make_pair(0,500),make_pair(500,500),make_pair(500,0),punkt[2],punkt[3],punkt[1],frontSide->bits());
    }

    if(kostka.checkUp())
    {
        pair<int,int> *punkt = kostka.Up();
        Teksturowanie(make_pair(0,0),make_pair(500,0),make_pair(0,500),punkt[0],punkt[1],punkt[2],upSide->bits());
        Teksturowanie(make_pair(0,500),make_pair(500,500),make_pair(500,0),punkt[2],punkt[3],punkt[1],upSide->bits());
    }

    if(kostka.checkDown())
    {
        pair<int,int> *punkt = kostka.Down();
        Teksturowanie(make_pair(0,0),make_pair(500,0),make_pair(0,500),punkt[0],punkt[1],punkt[2],downSide->bits());
        Teksturowanie(make_pair(0,500),make_pair(500,500),make_pair(500,0),punkt[2],punkt[3],punkt[1],downSide->bits());
    }

    if(kostka.checkLeft())
    {
        pair<int,int> *punkt = kostka.Left();
        Teksturowanie(make_pair(0,0),make_pair(500,0),make_pair(0,500),punkt[0],punkt[1],punkt[2],leftSide->bits());
        Teksturowanie(make_pair(0,500),make_pair(500,500),make_pair(500,0),punkt[2],punkt[3],punkt[1],leftSide->bits());
    }

    if(kostka.checkRight())
    {
        pair<int,int> *punkt = kostka.Right();
        Teksturowanie(make_pair(0,0),make_pair(500,0),make_pair(0,500),punkt[0],punkt[1],punkt[2],rightSide->bits());
        Teksturowanie(make_pair(0,500),make_pair(500,500),make_pair(500,0),punkt[2],punkt[3],punkt[1],rightSide->bits());
    }

    if(kostka.checkBack())
    {
        pair<int,int> *punkt = kostka.Back();
        Teksturowanie(make_pair(0,0),make_pair(500,0),make_pair(0,500),punkt[0],punkt[1],punkt[2],backSide->bits());
        Teksturowanie(make_pair(0,500),make_pair(500,500),make_pair(500,0),punkt[2],punkt[3],punkt[1],backSide->bits());
    }

    update();

}

void MyWindow::prosta(int startx, int starty, int koniecx, int koniecy,int r, int g, int c){
    double a, b;
    int pom;
    int i;
    if(abs(startx - koniecx) > abs(starty - koniecy)){
        if(startx > koniecx){
            pom = startx;
            startx = koniecx;
            koniecx = pom;
            pom = starty;
            starty = koniecy;
            koniecy = pom;
        }
        a = (koniecy - starty) / double(koniecx - startx);
        b = starty - a * startx;
        for(i = startx; i <= koniecx; i++){
            pom = int(a * i + b) ;
            rysujPiksel(i,pom,r,g,c);
        }
    } else {
        if(starty > koniecy){
            pom = startx;
            startx = koniecx;
            koniecx = pom;
            pom = starty;
            starty = koniecy;
            koniecy = pom;
        }
        a = (koniecx - startx) / double(koniecy - starty);
        b = startx - a * starty;
        for(i = starty; i <= koniecy; i++){
            pom = int(a * i + b);
            rysujPiksel(pom,i,r,g,c);
        }
    }
}

void MyWindow::on_horizontalSlider_4_valueChanged(int value)
{
    skalowanieOX = value / 250.0;
    wykonaj();
}

void MyWindow::on_horizontalSlider_5_valueChanged(int value)
{
    skalowanieOY = value / 250.0;
    wykonaj();
}

void MyWindow::on_horizontalSlider_6_valueChanged(int value)
{
    skalowanieOZ = value / 250.0;
    wykonaj();
}

void MyWindow::on_dial_valueChanged(int value)
{
    obrotOX = value / 180.0 * M_PI;
    wykonaj();
}

void MyWindow::on_dial_2_valueChanged(int value)
{
    obrotOY = value / 180.0 * M_PI;
    wykonaj();
}

void MyWindow::on_dial_3_valueChanged(int value)
{
    obrotOZ = value / 180.0 * M_PI;
    wykonaj();
}

void MyWindow::on_horizontalSlider_7_valueChanged(int value)
{
    pochylenieOX = 1 - (value/250.0);
    wykonaj();
}

void MyWindow::on_horizontalSlider_8_valueChanged(int value)
{
    pochylenieOY = 1 - (value/250.0);
    wykonaj();
}

void MyWindow::on_horizontalSlider_9_valueChanged(int value)
{
    pochylenieOZ = 1 - (value/250.0);
    wykonaj();
}
void MyWindow::on_horizontalSlider_valueChanged(int value)
{
    przesuniecieOX = value - 250;
    wykonaj();
}

void MyWindow::on_horizontalSlider_2_valueChanged(int value)
{
    przesuniecieOY = 250 - value;
    wykonaj();
}

void MyWindow::on_horizontalSlider_3_valueChanged(int value)
{
    przesuniecieOZ = 250 - value;
    wykonaj();
}
void MyWindow::on_pushButton_clicked()
{

    przesuniecieOX = 0;
    przesuniecieOY = 0;
    przesuniecieOZ = 0;
    skalowanieOX = 1;
    skalowanieOY = 1;
    skalowanieOZ = 1;
    pochylenieOX = 0;
    pochylenieOY = 0;
    pochylenieOZ = 0;
    obrotOX = 0;
    obrotOY = 0;
    obrotOZ = 0;
    ui->horizontalSlider->setValue(250);
    ui->horizontalSlider_2->setValue(250);
    ui->horizontalSlider_3->setValue(250);
    ui->horizontalSlider_4->setValue(250);
    ui->horizontalSlider_5->setValue(250);
    ui->horizontalSlider_6->setValue(250);
    ui->horizontalSlider_7->setValue(250);
    ui->horizontalSlider_8->setValue(250);
    ui->horizontalSlider_9->setValue(250);
    ui->dial->setValue(0);
    ui->dial_2->setValue(0);
    ui->dial_3->setValue(0);
    wykonaj();
}

double MyWindow::Interpolacja(int x, int y, double dx)
{
    return dx*y+(1-dx)*x;
}
void MyWindow::Teksturowanie(pair<int,int> A, pair<int,int> B, pair<int,int> C, pair<int,int> At, pair<int,int> Bt, pair<int,int> Ct ,unsigned char *org){

    int Xmin,Xmax,Ymin,Ymax;
    double a,b,v,w,u,delta;
    int i,j,x,y,xn,yn;
    int red,green,blue;
    Xmin = Xmax = At.first;
    Ymin = Ymax = At.second;
    if(Bt.first < Xmin){
        Xmin = Bt.first;
    }
    if(Ct.first < Xmin){
        Xmin = Ct.first;
    }
    if(Bt.first > Xmax){
        Xmax = Bt.first;
    }
    if(Ct.first > Xmax){
        Xmax = Ct.first;
    }
    if(Bt.second < Ymin){
        Ymin = Bt.second;
    }
    if(Ct.second < Ymin){
        Ymin = Ct.second;
    }
    if(Bt.second > Ymax){
        Ymax = Bt.second;
    }
    if(Ct.second > Ymax){
        Ymax = Ct.second;
    }
    delta = (((Bt.first - At.first) * (Ct.second - At.second)) - ((Ct.first - At.first) * (Bt.second - At.second)));
    for(i=Ymin; i<=Ymax; i++)
    {
        for(j=Xmin; j<=Xmax; j++)
        {
            v = (((j - At.first) * (Ct.second - At.second)) - ((Ct.first - At.first) * (i - At.second))) / delta;
            w = (((Bt.first - At.first) * (i - At.second)) - ((j - At.first) * (Bt.second - At.second))) / delta;
            u = 1 - v - w;
            if (u > 0 && u < 1 && v > 0 && v < 1 && w > 0 && w < 1){
               a = double(u * A.first + v * B.first + w * C.first);
               b = double(u * A.second + v * B.second + w * C.second);
               x = int(a);
               y = int(b);
               xn = x + 1;
               yn = y + 1;
               a -=x;
               b -=y;
               if(yn == wys)
                   yn--;
               if(xn == szer)
                   xn--;
               red = int(round(Interpolacja(int(Interpolacja(org[szer*4*y + 4*x + 2],org[szer*4*yn + 4*x + 2],b)),int(Interpolacja(org[szer*4*y + 4*xn + 2],org[szer*4*yn + 4*xn + 2],b)),a)));
               green = int(round(Interpolacja(int(Interpolacja(org[szer*4*y + 4*x + 1],org[szer*4*yn + 4*x + 1],b)),int(Interpolacja(org[szer*4*y + 4*xn + 1],org[szer*4*yn + 4*xn + 1],b)),a)));
               blue = int(round(Interpolacja(int(Interpolacja(org[szer*4*y + 4*x],org[szer*4*yn + 4*x],b)),int(Interpolacja(org[szer*4*y + 4*xn],org[szer*4*yn + 4*xn],b)),a)));

               rysujPiksel(j,i,red,green,blue);
            }
        }
    }
}
