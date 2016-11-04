#include <QApplication>
#include "gui.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  Gui gui;
  return app.exec();
}
