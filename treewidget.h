// treewidget.h
#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include "tree.h"

class TreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TreeWidget(QWidget *parent = nullptr);
    void setTree(Tree* tree);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Tree* m_tree;
    void drawTree(QPainter* painter, Tree* node, int x, int y, int hSpacing, int vSpacing, int level);
};

#endif // TREEWIDGET_H
