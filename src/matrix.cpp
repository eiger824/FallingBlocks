#include <iostream>
#include "matrix.hpp"

QList<COLOR> m_colors(QList<COLOR>()
		      << RED
		      << GREEN
		      << BLUE
		      << YELLOW
		      << PINK
		      << BLACK
		      << ORANGE
		      << WHITE);


Matrix::Matrix(unsigned int i,
	       unsigned int j,
	       QWidget* parent): QWidget(parent),
				 m_width(i),
				 m_height(j),
				 m_cnt(0),
				 m_hz_pos(0),
				 m_current_col(rand() % m_width),
				 m_score(0),
				 m_current_color(YELLOW) {

  setFixedSize(500,500);
  setStyleSheet("background-color: grey;");
  
  m_main_layout = new QVBoxLayout;
  m_main_layout->setAlignment(Qt::AlignCenter);
  m_block_layout = new QVBoxLayout;
  m_block_layout->setAlignment(Qt::AlignCenter);
  m_block_layout->setSpacing(0);

  m_info = "Score: ";
  m_label = new QLabel;
  m_label->setStyleSheet("background-color: white; \
                          color: black; font: arial 14px;");
  updateScore(true);
  
  std::cout << "Creating matrix with " << i *j << " positions\n";

  for (unsigned c=0; c<i; ++c) {
    QHBoxLayout *horizontal = new QHBoxLayout;
    horizontal->setSpacing(0);
    for (unsigned r=0; r<j; ++r) {
      horizontal->addWidget(new Position(c,r,WHITE));
    }
    m_block_layout->addLayout(horizontal);
  }

  for (unsigned c=0; c < m_width; ++c)
    m_track.append(9);
  
  m_main_layout->addLayout(m_block_layout);
  m_main_layout->addWidget(m_label);
  
  setLayout(m_main_layout);
  m_timer = new QTimer;
  connect(m_timer, SIGNAL(timeout()), this, SLOT(timerOut()));
  show();
}
Matrix::~Matrix() {
}

void Matrix::timerOut() {
  fillDefault();
  Position *position = 
    qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_cnt % m_height)->layout())->itemAt(m_current_col)->widget());
  ++m_cnt;
  //std::cout << "Cnt: " << m_cnt << std::endl;
  if (m_cnt == m_track.at(m_current_col) + 1) {
    if (m_cnt != m_height) {
      Position *next =
	qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_cnt)->layout())->itemAt(m_current_col)->widget());
      std::cout << "-----> Current [" << position->getXY().first
		<< "," << position->getXY().second << "], Color: " <<position->getColor() <<"\n";
      std::cout << "-----> Next    [" << next->getXY().first
		<< "," << next->getXY().second << "], COlor: " <<next->getColor() << "\n";
      if (/*position->getColor()*/m_current_color ==
	  next->getColor()) {
	//paint it, dont lock it
	//position->setColor(m_current_color);
	//just add score
	updateScore(true);
	m_cnt = 0;
	m_current_col = rand() % m_width;
	//switch color
	do {
	  m_current_color = m_colors.at(rand() % m_colors.size());
	} while (m_current_color == WHITE);
      } else {
	//lock position
	std::cout << "LOCKING POSITION\n";
	position->lock(m_cnt-1, m_current_col);
	position->setColor(m_current_color);
	m_locked_pos << qMakePair(m_cnt-1, m_current_col);
	//printLocked();
	unsigned int nr = m_track.at(m_current_col);
	if (nr == 0) {
	  m_timer->stop();
	} else {
	  m_track.replace(m_current_col, --nr);
	  //printAvailable();
	  //switch column
	  m_current_col = rand() % m_width;
	  //restart cnt
	  m_cnt = 0;
	  //switch color
	  do {
	    m_current_color = m_colors.at(rand() % m_colors.size());
	  } while (m_current_color == WHITE);
	}
      }
    } else { //first time
      //lock position
      position->lock(m_cnt-1, m_current_col);
      position->setColor(m_current_color);
      m_locked_pos << qMakePair(m_cnt-1, m_current_col);
      //printLocked();
      unsigned int nr = m_track.at(m_current_col);
      if (nr == 0) {
	m_timer->stop();
      } else {
	m_track.replace(m_current_col, --nr);
	//printAvailable();
	//switch column
	m_current_col = rand() % m_width;
	//restart cnt
	m_cnt = 0;
	//switch color
	do {
	  m_current_color = m_colors.at(rand() % m_colors.size());
	} while (m_current_color == WHITE);
      }
    }
  } else {
    position->setColor(m_current_color);
  }
}

void Matrix::fillDefault() {
  for (unsigned i=0; i<m_width; ++i) {
    for (unsigned j=0; j<m_height; ++j) {
      if (!isPosLocked(i,j)) {
	Position* position =
	  qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(i)->layout())->itemAt(j)->widget());
	if (!position->isLocked()) {
	  position->setColor(WHITE);
	}
      }
    }
  }
}

void Matrix::keyPressEvent(QKeyEvent* event) {
  if (event->key() == LEFT) {
    if (m_current_col == 0) {
      return;
    } else {
      if (canMove(false)) {
	--m_current_col;
      } else  {
	std::cerr << "Not possible to move. Returning...\n";
	return;
      }
      Position *position;
      for (unsigned i=0; i<m_width; ++i) {
	position =
	  qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_current_col % m_height)->layout())->itemAt(i)->widget());
	if (position->isLocked()) {
	  std::cerr << "Position locked. Omitting...\n";
	}  else {
	  if (i == m_current_col)
	    position->setColor(m_current_color);
	  else
	    position->setColor(WHITE);
	}
      }
      fillDefault();
    }
  } else if (event->key() == RIGHT) {
    if (m_current_col == m_width -1) {
      return;
    } else {
      if (canMove(true)) {
	++m_current_col;
      } else {
	std::cerr << "Not possible to move. Returning...\n";
	return;
      }
      Position *position;
      for (unsigned i=0; i<m_width; ++i) {
	position =
	  qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_current_col % m_width)->layout())->itemAt(i)->widget());
	if (position->isLocked()) {
	  std::cerr << "Position locked. Omitting...\n";
	} else {
	  if (i == m_current_col)
	    position->setColor(m_current_color);
	  else
	    position->setColor(WHITE);
	}
      }
    }
    fillDefault();
  } else if (event->key() == ESC) {
    if (m_timer->isActive()) {
      m_timer->stop();
    } else {
      m_timer->start(333);
    }
  } else if (event->key() == ASCII_ENTER) {
    std::cout << "Enter pressed\n";
    printLocked();
  }
}

void Matrix::printAvailable() {
  for (unsigned i=0; i<m_width; ++i) {
    std::cout << "Column: " << i << ": " << m_track.at(i) << std::endl;
  }
}

bool Matrix::canMove(bool direction) {
  Position *position;
  if (direction) {
    //right
    position =
      qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_cnt)->layout())->itemAt(m_current_col+1)->widget());
    if (position->isLocked()) return false;
    else return true;
  } else {
    //left
    position =
      qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_cnt)->layout())->itemAt(m_current_col-1)->widget());
    if (position->isLocked()) return false;
    else return true;
  }
}

void Matrix::printLocked() {
  std::cout << "@@@@@@ Locked Positions @@@@@@\n";
  for (unsigned i=0; i<m_locked_pos.size(); ++i) {
    std::cout << "[" << m_locked_pos.at(i).first
	      << "," << m_locked_pos.at(i).second << "]\n";
  }
  std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
}

bool Matrix::isPosLocked(unsigned int i, unsigned int j) {
  if (m_locked_pos.contains(qMakePair(i,j))) return true;
  else return false;
}

void Matrix::updateScore(bool opt) {
  m_label->setText(m_info + QString::number(m_score));
  ++m_score;
}

void Matrix::startTimer() {
  m_timer->start(333);
}
