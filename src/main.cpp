#include <QApplication>
#include <getopt.h>
#include "gui.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  bool login=true;
  int c;
  while ((c = getopt(argc,argv,"s")) != -1) {
    switch (c) {
    case 's':
      login=false;
      break;
    }
  }
  Gui gui(login);
  return app.exec();
}
