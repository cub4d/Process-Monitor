#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QIcon>
#include <QStyle>

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    SortFilterProxyModel(QObject *parent = 0);

protected:
    virtual bool lessThan ( const QModelIndex & left, const QModelIndex & right ) const;
};

#endif // SORTFILTERPROXYMODEL_H
