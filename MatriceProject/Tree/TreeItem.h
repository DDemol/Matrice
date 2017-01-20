#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVector>
#include <QVariant>

enum ItemType {DIRECTORY, CURVE};

class TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant> &data, ItemType type, TreeItem *parent = 0);
    ~TreeItem();

    TreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildrenCurve(int position, int count, int columns);
    bool insertChildrenDirectory(int position, int count, int columns);

    bool insertColumns(int position, int columns);
    TreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

    void setType(ItemType type);
    ItemType getType();

private:
    QList<TreeItem*> childItems;
    QVector<QVariant> itemData;
    TreeItem *parentItem;
    ItemType _type;
};
#endif // TREEITEM_H
