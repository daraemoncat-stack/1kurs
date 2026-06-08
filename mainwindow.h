#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "treewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onClearClicked();

private:
    Tree* m_tree;
    TreeWidget* m_treeWidget;
    QLineEdit* m_inputField;
    QPushButton* m_addButton;
    QPushButton* m_removeButton;
    QPushButton* m_clearButton;
};

#endif // MAINWINDOW_H
