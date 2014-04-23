#include <QApplication>

#include "mainwindow.h"
#ifdef Q_WS_X11       // on X11 / linux
    #include "GL/glut.h"
#endif

int main(int argc, char *argv[])
{
  #ifdef Q_WS_X11 
  glutInit(&argc, argv);
  #endif

  QApplication app(argc, argv);
  Q_INIT_RESOURCE(dockwidgets);
  MainWindow mainWin;
  mainWin.showMaximized();
  return app.exec();
}
