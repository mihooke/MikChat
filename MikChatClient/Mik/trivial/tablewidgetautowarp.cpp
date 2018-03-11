#include <QTextEdit>
#include "tablewidgetautowarp.h"

TableWidgetAutoWarp::TableWidgetAutoWarp(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

QWidget *TableWidgetAutoWarp::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTextEdit *editor = new QTextEdit(parent);
    return editor;
}

void TableWidgetAutoWarp::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QTextEdit *newEditor = static_cast<QTextEdit*>(editor);
    newEditor->setPlainText(value);
}

void TableWidgetAutoWarp::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTextEdit *newEditor = static_cast<QTextEdit*>(editor);
    QString value = newEditor->toPlainText();
    model->setData(index, value, Qt::EditRole);
}

void TableWidgetAutoWarp::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

