#include "processinfo.h"

ProcessInfo::ProcessInfo()
{
    this->comm  = "";
    this->cpu   = "";
    this->nice  = "";
    this->pid   = "";
    this->stat  = "";
    this->stime = "";
}

ProcessInfo::ProcessInfo(ProcessInfo *pi)
{
    this->comm  = pi->comm;
    this->cpu   = pi->cpu;
    this->nice  = pi->nice;
    this->pid   = pi->pid;
    this->stat  = pi->stat;
    this->stime = pi->stime;
}
