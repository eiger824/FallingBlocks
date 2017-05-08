#include <QDBusConnection>
#include <glog/logging.h>

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

    // Inform DBus daemon about this app
    m_dBusInterface = new SeMydnsMyslandDBusDaemonInterface(QString("se.mydns.mysland.DBusDaemon"),
                                                            QString("/DBusDaemon"),
                                                            QDBusConnection::sessionBus());

    m_matrix = new Matrix(debug,level,update,rows,columns,ms);
    connect(m_matrix, SIGNAL(exitApp()),
            this, SLOT(exitAppSlot()));

    m_main_stack = new QStackedWidget;
    m_main_stack->addWidget(m_matrix);
    m_main_layout->addWidget(m_main_stack);

    setLayout(m_main_layout);
    show();

    //start action!
    m_matrix->startTimer();
}

Gui::~Gui() {
    //free memory
    delete m_matrix;
    delete m_main_layout;
    delete m_main_stack;
}

void Gui::setPid(qint64 pid)
{
    // Inform about pid and appname
    m_pid = pid;
    DLOG (INFO) << "Informing about PID: " << (int)pid;
    QDBusPendingCallWatcher *watcher = new
            QDBusPendingCallWatcher(m_dBusInterface->registerApp((int)pid, "fallingblocks"), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(ready(QDBusPendingCallWatcher*)));
}

void Gui::ready(QDBusPendingCallWatcher *w) {
    QDBusPendingReply<> reply = *w;
    if (reply.isError()) {
        LOG (ERROR) << "[ERROR] in transaction" << reply.error().message().toStdString();
    } else {
        DLOG (INFO) << "Successfully completed D-Bus transaction";
    }
}

void Gui::exitAppSlot()
{
    DLOG (INFO) << "Unregistering PID: " << m_pid;
    QDBusPendingCallWatcher *watcher = new
            QDBusPendingCallWatcher(m_dBusInterface->unregisterApp((int)m_pid), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(exit(QDBusPendingCallWatcher*)));
}

void Gui::exit(QDBusPendingCallWatcher *w) {
    QDBusPendingReply<> reply = *w;
    if (reply.isError()) {
        LOG (ERROR) << "[ERROR] in exit command: "
                    << reply.error().message().toStdString();
    } else {
        qApp->quit();
    }
}
