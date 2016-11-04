#ifndef PASSWORD_FORM_HPP_
#define PASSWORD_FORM_HPP_

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class PasswordForm : public QWidget {
  Q_OBJECT
public:
  PasswordForm(QWidget* parent=0);
  ~PasswordForm();
signals:
  void loginCorrect();
private slots:
  void checkLogin();
private:
  QLabel *m_instructions;
  QLineEdit *m_edit;
};

#endif /*PASSWORD_FORM_HPP_*/
