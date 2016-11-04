#include <iostream>
#include "position.hpp"

const QString& PREFIX = "images/";

Position::Position(unsigned int i,
		   unsigned int j,
		   COLOR color,
		   QWidget* parent): QWidget(parent),
				     m_x(i),
				     m_y(j),
				     m_color(color),
				     m_locked(false) {
  setFixedSize(50,50);
  setContentsMargins(0,0,0,0);
  m_main_layout = new QVBoxLayout;
  QPixmap target;
  m_label = new QLabel();
  m_main_layout->addWidget(m_label);
  setColor(color);
  setLayout(m_main_layout);
}

Position::~Position() {}

void Position::lock(unsigned int i,
		    unsigned int j) {
  m_x = i;
  m_y = j;
  m_locked = true;
}

void Position::unLock() {
  m_locked = false;
}

bool Position::isLocked() {
  return m_locked;
}

COLOR Position::getColor() {
  return m_color;
}
void Position::setColor(COLOR color) {
  QString path;
  QPixmap target;
  switch (color) {
  case RED:
    path="red.png";
    break;
  case GREEN:
    path="green.png";
    break;
  case BLUE:
    path="blue.png";
    break;
  case YELLOW:
    path="yellow.png";
    break;
  case PINK:
    path="pink.png";
    break;
  case BLACK:
    path="black.png";
    break;
  case ORANGE:
    path="orange.png";
    break;
  case WHITE:
    path="white.png";
    break;
  default:
    path="red.png";
    break;
  }
  if (target.load(PREFIX+path)) {
    m_label->setPixmap(target);
  } else {
    std::cerr << "Failed\n";
  }
  m_color = color;
}

QPair<unsigned int,unsigned int> Position::getXY() {
  return qMakePair(m_x,m_y);
}
