#include <QGuiApplication>

#include "rotatingsquarewindow.h"

int main(int argc, char *argv[])
{
  QGuiApplication a(argc, argv);

  QSurfaceFormat format;
  format.setSamples(16);
  format.setVersion(3,3);
  format.setProfile(QSurfaceFormat::CoreProfile);

  RotatingSquareWindow window;
  window.setFormat(format);
  window.resize(640, 480);
  window.show();
  window.setAnimating(true);
  window.setVelocity(90.0f);
  return a.exec();
}
