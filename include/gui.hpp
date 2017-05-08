#ifndef GUI_HPP_
#define GUI_HPP_

#include <QWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QDBusPendingCallWatcher>

#include "matrix.hpp"
#include "dbusdaemon_interface.h"
#include "definitions.hpp"

class Gui : public QWidget {
  Q_OBJECT
public:
  Gui(bool debug,
      LEVEL level,
      bool update,
      unsigned columns,
      unsigned rows,
      unsigned ms,
      QWidget* parent=0);
  ~Gui();
public:
  void setPid(qint64 pid);
private slots:
  void ready(QDBusPendingCallWatcher* w);
  void exit(QDBusPendingCallWatcher* w);
  void exitAppSlot();
private:
  Matrix *m_matrix;
  QStackedWidget *m_main_stack;
  QVBoxLayout *m_main_layout;
  SeMydnsMyslandDBusDaemonInterface* m_dBusInterface;
  qint64 m_pid;
};

#endif /*GUI_HPP_*/
