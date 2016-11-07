#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <QString>
#include "definitions.hpp"

class Logger {
public:
  Logger(bool enable, LEVEL level=HIGH);
  ~Logger();
  void info(const QString& msg);
  void info(const int nr);
  void info(const QString& msg, const int nr);
  bool getEnabled();
  void setEnabled(bool enabled);
  LEVEL getLevel();
  void setLevel(LEVEL level);
  QString construct(const QString& msg, const int nr);
private:
  bool m_enable;
  LEVEL m_level;
};

#endif /*LOGGER_HPP_*/
