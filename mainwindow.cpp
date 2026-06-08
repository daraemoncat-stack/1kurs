#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRegExpValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Создаем пустое дерево
    m_tree = new Tree();

    // Создаем элементы интерфейса
    m_treeWidget = new TreeWidget(this);
    m_inputField = new QLineEdit(this);
    m_addButton = new QPushButton("Добавить", this);
    m_removeButton = new QPushButton("Удалить", this);
    m_clearButton = new QPushButton("Очистить дерево", this);

    // Настраиваем валидацию для ввода (только один символ)
    m_inputField->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]"), this));
    m_inputField->setMaxLength(1);

    // Соединяем кнопки с слотами
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(m_removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    // Создаем компоновку
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Панель управления
    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(new QLabel("Элемент:"));
    controlLayout->addWidget(m_inputField);
    controlLayout->addWidget(m_addButton);
    controlLayout->addWidget(m_removeButton);
    controlLayout->addWidget(m_clearButton);

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(m_treeWidget);

    setCentralWidget(centralWidget);
    resize(800, 600);
    setWindowTitle("Визуализатор двоичного дерева");
}

MainWindow::~MainWindow()
{
    delete m_tree;
}

void MainWindow::onAddClicked()
{
    QString text = m_inputField->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите символ для добавления");
        return;
    }

    char value = text.at(0).toLatin1();

    if (m_tree->find(value)) {
        QMessageBox::information(this, "Информация", "Элемент уже существует в дереве");
        return;
    }

    if (m_tree->get_data() == '\0') { // Если дерево пустое
        delete m_tree;
        m_tree = new Tree(value);
    } else {
        m_tree->insert(value);
    }

    m_treeWidget->setTree(m_tree);
    m_inputField->clear();
}

void MainWindow::onRemoveClicked()
{
    QString text = m_inputField->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите символ для удаления");
        return;
    }

    char value = text.at(0).toLatin1();

    // Здесь должна быть реализация удаления узла из дерева
    // В текущей реализации класса Tree нет метода удаления узла,
    // поэтому просто покажем сообщение
    QMessageBox::information(this, "Информация",
                             "Метод удаления узла не реализован в классе Tree");

    m_inputField->clear();
}

void MainWindow::onClearClicked()
{
    delete m_tree;
    m_tree = new Tree();
    m_treeWidget->setTree(m_tree);
}
