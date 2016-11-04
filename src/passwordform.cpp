#include <QVBoxLayout>
#include "keyvalues.hpp"
#include "passwordform.hpp"

PasswordForm::PasswordForm(QWidget* parent): QWidget(parent) {
  QVBoxLayout *main_layout = new QVBoxLayout;
  main_layout->setAlignment(Qt::AlignCenter);
  m_instructions = new QLabel("Type your password below:");
  m_instructions->setStyleSheet("font: arial 14px; color: black;");
  m_instructions->setFixedSize(204,30);
  m_edit = new QLineEdit();
  m_edit->setStyleSheet("background-color: white; font: arial 14px; color: black;");
  m_edit->setFixedSize(204,30);
  m_edit->setEchoMode(QLineEdit::Password);
  main_layout->addWidget(m_instructions);
  main_layout->addWidget(m_edit);
  connect(m_edit, SIGNAL(returnPressed()), this, SLOT(checkLogin()));
  setLayout(main_layout);
}

PasswordForm::~PasswordForm() {
}

void PasswordForm::checkLogin() {
  if (m_edit->text() == PASSPHRASE) {
    emit loginCorrect();
  } else {
    m_edit->setText("");
  }
}
