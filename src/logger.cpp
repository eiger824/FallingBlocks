#include <iostream>
#include <QTime>
#include <QDate>
#include "logger.hpp"

Logger::Logger(bool enable,
	       LEVEL level): m_enable(enable),
			     m_level(level) {
  std::cout << "Started logger with level: " << level << std::endl;
			     }
Logger::~Logger() {}

void Logger::info(const QString& msg, const LEVEL level) {
  if (m_level <= level)
    std::cout << QDate::currentDate().toString().toStdString() << ","
	      << QTime::currentTime().toString().toStdString() << ": "
	      << msg.toStdString() << std::endl;
}

void Logger::info(const int nr, const LEVEL level) {
  if (m_level <= level)
    std::cout << QDate::currentDate().toString().toStdString() << ","
	      << QTime::currentTime().toString().toStdString() << ": "
	      << nr << std::endl;
}

void Logger::info(const QString& msg, const int nr, const LEVEL level) {
  if (m_level <= level)
    std::cout << QDate::currentDate().toString().toStdString() << ","
	      << QTime::currentTime().toString().toStdString() << ": "
	      << msg.toStdString() << nr << std::endl;
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
