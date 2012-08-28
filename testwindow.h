#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QtOpenGL/QGLWidget>
#include <QTimer>

class TestWindow : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    TestWindow(QWidget *parent = NULL);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    QTimer* timer;

public slots:
    void showFps();
    void processing();
};

#endif // TESTWINDOW_H
