#include "processmonitor.h"

ProcessMonitor::ProcessMonitor(QWidget *parent)
    : QWidget(parent)
{
    processTable         = new QTableView;
    processTableModel    = new ProcessTableModel;
    killProcessButton    = new QPushButton;
    changePriorityButton = new QPushButton;
    saveProtocolButton   = new QPushButton;
    vLayout              = new QVBoxLayout;
    hLayout              = new QHBoxLayout;
    proxyModel           = new SortFilterProxyModel;
    prioritySpinBox      = new QSpinBox;

    proxyModel->setSourceModel(processTableModel);
    processTable->setModel(proxyModel);

    processTable->horizontalHeader()->setSectionsMovable(true);
    processTable->horizontalHeader()->setSectionsClickable(true);
    processTable->horizontalHeader()->setHighlightSections(false);
    processTable->horizontalHeader()->setStretchLastSection(true);

    processTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    processTable->setSortingEnabled(false);
    processTable->setAlternatingRowColors(true);

    this->ascIcon  = processTable->style()->standardIcon(QStyle::SP_ArrowUp);
    this->descIcon = processTable->style()->standardIcon(QStyle::SP_ArrowDown);

    hLayout->addWidget(saveProtocolButton);
    hLayout->addWidget(prioritySpinBox);
    hLayout->addWidget(changePriorityButton);
    hLayout->addWidget(killProcessButton);

    vLayout->addWidget(processTable);
    vLayout->addLayout(hLayout);

    vLayout->setAlignment(hLayout, Qt::AlignBottom | Qt::AlignRight);

    killProcessButton->setText("Die! Die! Die!");
    changePriorityButton->setText("Change priority");
    saveProtocolButton->setText("Save protocol");
    prioritySpinBox->setMaximum(19);
    prioritySpinBox->setMinimum(-20);

    this->setLayout(vLayout);

    this->updateProcessList();
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateProcessList()));
    connect(processTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerClicked(int)));
    connect(killProcessButton, SIGNAL(clicked()), this, SLOT(killProcess()));
    connect(changePriorityButton, SIGNAL(clicked()), this, SLOT(changePriority()));
    connect(saveProtocolButton, SIGNAL(clicked()), this, SLOT(saveProtocol()));

    timer->start(500);
}

ProcessMonitor::~ProcessMonitor()
{

}

void ProcessMonitor::updateProcessList()
{
    QDir *dir = new QDir("/proc");
    QStringList dirProcessList = dir->entryList(QStringList("*"), QDir::AllDirs);
    QFile stat;

    int len = processList.size();

    beforeSnapshot.clear();
    takeSnapshot(&beforeSnapshot);

    processList.clear();
    foreach(QString str, dirProcessList)
    {
        if (str.toInt())
        {
            stat.setFileName("/proc/" + str.toUtf8() + "/stat");
            stat.open(QIODevice::ReadOnly);

            processList.append(getProcessInfoByStat(&stat));

            stat.close();
        }
        else continue;
    }

    afterSnapshot.clear();
    takeSnapshot(&afterSnapshot);

    int dif = len - processList.size();

    int lastRow = processTableModel->rowCount();
    if (dif < 0)
    {
        processTableModel->insertRows(lastRow, -dif, QModelIndex());
    }
    else if (dif > 0)
    {
        processTableModel->removeRows(lastRow - dif,  dif, QModelIndex());
    }

    for (int i = 0; i < processList.size(); i++)
    {
        processTableModel->setData(processTableModel->index(i, 0), processList.at(i)->comm,  Qt::DisplayRole);
        processTableModel->setData(processTableModel->index(i, 1), processList.at(i)->pid,   Qt::DisplayRole);
        processTableModel->setData(processTableModel->index(i, 2), processList.at(i)->stat,  Qt::DisplayRole);
        processTableModel->setData(processTableModel->index(i, 3), processList.at(i)->nice,  Qt::DisplayRole);
        processTableModel->setData(processTableModel->index(i, 4), processList.at(i)->cpu,   Qt::DisplayRole);
        processTableModel->setData(processTableModel->index(i, 5), processList.at(i)->stime, Qt::DisplayRole);
    }

    sort();
    updateLog(&beforeSnapshot, &afterSnapshot);
}

ProcessInfo* ProcessMonitor::getProcessInfoByStat(QFile *stat)
{
    QTextStream stream;
    QStringList list;
    QString buffer;

    stream.setDevice(stat);
    buffer = stream.readLine();
    list = buffer.split(" ");

    ProcessInfo *process = new ProcessInfo;

    process->pid  = list[0];
    process->comm = list[1].replace("(", "").replace(")", "");
    process->stat = list[2];
    process->nice = list[18];

    QFile uptimeFile("/proc/uptime");
    uptimeFile.open(QIODevice::ReadOnly);

    stream.setDevice(&uptimeFile);
    buffer = stream.readLine();
    int uptime = buffer.replace(QRegExp("\.[0-9]+ [0-9]+\.[0-9]+"), "").toInt();
    uptimeFile.close();

    int dif = uptime - list[21].toInt() / 100;
    QTime startTime = QTime::currentTime().addSecs(-dif);

    process->stime = startTime.toString("hh:mm");

    double total_time = list[13].toDouble() + list[14].toDouble() + list[15].toDouble() + list[16].toDouble();
    double seconds = (double)uptime - (list[21].toDouble() / 100.0);

    process->cpu = QString::number((double)(100 * ((total_time / 100) / seconds)), 'f', 2);

    return process;
}

void ProcessMonitor::headerClicked(int section)
{
    bool ascending = true;

    if (!sortOrder.isEmpty())
    {
        int index = findSection(section);
        if (index != -1)
        {
            ascending = !sortOrder.at(index).ascending;
            sortOrder.removeAt(index);
        }
        sortOrder.clear();
    }
    sortOrder.append(SortData(section, ascending));
    updateProcessList();
}

int ProcessMonitor::findSection(int section) const
{
  for (int i=0; i<sortOrder.size(); ++i) {
    if (sortOrder.at(i).column == section)
      return i;
  }
  return -1;
}

void ProcessMonitor::sort()
{
    for (int i=sortOrder.size() - 1; i>=0; --i) {
        proxyModel->sort(sortOrder.at(i).column,
                        sortOrder.at(i).ascending ? Qt::AscendingOrder : Qt::DescendingOrder);
    }
}

void ProcessMonitor::killProcess()
{
    QModelIndex index = processTable->model()->index(processTable->currentIndex().row(), 1);
    QVariant spid = processTable->model()->data(index, Qt::DisplayRole);

    pid_t pid = spid.toInt();
    if (kill(pid, SIGHUP) == -1)
        QMessageBox::warning(0, "Kill", "Процесс не может быть завершён");
}

void ProcessMonitor::changePriority()
{
    QModelIndex index = processTable->model()->index(processTable->currentIndex().row(), 1);
    QVariant spid = processTable->model()->data(index, Qt::DisplayRole);

    pid_t pid = spid.toInt();
    setpriority(PRIO_PROCESS, pid, prioritySpinBox->value());
}

void ProcessMonitor::takeSnapshot(QList<ProcessInfo *> *list)
{
    snapTime = QTime::currentTime();
    foreach(ProcessInfo *p, processList)
    {
        list->append(new ProcessInfo(p));
    }
}

void ProcessMonitor::updateLog(QList<ProcessInfo *> *before, QList<ProcessInfo *> *after)
{
    int i, j;

    QList<ProcessInfo *> list2;
    list2.append(*after);

    for (i = 0; i < before->size(); i++)
    {
        for (j = 0; j < after->size(); j++)
            if (before->at(i)->comm == after->at(j)->comm && before->at(i)->pid == after->at(j)->pid)
            {
                after->removeAt(j);
            }
    }

    for (i = 0; i < list2.size(); i++)
    {
        for (j = 0; j < before->size(); j++)
            if (before->at(j)->comm == list2.at(i)->comm && before->at(j)->pid == list2.at(i)->pid)
            {
                before->removeAt(j);
            }
    }

    for (i = 0; i < before->size(); i++)
        for (j = 0; j < runningProcessList.size(); j++)
            if (before->at(i)->comm == runningProcessList.at(j)->comm && before->at(i)->pid == runningProcessList.at(j)->pid)
            {
                runningProcessList.removeAt(j);
                break;
            }

    foreach (ProcessInfo *p, *before)
        completedProcessList.append(Report(p, snapTime.toString()));

    runningProcessList.append(*after);
}

void ProcessMonitor::saveProtocol()
{
    QString filters("Text files (*.txt)");
    QString defaultFilter("Text files (*.txt)");

    QString fileName = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(),
            filters, &defaultFilter);

    QFile file(fileName);
    QTextStream out(&file);
    file.open(QIODevice::WriteOnly);

    foreach (Report rep, completedProcessList)
        out << rep.p->comm << "|" << rep.p->pid << "|" << rep.p->stime << "|" << rep.completedTime << "\n";

    foreach (ProcessInfo *p, runningProcessList)
        out << p->comm << "|" << p->pid << "|" << p->stime << "|" << "-\n";

    file.close();
}
