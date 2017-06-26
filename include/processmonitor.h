#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <QtWidgets>
#include "processtablemodel.h"
#include "sortfilterproxymodel.h"

#include <signal.h>
#include <sys/resource.h>

class ProcessMonitor : public QWidget
{
    Q_OBJECT

private:
    QTableView           *processTable;
    QPushButton          *killProcessButton;
    QPushButton          *changePriorityButton;
    QPushButton          *saveProtocolButton;
    QSpinBox             *prioritySpinBox;
    QVBoxLayout          *vLayout;
    QHBoxLayout          *hLayout;
    ProcessTableModel    *processTableModel;
    SortFilterProxyModel *proxyModel;
    QTimer               *timer;
    QList<ProcessInfo *>  processList;
    QList<ProcessInfo *>  beforeSnapshot;
    QList<ProcessInfo *>  afterSnapshot;

    struct Report
    {
        Report(ProcessInfo *pi = NULL, QString ct = "") : p(pi), completedTime(ct) {}
        ProcessInfo *p;
        QString completedTime;
    };

    QList<Report>  completedProcessList;
    QList<ProcessInfo *>  runningProcessList;
    QTime snapTime;

    QIcon ascIcon, descIcon;
    struct SortData {
        SortData(int sec = -1, bool asc = true) : column(sec), ascending(asc) {}
        int column;
        bool ascending;
    };
    QList<SortData> sortOrder;
    
    int findSection(int section) const;

public:
    ProcessMonitor(QWidget *parent = 0);
    ~ProcessMonitor();

    ProcessInfo * getProcessInfoByStat(QFile *stat);
    void sort();
    void takeSnapshot(QList<ProcessInfo *> *list);
    void updateLog(QList<ProcessInfo *> *before, QList<ProcessInfo *> *after);

public slots:
    void updateProcessList();
    void headerClicked(int section);
    void killProcess();
    void changePriority();
    void saveProtocol();
};

#endif // PROCESSMONITOR_H
