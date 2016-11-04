#include <QApplication>
#include <getopt.h>
#include <iostream>
#include "gui.hpp"

int help(int code) {
  std::cout << "Falling Blocks - The Game!\n";
  std::cout << "[USAGE]: fblocks [OPTIONS]\n";
  std::cout << "OPTIONS:\n";
  std::cout << "-c <n>\tSet column count to n\n";
  std::cout << "-d\tEnable debug controls\n";
  std::cout << "-h\tPrint this help and exit\n";
  std::cout << "-r <n>\tSet row count to n\n";
  std::cout << "-s\tSkip login page\n";
  std::cout << "-t <ms>\tSet falling block refresh rate (in ms.)\n";
  if (code == 0) {
    return 0;
  } else {
    return -1;
  }
}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  bool login=true;
  bool debug=false;
  int c;
  unsigned columns = 10;
  unsigned ms = 333;
  unsigned rows = 10;
  while ((c = getopt(argc,argv,"dhsc:r:t:")) != -1) {
    switch (c) {
    case 'd':
      debug=true;
      break;
    case 's':
      login=false;
      break;
    case 'h':
      return help(0);
    case 'c':
      columns = std::atoi(optarg);
      break;
    case 'r':
      rows = std::atoi(optarg);
      break;
    case 't':
      ms = std::atoi(optarg);
      std::cout << "Timer: " << ms << std::endl;
      break;
    default:
      std::cerr << "Wrong argument " << c << "\n";
      return help(1);
    }
  }
  Gui gui(debug,
	  login,
	  columns,
	  rows,
	  ms);
  return app.exec();
}
