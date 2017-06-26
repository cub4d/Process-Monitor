#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <QString>

class ProcessInfo
{
public:
    ProcessInfo();
    ProcessInfo(ProcessInfo *pi);
    QString comm;
    QString stat;
    QString pid;
    QString nice;
    QString cpu;
    QString stime;
};

#endif // PROCESSINFO_H
