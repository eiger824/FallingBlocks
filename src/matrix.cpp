#include <iostream>
#include <QMessageBox>
#include "../include/matrix.hpp"

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
	       LEVEL level,
	       bool update,
	       unsigned int i,
	       unsigned int j,
	       unsigned int ms,
	       QWidget* parent): QWidget(parent),
				 m_width(j),
				 m_height(i),
				 m_cnt(0),
				 m_ms(ms),
				 m_ms_0(ms),
				 m_debug(debug),
				 m_level(level),
				 m_update(update),
				 m_current_col(rand() % m_width),
				 m_score(0),
				 m_current_color(YELLOW) {

  setStyleSheet("background-color: grey;");
  //logger init
  m_logger = new Logger(debug,level);
  
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
  
  m_logger->info("Creating matrix with nr. positions: ", i*j, HIGH);

  for (unsigned c=0; c<i; ++c) {
    QHBoxLayout *horizontal = new QHBoxLayout;
    horizontal->setSpacing(0);
    for (unsigned r=0; r<j; ++r) {
      horizontal->addWidget(new Position(c,r,WHITE));
    }
    m_block_layout->addLayout(horizontal);
  }
  for (unsigned c=0; c < m_width; ++c)
    m_track.append(m_height-1);
  
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
	m_logger->info("Found at index: ",index, HIGH);
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
        m_logger->info("Locking position\n", MEDIUM);
	position->lock(m_cnt-1, m_current_col);
	position->setColor(m_current_color);
	m_locked_pos << qMakePair(m_cnt-1, m_current_col);
	unsigned int nr = m_track.at(m_current_col);
	if (nr == 0) {
	  m_logger->info("Reached limit (you lost ;-)\n", HIGH);
	  QMessageBox::information(this, "Game over", "Obtained score: "
				   + QString::number(m_score) + ". Press OK"
				   + "/Enter to restart!");
	  
	  m_timer->stop();
	  restartGame();
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
	m_logger->info("Reached limit (you lost ;-)", HIGH);
	QMessageBox::information(this, "Game over", "Obtained score: "
				 + QString::number(m_score) + ". Press OK"
				   + "/Enter to restart!");
	m_timer->stop();
	restartGame();
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
  for (unsigned i=0; i<m_height; ++i) {
    for (unsigned j=0; j<m_width; ++j) {
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

void Matrix::mousePressEvent(QMouseEvent* event) {
    m_logger->info("Mouse event", HIGH);
  if (event->x() < 400) {
    if (m_current_col == 0) {
      return;
    } else {
      if (canMove(false)) {
	--m_current_col;
      } else  {
	m_logger->info("Not possible to move. Returning...", HIGH);
	return;
      }
      Position *position;
      for (unsigned i=0; i<m_width; ++i) {
	position =
	  qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_current_col % m_height)->layout())->itemAt(i)->widget());
	if (position->isLocked()) {
	  m_logger->info("Position locked. Omitting...", HIGH);
	}  else {
	  if (i == m_current_col)
	    position->setColor(m_current_color);
	  else
	    position->setColor(WHITE);
	}
      }
      fillDefault();
    }
  } else if (event->x() >= 400) {
    if (m_current_col == m_width -1) {
      return;
    } else {
      if (canMove(true)) {
	++m_current_col;
      } else {
	m_logger->info("Not possible to move. Returning...", HIGH);
	return;
      }
      Position *position;
      for (unsigned i=0; i<m_width; ++i) {
	position =
	  qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(m_current_col % m_height)->layout())->itemAt(i)->widget());
	if (position->isLocked()) {
	  m_logger->info("Position locked. Omitting...", HIGH);
	} else {
	  if (i == m_current_col)
	    position->setColor(m_current_color);
	  else
	    position->setColor(WHITE);
	}
      }
    }
    fillDefault();
  } /*else if (event->key() == ESC && m_debug) {
    if (m_timer->isActive()) {
      m_logger->info("Stopping...", MEDIUM);
      m_timer->stop();
    } else {
      m_logger->info("Resuming...", MEDIUM);
      m_timer->start(m_ms);
    }
  } else if (event->key() == ASCII_SPACE && m_debug) {
    printLocked();
  } else if (event->key() == ENTER && m_debug) {
    printAvailable();
    }*/
}

void Matrix::mouseMoveEvent(QMouseEvent *event) {
    m_logger->info("(x:" + QString::number(event->x()) +
                   ",y:" + QString::number(event->y()) + ")", HIGH);
}

void Matrix::printAvailable() {
  for (unsigned i=0; i<m_width; ++i) {
    m_logger->info("Column: " + QString::number(i) + ": " + QString::number(m_track.at(i)), LOW);
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
  m_logger->info("@@@@@@ Locked Positions @@@@@@", LOW);
  for (unsigned i=0; i<m_locked_pos.size(); ++i) {
    m_logger->info("[" + QString::number(m_locked_pos.at(i).first)
		   + "," + QString::number(m_locked_pos.at(i).second) + "]", LOW);
  }
  m_logger->info("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", LOW);
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
	m_logger->info("----------------> LEVEL UP!", LOW);
      }
    }
  }
}

void Matrix::startTimer() {
  m_logger->info("Starting timer: ", m_ms_0, MEDIUM);
  m_timer->start(m_ms);
}

void Matrix::restartGame() {
  m_logger->info("Restarting game!", LOW);
  //clear list
  m_locked_pos.clear();
  //clear positions
  for (unsigned i=0; i<m_width; ++i) {
    for (unsigned j=0; j<m_height; ++j) {
      Position *position =
	qobject_cast<Position*>(qobject_cast<QLayout*>(m_block_layout->itemAt(i)->layout())->itemAt(j)->widget());
      position->unLock();
    }
  }
  //update blocks
  fillDefault();
  //start timer
  m_timer->start(m_ms_0);
  //restart score
  m_score = 0;
  updateScore(false);
  //restart counter
  m_cnt = 0;
  //restart m_track
  m_track.clear();
  for (unsigned c=0; c < m_width; ++c)
    m_track.append(m_width-1);
}
