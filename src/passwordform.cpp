#include <QVBoxLayout>
#include <QPushButton>
#include "../include/definitions.hpp"
#include "../include/passwordform.hpp"

#include <iostream>

PasswordForm::PasswordForm(bool login, QWidget* parent): QWidget(parent) {
  QVBoxLayout *main_layout = new QVBoxLayout;
  main_layout->setAlignment(Qt::AlignCenter);
  m_instructions = new QLabel("Type your password below:");
  m_instructions->setStyleSheet("font: arial 14px; color: black;");
  m_instructions->setFixedSize(204,30);
  m_edit = new QLineEdit();
  m_edit->setStyleSheet("background-color: white; font: arial 14px; color: black;");
  m_edit->setFixedSize(204,30);
  m_edit->setEchoMode(QLineEdit::Password);
  if (!login)
    m_edit->setText(PASSPHRASE);
  main_layout->addWidget(m_instructions);
  main_layout->addWidget(m_edit);
  QPushButton *b = new QPushButton("Log in");
  b->setFixedSize(100,50);
  std::cout << "Created button\n";
  connect(b, SIGNAL(pressed()), this, SLOT(checkLogin()));
  main_layout->addWidget(b);
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
