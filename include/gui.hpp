#ifndef GUI_HPP_
#define GUI_HPP_

#include <QWidget>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "matrix.hpp"
#include "passwordform.hpp"

class Gui : public QWidget {
  Q_OBJECT
public:
  Gui(bool login, QWidget* parent=0);
  ~Gui();
private slots:
  void changeIndex();
private:
  Matrix *m_matrix;
  PasswordForm *m_password_form;
  QStackedWidget *m_main_stack;
  QVBoxLayout *m_main_layout;
};

#endif /*GUI_HPP_*/
