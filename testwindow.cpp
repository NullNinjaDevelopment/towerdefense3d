// Opengl
#include <GL/gl.h>
#include <GL/glu.h>
#include <conio.h>//needed for getch

// Qt
#include <QtGui/QMouseEvent>
#include <QCursor>
#include <QPoint>
#include <QTimer>
#include <qglobal.h>

// STD
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>

// Local
#include "testwindow.h"
#include "gametile.h"

// CONSTANTS
const int BOARD_WIDTH = 20;
const int BOARD_HEIGHT = 20;
const int TILE_WIDTH = 2;
const int TILE_HEIGHT = 2;

float viewx = 0.0f, viewy = 0.0f;
int mouseoriginx = -1, mouseoriginy = -1;
float zoomdist = 0.0f;

float fps;

GameTile* gameboard[BOARD_WIDTH][BOARD_HEIGHT];

/* GENERIC FUNCTIONS */
void drawTiles(){
    int centerx, centery;
    GameTile* tile;
    for(int i = 0; i < BOARD_WIDTH; ++i){
        //printf("~~~~~~~~~COLUMN %d~~~~~~~~~\n", i);
        for(int j = 0; j < BOARD_HEIGHT; ++j){

            tile = gameboard[i][j];
            centerx = tile->x * TILE_WIDTH;
            centery = tile->y * TILE_HEIGHT;

            glBegin(GL_QUADS);
                glColor4f(tile->r, tile->g, tile->b, tile->a);
                //glColor3f(255, 0, 0);
                glVertex3f(-1+centerx, -1+centery, 0);
                glVertex3f(1+centerx, -1+centery, 0);
                glVertex3f(1+centerx, 1+centery, 0);
                glVertex3f(-1+centerx, 1+centery, 0);
            glEnd();
            //printf("Coloring in tile! x=%f, y=%f, red=%f, blue=%f, green=%f, alpha=%f\n", tile->x, tile->y, tile->r, tile->g, tile->b, tile->a);
        }
    }
}

void initTileMap(){
    srand(time(NULL));
    for(int i = 0; i < BOARD_WIDTH; ++i){
        printf("###############COLUMN %d###############\n", i);
        for(int j = 0; j < BOARD_HEIGHT; ++j){
            float red = (float)(rand() % 100)*0.01;
            float green = (float)(rand() % 100)*0.01;
            float blue = (float)(rand() % 100)*0.01;
            float alpha = (float)((rand() % 51) + 50)*0.01;
            gameboard[i][j] = new GameTile((float)i, (float)j, red, green, blue, alpha);
            printf("Creating tile! x=%f, y=%f, red=%f, green=%f, blue=%f, alpha=%f\n", (float)i, (float)j, red, green, blue, alpha);
        }
    }
}


std::string floatToString(float d)
 {
    std::ostringstream ss;
    ss << d;
    return ss.str();
 }

TestWindow::TestWindow(QWidget *parent) : QGLWidget(parent) {
    printf("START TestWindow()\n");
    initTileMap();

    setMouseTracking(true);

    this->setParent(parent);
    this->setGeometry(this->x(), this->y(), 800, 600);

    initializeGL();

    fps = 0;
    timer = new QTimer();

    QTimer* fpsTimer = new QTimer();
    QObject::connect(fpsTimer, SIGNAL(timeout()), this, SLOT(showFps()));
    fpsTimer->start(1000);

    this->timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(processing()));
    timer->start(10);
    printf("END TestWindow()\n");
}

void TestWindow::showFps()
{
    printf("%ffps\n", fps);
    fps = 0;
}

void TestWindow::processing()
{
    this->updateGL();
    fps++;
}

void TestWindow::initializeGL() {
    printf("START initializeGL()\n");
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    printf("END initializeGL()\n");
}

void TestWindow::resizeGL(int w, int h) {
    printf("START resizeGL()\n");
    if (h <= 0) {
        h = 1;
    }
    float height = (float) w / (float) h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(50.0f, height, 1.0, 1000.0);
    //glOrtho(-5, 5, -5, 5, -10, -20.0);
    glOrtho(-20.0,20.0,-20.0,20.0,-1.0,100.0);//default = -1.0,+1.0,-1.0,+1.0,-1.0,+1.0
    //glOrtho(left,right,bottom,top,near,far);//default = -1.0,+1.0,-1.0,+1.0,-1.0,+1.0
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    printf("END resizeGL()\n");

    // Set mouse origin values to the center point
    mouseoriginx = TestWindow::x() + (int)(TestWindow::width()*0.5f+0.5f);
    mouseoriginy = TestWindow::y() + (int)(TestWindow::height()*0.5f+0.5f);
}

float testval = 0.0f;
void TestWindow::paintGL() {
    //printf("START paintGL()\n");
    //testval += 1.0f;
    //printf("%f\n", testval);

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    //glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glPushMatrix();

    //glTranslatef(0.0f, 0.0f, -60.0f);
    glTranslatef(viewx, viewy, -60.0f);
    //glTranslatef(viewx, viewy, -zoomdist);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(45.0f,  -1.0f,  1.0f,  0.0f);

    /* */
    drawTiles();
    /* *
    glBegin(GL_QUADS);
        glColor3f(1, 0, 0);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
    glEnd();
    /* */

    //glPopMatrix();
    //printf("END paintGL()\n");
}

void TestWindow::mousePressEvent(QMouseEvent *event) {
    printf("START mousePressEvent()\n");
    switch(event->button()){
    case Qt::LeftButton:
        zoomdist += 1.0f;
        printf("--------------\n");
        printf("Left Clicky\n");
        break;
    case Qt::RightButton:
        zoomdist -= 1.0f;
        printf("--------------\n");
        printf("Right Clicky\n");
        break;
    default:
        break;
    }
    printf("END mousePressEvent()\n");
}


void TestWindow::mouseMoveEvent(QMouseEvent *event) {
    /*
    printf("###### Comparing: %d, %d and %d, %d ######\n", event->x(), event->y(), QCursor::pos().x(), QCursor::pos().y());

    std::string myOutput = "";
    myOutput += "---------------------------\n";
    myOutput += "START mouseMoveEvent()\n";
    // PRINT BEFORE DIFF POS
    myOutput += "BEFORE DIFF POS: ";
    myOutput += floatToString(0.02f*((float)QCursor::pos().x() - mouseoriginx));
    myOutput += ", ";
    myOutput += floatToString(0.02f*((float)QCursor::pos().y() - mouseoriginy));
    myOutput += "\n";
    // PRINT BEFORE VIEW POS
    myOutput += "BEFORE VIEW POS: ";
    myOutput += floatToString(viewx);
    myOutput += ", ";
    myOutput += floatToString(viewy);
    myOutput += "\n";
    */
    if (mouseoriginx < 0 || mouseoriginy < 0){
        mouseoriginx = TestWindow::x() + (int)(TestWindow::width()*0.5f+0.5f);
        mouseoriginy = TestWindow::y() + (int)(TestWindow::height()*0.5f+0.5f);
    }
    else{
        viewx = viewx - 0.02f*(float)((float)QCursor::pos().x() - mouseoriginx);
        viewy = viewy + 0.02f*(float)((float)QCursor::pos().y() - mouseoriginy);
        /*
        // PRINT AFTER DIFF POS
        myOutput += "AFTER DIFF POS: ";
        myOutput += floatToString(0.02f*((float)QCursor::pos().x() - mouseoriginx));
        myOutput += ", ";
        myOutput += floatToString(0.02f*((float)QCursor::pos().y() - mouseoriginy));
        myOutput += "\n";
        // PRINT AFTER VIEW POS
        myOutput += "AFTER VIEW POS: ";
        myOutput += floatToString(viewx);
        myOutput += ", ";
        myOutput += floatToString(viewy);
        myOutput += "\n";
        */
        QCursor::setPos(mouseoriginx, mouseoriginy);
    }
    /*
    myOutput += "END mouseMoveEvent()\n";
    myOutput += "---------------------------\n";
    //printf("END mouseMoveEvent()\n");
    //printf("%s", myOutput);
    std::cout << myOutput;
    std::cout.flush();
    */
    paintGL();
}

void TestWindow::keyPressEvent(QKeyEvent* event) {
    printf("START keyPressEvent()\n");
    switch(event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        event->ignore();
        break;
    }
    printf("END keyPressEvent()\n");
}
