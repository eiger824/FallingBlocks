#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QKeyEvent>

#include "position.hpp"

class Matrix : public QWidget {
  Q_OBJECT
public:
  Matrix(unsigned int i,
	 unsigned int j,
	 QWidget* parent=0);
  ~Matrix();
private:
  void fillDefault();
  void printAvailable();
  bool canMove(bool direction);
  void printLocked();
  bool isPosLocked(unsigned int i, unsigned int j);
  void updateScore(bool opt);
protected:
  void keyPressEvent(QKeyEvent *event);
private slots:
  void timerOut();
  void startTimer();
private:
  unsigned int m_width;
  unsigned int m_height;
  QVBoxLayout *m_main_layout;
  QVBoxLayout *m_block_layout;
  QTimer *m_timer;
  unsigned m_cnt;
  unsigned m_hz_pos;
  unsigned int m_current_col;
  QList<unsigned int>m_track;
  COLOR m_current_color;
  QList<QPair<unsigned int, unsigned int> >m_locked_pos;
  QLabel *m_label;
  QString m_info;
  unsigned m_score;
};

#endif /*MATRIX_HPP_*/
