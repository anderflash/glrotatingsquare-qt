#include <QGuiApplication>

#include "rotatingsquarewindow.h"

int main(int argc, char *argv[])
{
  QGuiApplication a(argc, argv);
  RotatingSquareWindow window;
  window.resize(640, 480);
  window.show();
  return a.exec();
}
