#ifndef GUI_HPP_
#define GUI_HPP_

#include <QWidget>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "matrix.hpp"

#include "definitions.hpp"

class Gui : public QWidget {
  Q_OBJECT
public:
  Gui(bool debug,
      LEVEL level,
      bool update,
      unsigned columns,
      unsigned rows,
      unsigned ms,
      QWidget* parent=0);
  ~Gui();
private:
  Matrix *m_matrix;
  QStackedWidget *m_main_stack;
  QVBoxLayout *m_main_layout;
};

#endif /*GUI_HPP_*/
