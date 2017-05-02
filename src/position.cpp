#include <iostream>
#include "../include/position.hpp"

const QString& COMMONPREFIX = "/usr/bin/apps";
const QString& PREFIX = "fallingblocks";
const QString& SEP = "/";

Position::Position(unsigned int i,
		   unsigned int j,
		   COLOR color,
		   QWidget* parent): QWidget(parent),
				     m_x(i),
				     m_y(j),
				     m_color(color),
				     m_locked(false) {
  //setFixedSize(50,50);
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
    path="images/red.png";
    break;
  case GREEN:
    path="images/green.png";
    break;
  case BLUE:
    path="images/blue.png";
    break;
  case YELLOW:
    path="images/yellow.png";
    break;
  case PINK:
    path="images/pink.png";
    break;
  case BLACK:
    path="images/black.png";
    break;
  case ORANGE:
    path="images/orange.png";
    break;
  case WHITE:
    path="images/white.png";
    break;
  default:
    path="images/red.png";
    break;
  }

  if (target.load(COMMONPREFIX + SEP + PREFIX + SEP +path)) {
    m_label->setPixmap(target);
  } else {
    //std::cerr << "Failed\n";
  }
  m_color = color;
}

QPair<unsigned int,unsigned int> Position::getXY() {
  return qMakePair(m_x,m_y);
}
