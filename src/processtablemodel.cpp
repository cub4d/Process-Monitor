#include "processtablemodel.h"

ProcessTableModel::ProcessTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    headerLabelList << "COMM" << "PID" << "STAT" << "NICE" << "%CPU" << "STIME";
}

QVariant ProcessTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= list.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        if (index.column() == 0 )
            return list.at(index.row())->comm;
        if (index.column() == 1 )
            return list.at(index.row())->pid;
        if (index.column() == 2 )
            return list.at(index.row())->stat;
        if (index.column() == 3 )
            return list.at(index.row())->nice;
        if (index.column() == 4 )
            return list.at(index.row())->cpu;
        if (index.column() == 5 )
            return list.at(index.row())->stime;
    }
    return QVariant();
}

bool ProcessTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::DisplayRole) {
        if(index.column()==0){
            list.at(index.row())->comm = value.toString();
            emit dataChanged(index, index);
        }
        if(index.column()==1){
            list.at(index.row())->pid = value.toString();
            emit dataChanged(index, index);
        }
        if(index.column()==2){
            list.at(index.row())->stat = value.toString();
            emit dataChanged(index, index);
        }
        if(index.column()==3){
            list.at(index.row())->nice = value.toString();
            emit dataChanged(index, index);
        }
        if(index.column()==4){
            list.at(index.row())->cpu = value.toString();
            emit dataChanged(index, index);
        }
        if(index.column()==5){
            list.at(index.row())->stime = value.toString();
            emit dataChanged(index, index);
        }
        return true;
    }
    return false;
}

bool ProcessTableModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    if(parent.isValid()) return false;

    beginInsertRows(parent, position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        list.insert(position, new ProcessInfo());
    }

    endInsertRows();

    return true;
}

bool ProcessTableModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    if(parent.isValid()) return false;

    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        list.removeAt(position);
    }

    endRemoveRows();

    return true;
}

int ProcessTableModel::rowCount(const QModelIndex &parent) const
{
    return list.size();
}

int ProcessTableModel::columnCount(const QModelIndex &parent) const
{
    return headerLabelList.size();
}

QVariant ProcessTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if(role != Qt::DisplayRole)
           return QAbstractTableModel::headerData(section, orientation, role);
   if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
   {
       return headerLabelList.at(section); 
   }
   else
   {
       return QString("%1").arg( section + 1 ); 
   }
}

Qt::ItemFlags ProcessTableModel::flags(const QModelIndex &index) const
{
   if (!index.isValid())
        return Qt::ItemIsEnabled;

   return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
