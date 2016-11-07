#include <QApplication>
#include <getopt.h>
#include <iostream>
#include "definitions.hpp"
#include "gui.hpp"

int help(int code) {
  std::cout << "Falling Blocks - The Game!\n";
  std::cout << "[USAGE]: fblocks [OPTIONS]\n";
  std::cout << "OPTIONS:\n";
  std::cout << "-c <n>\t\tSet column count to n\n";
  std::cout << "-d <level>\tEnable debug controls\n";
  std::cout << "   (level: HIGH,MEDIUM,LOW)\n";
  std::cout << "-h\t\tPrint this help and exit\n";
  std::cout << "-r <n>\t\tSet row count to n\n";
  std::cout << "-s\t\tSkip login page\n";
  std::cout << "-t <ms>\t\tSet falling block refresh rate (in ms.)\n";
  std::cout << "-u\t\tUpdate timer every 10 points\n";
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
  bool update=false;
  int c;
  LEVEL level = HIGH;
  unsigned columns = 10;
  unsigned ms = 333;
  unsigned rows = 10;
  while ((c = getopt(argc,argv,"d:hsuc:r:t:")) != -1) {
    switch (c) {
    case 'd':
      debug=true;
      if (strcmp(optarg,"HIGH") &&
          strcmp(optarg,"MEDIUM") &&
          strcmp(optarg,"LOW")) {
          std::cerr << "Error, invalid level: " << optarg << ".\n";
	  return help(1);
      }
      std::cout << "Debug enabled (level: " << optarg << ")\n";
      if (!strcmp(optarg,"HIGH")) level = HIGH;
      else if (!strcmp(optarg,"MEDIUM")) level = MEDIUM;
      else if (!strcmp(optarg,"LOW")) level = LOW;
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
    case 'u':
      update=true;
      break;
    default:
      std::cerr << "Wrong argument " << c << "\n";
      return help(1);
    }
  }
  Gui gui(debug,
	  level,
	  login,
	  update,
	  columns,
	  rows,
	  ms);
  return app.exec();
}
