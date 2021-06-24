#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QImage>
//#include "ft2build.h"
//#include "freetype.h"
#include <SDL.h>
#include <SDL_ttf.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    char * pstr = "中方字库打印HelloWorld,2021!";
    SDL_PixelFormat *fmt;
    TTF_Font *font;
    SDL_Surface *text, *temp;

    if (TTF_Init() < 0 )
    {
        qDebug()<< "Couldn't initialize TTF: %s\n",SDL_GetError();
        SDL_Quit();
        return ;
    }

    font = TTF_OpenFont(":/simsun.ttc", 32);
    if ( font == NULL )
    {
        qDebug()<< "Couldn't load %d pt font from %s: %s\n",18,"ptsize", SDL_GetError();
        return ;
    }

    SDL_Color forecol = { 0xff, 0xff, 0xff, 0xff };
    text = TTF_RenderUTF8_Solid(font, pstr, forecol);

    fmt = (SDL_PixelFormat*)malloc(sizeof(SDL_PixelFormat));
    memset(fmt,0,sizeof(SDL_PixelFormat));
    fmt->BitsPerPixel = 16;
    fmt->BytesPerPixel = 2;
    fmt->Rloss = 0xFF;
    fmt->Gloss = 0xFF;
    fmt->Bloss = 0xFF;
    fmt->Aloss = 0;

    temp = SDL_ConvertSurface(text,fmt,0);
    SDL_SaveBMP(temp, "/mnt/internal_sd/Z390/Save.bmp");
    SDL_FreeSurface(text);
    SDL_FreeSurface(temp);
    TTF_CloseFont(font);
    TTF_Quit();
    QPixmap  Image;
    Image.load("/mnt/internal_sd/Z390/Save.bmp");
    ui->labelPic->setPixmap(Image);
}

