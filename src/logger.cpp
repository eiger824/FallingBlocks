#include <iostream>
#include "logger.hpp"

Logger::Logger(bool enable,
	       LEVEL level): m_enable(enable),
			     m_level(level) {
			     }
Logger::~Logger() {}

void Logger::info(const QString& msg) {
  std::cout << msg.toStdString() << std::endl;
}

void Logger::info(const int nr) {
  std::cout << nr << std::endl;
}

void Logger::info(const QString& msg, const int nr) {
  std::cout << msg.toStdString() << nr << std::endl;
}

void Logger::setEnabled(bool enabled) {
  m_enable = enabled;
}

bool Logger::getEnabled() {
  return m_enable;
}

void Logger::setLevel(LEVEL level) {
  m_level = level;
}

LEVEL Logger::getLevel() {
  return m_level;
}

QString Logger::construct(const QString& msg, const int nr) {
  return (msg + QString::number(nr));
}
