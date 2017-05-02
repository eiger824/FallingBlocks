#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QMouseEvent>

#include "position.hpp"
#include "logger.hpp"
#include "definitions.hpp"

class Matrix : public QWidget {
  Q_OBJECT
public:
  Matrix(bool debug,
	 LEVEL level,
	 bool update,
	 unsigned int i,
	 unsigned int j,
	 unsigned ms,
	 QWidget* parent=0);
  ~Matrix();
private:
  void fillDefault();
  void printAvailable();
  bool canMove(bool direction);
  void printLocked();
  bool isPosLocked(unsigned int i, unsigned int j);
  void updateScore(bool opt);
  void restartGame();
protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
public slots:
  void startTimer();
private slots:
  void timerOut();
private:
  unsigned int m_width;
  unsigned int m_height;
  QVBoxLayout *m_main_layout;
  QVBoxLayout *m_block_layout;
  QTimer *m_timer;
  unsigned m_cnt;
  unsigned int m_current_col;
  QList<unsigned int>m_track;
  COLOR m_current_color;
  QList<QPair<unsigned int, unsigned int> >m_locked_pos;
  QLabel *m_label;
  QString m_info;
  unsigned m_score;
  unsigned m_ms;
  unsigned m_ms_0;
  bool m_debug;
  bool m_update;
  Logger *m_logger;
  LEVEL m_level;
};

#endif /*MATRIX_HPP_*/
