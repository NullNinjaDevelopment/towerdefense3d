#include <QtGui/QApplication>
#include <QtOpenGL/QGLWidget>
#include "testwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOverrideCursor( QCursor( Qt::BlankCursor ) );

    TestWindow window;
    window.resize(800,600);
    window.show();

    return app.exec();
}
