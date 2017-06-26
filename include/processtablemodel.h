#ifndef PROCESSTABLEMODEL_H
#define PROCESSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <QList>
#include <QStringList>
#include <QIcon>
#include <QStyle>
#include "processinfo.h"

class ProcessTableModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    QList<ProcessInfo *> list;


public:
    QStringList headerLabelList;
    explicit ProcessTableModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool insertRows(int position, int rows, const QModelIndex &index);
    bool removeRows(int position, int rows, const QModelIndex &index);

signals:
    
public slots:
    
};

#endif // PROCESSTABLEMODEL_H
