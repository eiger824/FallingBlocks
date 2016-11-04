#include "gui.hpp"

Gui::Gui(QWidget* parent): QWidget(parent) {
  setStyleSheet("background-color: #c3c3c3;");
  m_main_layout = new QVBoxLayout;
  m_main_layout->setAlignment(Qt::AlignCenter);

  m_matrix = new Matrix(10,10);
  m_password_form = new PasswordForm;

  connect(m_password_form, SIGNAL(loginCorrect()), this, SLOT(changeIndex()));
  connect(m_password_form, SIGNAL(loginCorrect()), m_matrix, SLOT(startTimer()));
  
  m_main_stack = new QStackedWidget;
  m_main_stack->addWidget(m_password_form);
  m_main_stack->addWidget(m_matrix);

  m_main_layout->addWidget(m_main_stack);
  setLayout(m_main_layout);
  show();
}

Gui::~Gui() {
}

void Gui::changeIndex() {
  m_main_stack->setCurrentIndex(1);
}
