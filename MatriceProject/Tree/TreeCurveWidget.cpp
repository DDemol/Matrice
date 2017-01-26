#include "TreeCurveWidget.h"


TreeCurveWidget::TreeCurveWidget(QList<CurveItem> *curves) : QTreeView()
{
    QStringList headers;
    headers << tr("Curves");
    _model = new TreeModel(headers, "");
    _curves = curves;
    _nbRow = 0;
    _nbDirectory = 0;

    setModel(_model);
    viewport()->setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);

    /*
    model->insertRow(0, index.parent());
    QModelIndex par1 = model->index(0, 0);
    model->setData(par1, QVariant("A"), Qt::EditRole);

    model->insertRow(1, index.parent());
    QModelIndex par2 = model->index(1, 0);
    model->setData(par2, QVariant("B"), Qt::EditRole);

    model->insertRow(0, par1);
    QModelIndex child = model->index(0, 0,par1);

    model->setData(child, QVariant("C"), Qt::EditRole);
*/
    insertDirectory("/Directory1");


    connect(_model, SIGNAL(dataTreeChanged(QVariant,QVariant)),this, SLOT(sendNewData(QVariant,QVariant)));

}

void TreeCurveWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeView::mousePressEvent(event);

    QAbstractItemModel *model = _model;
    QModelIndex index = this->indexAt(event->pos());
    //  std::cout << index.row() << std::endl;

    if(index.row() == -1)
    {
        std::cout << "-1" << std::endl;
        emit selectedRow("");
    }
    else
    {
        QString var = model->data(index).toString();

        if(var.at(0) == "/")
            emit selectedRows(extractStringsFromModel(model,index));
        else
            emit selectedRow(var);
    }
}

void TreeCurveWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = selectionModel()->currentIndex();
    //   QAbstractItemModel *model = _model;

    edit(index, DoubleClicked, event);
    //   model->setData(index,)

}

void TreeCurveWidget::insertRow(CurveItem item)
{
    QModelIndex index = selectionModel()->currentIndex();
    QAbstractItemModel *model = _model;

    if (!model->insertRow(_nbRow, currentDirectory))
        return;

    for (int column = 0; column < model->columnCount(index.parent()); ++column)
    {
        QModelIndex child = model->index(_nbRow, column, currentDirectory);
        model->setData(child, QVariant(item.getName()), Qt::EditRole);
    }

    //  QModelIndex test = model->index(model->rowCount()-1,0,index.parent());
    //  QVariant var = model->data(test);
    // qDebug( std::to_string(model->rowCount()).c_str());

    /*
    QVariant var = findRow("TT");

    QString ss = qvariant_cast<QString>(var);

    qDebug(ss.toLatin1());*/

    expandAll();
    _nbRow++;
}

void TreeCurveWidget::insertDirectory(QString name)
{
    QModelIndex index = selectionModel()->currentIndex();
    //   QAbstractItemModel *model = _model;

    if (!_model->insertDirectory(_nbRow,1, index.parent()))
        return;

    for (int column = 0; column < _model->columnCount(index.parent()); ++column)
    {
        QModelIndex child = _model->index(_nbRow, column, index.parent());
        _model->setData(child, QVariant(name), Qt::EditRole);

        if(_nbDirectory == 0)
        {
            currentDirectory = child;
        }
    }

    _nbDirectory++;
}

void TreeCurveWidget::removeRow(CurveItem item)
{
    QAbstractItemModel *model = _model;
    QModelIndexList items = _model->match(_model->index(0,0),
                                          Qt::EditRole,
                                          QVariant::fromValue(item.getName()),
                                          2,
                                          Qt::MatchRecursive);

    if(! items.isEmpty())
    {
        for(int i = 0 ; i < items.size() ; i++)
        {
            model->removeRow(items.at(i).row(),items.at(i).parent());
            _nbRow--;
        }
    }
}

void TreeCurveWidget::removeAll(QList <CurveItem> l)
{
    for(int i = 0 ; i < l.size() ; i++)
    {
        removeRow(l.at(i));
    }

}

void TreeCurveWidget::sendNewData(const QVariant &oldValue, const QVariant &value)
{
    QString oldV = qvariant_cast<QString>(oldValue);
    QString newV = qvariant_cast<QString>(value);

    emit sendNewName(oldV,newV);
}

QStringList TreeCurveWidget::extractStringsFromModel(QAbstractItemModel *model, const QModelIndex &parent)
{
    QStringList retval;

    int rowCount = model->rowCount(parent);

    for(int i = 0; i < rowCount; ++i)
    {
        QModelIndex idx = model->index(i, 0, parent);

        if(idx.isValid())
        {
            retval << idx.data(Qt::EditRole).toString();
            retval << extractStringsFromModel(model, idx);
        }
    }

    return retval;
}

std::pair<QModelIndex,int> TreeCurveWidget::findRow(QString s, const QModelIndex &parent)
{
    int rowCount = _model->rowCount(parent);

    for(int i = 0; i < rowCount; ++i)
    {
        QModelIndex idx = _model->index(i, 0, parent);

        if(idx.isValid())
        {
            if(idx.data(Qt::EditRole).toString() == s)
            {
                return std::make_pair(idx,i);
            }
            else
                findRow(s,idx);
        }
    }
}
