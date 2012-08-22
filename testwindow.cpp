// Opengl
#include <GL/gl.h>
#include <GL/glu.h>
#include <conio.h>//needed for getch

// Qt
#include <QtGui/QMouseEvent>
#include <QCursor>
#include <QPoint>

// Local
#include "testwindow.h"

int viewx = 0, viewy = 0;
int lastmousex = 0, lastmousey = 0;
int mouseoriginx = -1, mouseoriginy = -1;

TestWindow::TestWindow(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
}

void TestWindow::initializeGL() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void TestWindow::resizeGL(int w, int h) {
    if (h <= 0) {
        h = 1;
    }
    float height = (float) w / (float) h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0f, height, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TestWindow::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(viewx, viewy, 0);
    glTranslatef(0, 0, -60);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(45.0f,  -1.0f,  1.0f,  0.0f);
    //gl.glTranslatef(viewX, viewY, 0.0f);

    glBegin(GL_QUADS);
        glColor3f(1,0,0);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
    glEnd();
}

void TestWindow::mousePressEvent(QMouseEvent *event) {
    printf("Clicky clicky!\n");
}

void TestWindow::mouseMoveEvent(QMouseEvent *event) {
    if (mouseoriginx < 0 || mouseoriginy < 0){
        printf("MOUSE ORIGIN POS: %d, %d\n", mouseoriginx, mouseoriginy);
        mouseoriginx = TestWindow::x() + (int)(TestWindow::width()*0.5f+0.5f);
        mouseoriginy = TestWindow::y() + (int)(TestWindow::height()*0.5f+0.5f);
        printf("MOUSE ORIGIN POS: %d, %d\n", mouseoriginx, mouseoriginy);
    }
    printf("DIFF POS: %d, %d\n", event->x() - mouseoriginx, event->y() - mouseoriginy);
    viewx = viewx + (event->x() - mouseoriginx/2);
    viewy = viewy + (event->y() - mouseoriginy/2);
    printf("EVENT POS: %d, %d\n", event->x(), event->y());
    //printf("LAST POS: %d, %d\n", lastmousex, lastmousey);
    printf("VIEW POS: %d, %d\n", viewx, viewy);
    //lastmousex = event->x();
    //lastmousey = event->y();
    QCursor::setPos(mouseoriginx, mouseoriginy);

}

void TestWindow::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        event->ignore();
        break;
    }
}
