#include "sortfilterproxymodel.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    QSortFilterProxyModel::setSortCaseSensitivity(Qt::CaseInsensitive);
}

bool SortFilterProxyModel::lessThan(const QModelIndex &left,
                                    const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    if (leftData.toDouble() && rightData.toDouble()) {
        return leftData.toDouble() < rightData.toDouble();
    } else {
        return QSortFilterProxyModel::lessThan(left, right);
    }
}
