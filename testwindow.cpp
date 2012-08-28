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

// Local
#include "testwindow.h"

float viewx = 0.0f, viewy = 0.0f;
int mouseoriginx = -1, mouseoriginy = -1;
float zoomdist = 0.0f;

float fps;

/* GENERIC FUNCTIONS */
void drawTiles(){
    glBegin(GL_QUADS);
        glColor3f(1,0,0);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
    glEnd();
}


std::string floatToString(float d)
 {
    std::ostringstream ss;
    ss << d;
    return ss.str();
 }

TestWindow::TestWindow(QWidget *parent) : QGLWidget(parent) {
    printf("START TestWindow()\n");
    setMouseTracking(true);

    this->setParent(parent);
    this->setGeometry(0, 0, 800, 600);

    initializeGL();

    fps = 0;
    timer = new QTimer();

    QTimer* fpsTimer = new QTimer();
    QObject::connect(fpsTimer, SIGNAL(timeout()), this, SLOT(showFps()));
    fpsTimer->start(10);

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
    gluPerspective(50.0f, height, 1.0, 1000.0);
    //glOrtho(-5, 5, -5, 5, -10, -20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    printf("END resizeGL()\n");
}

float testval = 0.0f;
void TestWindow::paintGL() {
    printf("START paintGL()\n");
    testval += 1.0f;
    printf("%f\n", testval);

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

    drawTiles();

    //glPopMatrix();
    printf("END paintGL()\n");
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
    std::string myOutput = "";
    myOutput += "---------------------------\n";
    //printf("---------------------------\n");
    myOutput += "START mouseMoveEvent()\n";
    //printf("START mouseMoveEvent()\n");
    myOutput += "AFTER EVENT POS: ";
    myOutput += floatToString(event->x());
    myOutput += ", ";
    myOutput += floatToString(event->y());
    myOutput += "\n";
    //printf("BEFORE EVENT POS: %d, %d\n", event->x(), event->y());
    myOutput += "BEFORE MOUSE ORIGIN: ";
    myOutput += floatToString(mouseoriginx);
    myOutput += ", ";
    myOutput += floatToString(mouseoriginy);
    myOutput += "\n";
    //printf("BEFORE MOUSE ORIGIN: %d, %d\n", mouseoriginx, mouseoriginy);
    myOutput += "BEFORE DIFF POS: ";
    myOutput += floatToString(0.005f*((float)event->x() - mouseoriginx));
    myOutput += ", ";
    myOutput += floatToString(0.005f*((float)event->y() + mouseoriginy));
    myOutput += "\n";
    //printf("BEFORE DIFF POS: %f, %f\n", (float)event->x() - mouseoriginx*0.5f, (float)event->y() - mouseoriginy*0.5f);
    myOutput += "BEFORE VIEW POS: ";
    myOutput += floatToString(viewx);
    myOutput += ", ";
    myOutput += floatToString(viewy);
    myOutput += "\n";
    //printf("BEFORE VIEW POS: %f, %f\n", viewx, viewy);
    if (mouseoriginx < 0 || mouseoriginy < 0){
        //printf("MOUSE ORIGIN POS: %d, %d\n", mouseoriginx, mouseoriginy);
        mouseoriginx = TestWindow::x() + (int)(TestWindow::width()*0.5f+0.5f);
        mouseoriginy = TestWindow::y() + (int)(TestWindow::height()*0.5f+0.5f);
        //printf("MOUSE ORIGIN POS: %d, %d\n", mouseoriginx, mouseoriginy);
    }
    else{
        viewx = viewx - 0.005f*(float)((float)event->x() - mouseoriginx);
        viewy = viewy + 0.005f*(float)((float)event->y() - mouseoriginy);
        myOutput += "AFTER EVENT POS: ";
        myOutput += floatToString(event->x());
        myOutput += ", ";
        myOutput += floatToString(event->y());
        myOutput += "\n";
        //printf("AFTER EVENT POS: %d, %d\n", event->x(), event->y());
        myOutput += "AFTER MOUSE ORIGIN: ";
        myOutput += floatToString(mouseoriginx);
        myOutput += ", ";
        myOutput += floatToString(mouseoriginy);
        myOutput += "\n";
        //printf("AFTER MOUSE ORIGIN: %d, %d\n", mouseoriginx, mouseoriginy);
        myOutput += "AFTER DIFF POS: ";
        myOutput += floatToString(0.005f*((float)event->x() - mouseoriginx));
        myOutput += ", ";
        myOutput += floatToString(0.005f*((float)event->y() + mouseoriginy));
        myOutput += "\n";
        //printf("AFTER DIFF POS: %f, %f\n", (float)event->x() - mouseoriginx*0.5f, (float)event->y() - mouseoriginy*0.5f);
        myOutput += "AFTER VIEW POS: ";
        myOutput += floatToString(viewx);
        myOutput += ", ";
        myOutput += floatToString(viewy);
        myOutput += "\n";
        //printf("AFTER VIEW POS: %f, %f\n", viewx, viewy);
        QCursor::setPos(mouseoriginx, mouseoriginy);
    }
    myOutput += "END mouseMoveEvent()\n";
    myOutput += "---------------------------\n";
    //printf("END mouseMoveEvent()\n");
    //printf("%s", myOutput);
    std::cout << myOutput;
    std::cout.flush();
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
