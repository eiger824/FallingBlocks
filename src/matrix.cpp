#include <iostream>
#include <QMessageBox>
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


Matrix::Matrix(bool debug,
	       bool update,
	       unsigned int i,
	       unsigned int j,
	       unsigned int ms,
	       QWidget* parent): QWidget(parent),
				 m_width(i),
				 m_height(j),
				 m_cnt(0),
				 m_ms(ms),
				 m_debug(debug),
				 m_update(update),
				 m_current_col(rand() % m_width),
				 m_score(0),
				 m_current_color(YELLOW) {

  setFixedSize(500,500);
  setStyleSheet("background-color: grey;");

  //logger init
  m_logger = new Logger(debug,HIGH);
  
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
  
  m_logger->info("Creating matrix with nr. positions: ", i*j);

  for (unsigned c=0; c<i; ++c) {
    QHBoxLayout *horizontal = new QHBoxLayout;
    horizontal->setSpacing(0);
    for (unsigned r=0; r<j; ++r) {
      horizontal->addWidget(new Position(c,r,WHITE));
    }
    m_block_layout->addLayout(horizontal);
  }

  for (unsigned c=0; c < m_width; ++c)
    m_track.append(i-1);
  
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
  position->setColor(m_current_color);
  ++m_cnt;
  if (m_cnt == m_track.at(m_current_col) + 1) {
    if (m_cnt != m_height) {
      Position *next =
	qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_cnt)->layout())->itemAt(m_current_col)->widget());
      if (m_current_color ==
	  next->getColor()) {
	//paint it, just add score
	updateScore(true);
	//and unlock position
	int index = m_locked_pos.indexOf(qMakePair(m_cnt, m_current_col));
	m_logger->info("Found at index: ",index);
	m_locked_pos.removeAt(index);
	next->unLock();
	unsigned int nr = m_track.at(m_current_col);
	m_track.replace(m_current_col, ++nr);
	m_cnt = 0;
	m_current_col = rand() % m_width;
	//switch color
	do {
	  m_current_color = m_colors.at(rand() % m_colors.size());
	} while (m_current_color == WHITE);
      } else {
	//lock position
        m_logger->info("Locking position\n");
	position->lock(m_cnt-1, m_current_col);
	position->setColor(m_current_color);
	m_locked_pos << qMakePair(m_cnt-1, m_current_col);
	unsigned int nr = m_track.at(m_current_col);
	if (nr == 0) {
	  m_logger->info("Reached limit (you lost ;-)\n");
	  QMessageBox::information(this, "Game over", "Obtained score: "
				   + QString::number(m_score));
	  m_score = 0;
	  updateScore(false);
	  m_timer->stop();
	} else {
	  m_track.replace(m_current_col, --nr);
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
	m_logger->info("Reached limit (you lost ;-)");
	QMessageBox::information(this, "Game over", "Obtained score: "
				 + QString::number(m_score));
	m_score = 0;
	updateScore(false);
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
	m_logger->info("Not possible to move. Returning...");
	return;
      }
      Position *position;
      for (unsigned i=0; i<m_width; ++i) {
	position =
	  qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_current_col % m_height)->layout())->itemAt(i)->widget());
	if (position->isLocked()) {
	  m_logger->info("Position locked. Omitting...");
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
	m_logger->info("Not possible to move. Returning...");
	return;
      }
      Position *position;
      for (unsigned i=0; i<m_width; ++i) {
	position =
	  qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_current_col % m_width)->layout())->itemAt(i)->widget());
	if (position->isLocked()) {
	  m_logger->info("Position locked. Omitting...");
	} else {
	  if (i == m_current_col)
	    position->setColor(m_current_color);
	  else
	    position->setColor(WHITE);
	}
      }
    }
    fillDefault();
  } else if (event->key() == ESC && m_debug) {
    if (m_timer->isActive()) {
      m_logger->info("Stopping...");
      m_timer->stop();
    } else {
      m_logger->info("Resuming...");
      m_timer->start(m_ms);
    }
  } else if (event->key() == ASCII_SPACE && m_debug) {
    printLocked();
  } else if (event->key() == ENTER && m_debug) {
    printAvailable();
  }
}

void Matrix::printAvailable() {
  for (unsigned i=0; i<m_width; ++i) {
    m_logger->info("Column: " + QString::number(i) + ": " + QString::number(m_track.at(i)));
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
  m_logger->info("@@@@@@ Locked Positions @@@@@@");
  for (unsigned i=0; i<m_locked_pos.size(); ++i) {
    m_logger->info("[" + QString::number(m_locked_pos.at(i).first)
		   + "," + QString::number(m_locked_pos.at(i).second) + "]");
  }
  m_logger->info("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
}

bool Matrix::isPosLocked(unsigned int i, unsigned int j) {
  if (m_locked_pos.contains(qMakePair(i,j))) return true;
  else return false;
}

void Matrix::updateScore(bool opt) {
  if (!opt) {
    m_label->setText(m_info + "0");
    m_score = 0;
    return;
  }
  m_label->setText(m_info + QString::number(m_score));
  ++m_score;
  if (m_update) {
    if (m_score % 10 == 0) {
      m_ms-=50;
      if (m_ms > 0) {
	m_timer->start(m_ms);
	m_logger->info("----------------> LEVEL UP!");
      }
    }
  }
}

void Matrix::startTimer() {
  m_logger->info("Starting timer: ", m_ms);
  m_timer->start(m_ms);
}
