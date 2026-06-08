// treewidget.cpp
#include "treewidget.h"
#include <QPainter>
#include <cmath>

TreeWidget::TreeWidget(QWidget *parent) : QWidget(parent), m_tree(nullptr) {}

void TreeWidget::setTree(Tree* tree)
{
    m_tree = tree;
    update();
}

void TreeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (!m_tree) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Настройки отрисовки
    int width = this->width();
    int height = this->height();
    int hSpacing = width / 4;  // Горизонтальный интервал
    int vSpacing = 50;         // Вертикальный интервал

    // Очищаем фон
    painter.fillRect(rect(), Qt::white);

    // Рисуем дерево
    drawTree(&painter, m_tree, width / 2, 30, hSpacing, vSpacing, 0);
}

void TreeWidget::drawTree(QPainter* painter, Tree* node, int x, int y, int hSpacing, int vSpacing, int level)
{
    if (!node) return;

    // Настройки кисти и пера
    painter->setPen(Qt::black);
    painter->setBrush(Qt::lightGray);

    // Рисуем узел (круг)
    int radius = 20;
    painter->drawEllipse(QPoint(x, y), radius, radius);

    // Текст внутри узла
    painter->drawText(QRect(x - radius, y - radius, 2 * radius, 2 * radius),
                      Qt::AlignCenter, QString(node->get_data()));

    // Рекурсивно рисуем левое и правое поддеревья
    if (node->get_left())
    {
        int leftX = x - hSpacing;
        int leftY = y + vSpacing;

        // Линия к левому узлу
        painter->drawLine(x, y + radius, leftX, leftY - radius);

        drawTree(painter, node->get_left(), leftX, leftY, hSpacing / 2, vSpacing, level + 1);
    }

    if (node->get_right())
    {
        int rightX = x + hSpacing;
        int rightY = y + vSpacing;

        // Линия к правому узлу
        painter->drawLine(x, y + radius, rightX, rightY - radius);

        drawTree(painter, node->get_right(), rightX, rightY, hSpacing / 2, vSpacing, level + 1);
    }
}
