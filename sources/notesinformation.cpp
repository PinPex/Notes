#include "notesinformation.h"

NotesInformation::NotesInformation(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant NotesInformation::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex NotesInformation::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex NotesInformation::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int NotesInformation::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int NotesInformation::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant NotesInformation::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
