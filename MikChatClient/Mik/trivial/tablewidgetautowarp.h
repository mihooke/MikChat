#ifndef TABLEWIDGETAUTOWARP_H
#define TABLEWIDGETAUTOWARP_H

#include <QStyledItemDelegate>
#include <QPainter>

class TableWidgetAutoWarp : public QStyledItemDelegate
{
    Q_OBJECT

public:
    TableWidgetAutoWarp(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // TABLEWIDGETAUTOWARP_H
