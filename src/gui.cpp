#include <iostream>
#include "../include/gui.hpp"

Gui::Gui(bool debug,
         LEVEL level,
         bool update,
         unsigned columns,
         unsigned rows,
         unsigned ms,
         QWidget* parent): QWidget(parent) {

    this->setFixedWidth(800);
    this->setFixedHeight(480);
    setStyleSheet("background-color: #c3c3c3;");
    m_main_layout = new QVBoxLayout;
    m_main_layout->setAlignment(Qt::AlignCenter);

    m_matrix = new Matrix(debug,level,update,rows,columns,ms);

    m_main_stack = new QStackedWidget;
    m_main_stack->addWidget(m_matrix);
    m_main_layout->addWidget(m_main_stack);

    setLayout(m_main_layout);
    show();

    //start action!
    m_matrix->startTimer();
}

Gui::~Gui() {
}
