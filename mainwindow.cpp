#include "mainwindow.h"
#include <QGridLayout>
#include <QPushButton>
#include <QIntValidator>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QTime>
#include <QTimer>
#include <QSettings>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    seconds(0)
{
    ShowStartWindow();
}

MainWindow::~MainWindow()
{
    delete GameTimer;
    delete StartTime;
}

void MainWindow::ShowStartWindow()
{
    QDialog* startDialog = new QDialog(this);
    startDialog->setWindowTitle(QString::fromUtf8("Судоку"));
    startDialog->setFixedSize(300, 200);
    startDialog->setStyleSheet("background-color: #E6F0FA;");

    QVBoxLayout* startLayout = new QVBoxLayout(startDialog);
    startLayout->setAlignment(Qt::AlignCenter);

    QLabel* titleLabel = new QLabel(QString::fromUtf8(u8"Судоку"), startDialog);
    titleLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #1A3C5A; }");
    titleLabel->setAlignment(Qt::AlignCenter);

    QPushButton* startButton = new QPushButton(QString::fromUtf8(u8"Старт"), startDialog);
    startButton->setStyleSheet(
        "QPushButton { background-color: #42A5F5; color: white; "
        "font-size: 16px; font-weight: bold; padding: 10px; border: none; "
        "border-radius: 5px; min-width: 120px; } "
        "QPushButton:hover { background-color: #1E88E5; }");
    connect(startButton, &QPushButton::clicked, startDialog, &QDialog::accept);

    startLayout->addStretch();
    startLayout->addWidget(titleLabel);
    startLayout->addSpacing(20);
    startLayout->addWidget(startButton);
    startLayout->addStretch();

    if (startDialog->exec() == QDialog::Accepted) {
        SetupMainWindow();
    }
    delete startDialog;
}

void MainWindow::SetupMainWindow()
{
    SetupUI();
    setWindowTitle(QString::fromUtf8(u8"Судоку"));
    MakeBoard();
    setFixedSize(500, 600);
}

void MainWindow::SetupUI() {
    QWidget* centralWidget = new QWidget(this);//контейнер для других виджетов
    setCentralWidget(centralWidget);//становится частью структуры окна
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    centralWidget->setStyleSheet("background-color: #E6F0FA;");

    QGridLayout* boardLayout = new QGridLayout();
    boardLayout->setSpacing(0);//расстояние между клетками 0
    boardLayout->setContentsMargins(15, 15, 15, 15);//расстояние до края centralWidget
    for (int i = 0; i < BSize; i++) {
        for (int j = 0; j < BSize; j++) {
            cells[i][j] = new QLineEdit(this);
            cells[i][j]->setMaxLength(1);//можно вписать только один символ
            cells[i][j]->setAlignment(Qt::AlignCenter);//цифра по центру
            cells[i][j]->setFixedSize(50, 50);
            cells[i][j]->setValidator(new QIntValidator(1, 9, this));//можно вводить только 1-9
            CellStyle(cells[i][j], i, j);
            boardLayout->addWidget(cells[i][j], i, j);
        }
    }

    QPushButton* checkButton = new QPushButton(QString::fromUtf8(u8"Проверить решение"), this);
    checkButton->setStyleSheet(
        "QPushButton { background-color: #42A5F5; color: white; "
        "font-size: 16px; font-weight: bold; padding: 10px; border: none; "
        "border-radius: 5px; min-width: 120px; } "
        "QPushButton:hover { background-color: #1E88E5; }");//цвет при наведении
    connect(checkButton, &QPushButton::clicked, this, &MainWindow::CheckSolution);

    QHBoxLayout* controls = new QHBoxLayout();
    controls->addStretch();

    QVBoxLayout* difficultyLayout = new QVBoxLayout();
    DifficultyBox = new QComboBox(this);
    DifficultyBox->addItems({QString::fromUtf8(u8"Низкая сложность"),
                             QString::fromUtf8(u8"Средняя сложность"),
                             QString::fromUtf8(u8"Высокая сложность")});
    DifficultyBox->setStyleSheet(
        "QComboBox { background-color: #B3E5FC; color: #1A3C5A; padding: 7px; font-size: 16px; "
        "border: 1px solid #4A90E2; border-radius: 5px; } "
        "QComboBox QAbstractItemView { background-color: #FFFFFF; color: #1A3C5A; }");
    difficultyLayout->addWidget(DifficultyBox);

    QPushButton* newGameButton = new QPushButton(QString::fromUtf8(u8"Новая игра"), this);
    newGameButton->setStyleSheet(
        "QPushButton { background-color: #0288D1; color: white; "
        "font-size: 16px; font-weight: bold; padding: 10px; border: none; "
        "border-radius: 5px;}"
        "QPushButton:hover { background-color: #0277BD; }");
    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::NewGame);

    QPushButton* highScoresButton = new QPushButton(QString::fromUtf8(u8"Рекорды"), this);
    highScoresButton->setStyleSheet(
        "QPushButton { background-color: #90CAF9; color: white; "
        "font-size: 16px; font-weight: bold; padding: 10px; border: none; "
        "border-radius: 5px;}"
        "QPushButton:hover { background-color: #64B5F6; }");
    connect(highScoresButton, &QPushButton::clicked, this, &MainWindow::ShowHighScores);

    TimerLabel = new QLabel("00:00", this);
    TimerLabel->setStyleSheet(
        "QLabel { font-size: 16px; font-weight: bold; color: #1A3C5A; padding: 15px; "
        "background-color: #E3F2FD;}");
    TimerLabel->setAlignment(Qt::AlignCenter);
    GameTimer = new QTimer(this);
    StartTime = new QTime(0, 0, 0);
    connect(GameTimer, &QTimer::timeout, this, &MainWindow::UpdateTimer);

    controls->addLayout(difficultyLayout);
    controls->addWidget(TimerLabel);
    controls->addWidget(newGameButton);
    controls->addWidget(highScoresButton);
    controls->addStretch(); // Добавляем растяжку справа для центрирования

    mainLayout->addLayout(controls);
    mainLayout->addLayout(boardLayout);
    mainLayout->addSpacing(15);//отступ между задачей и кнопкой проверки решения
    mainLayout->addWidget(checkButton);
    mainLayout->addStretch();//все элементы сверху
}

void MainWindow::CellStyle(QLineEdit* cell, int i, int j, bool error) {
    //QString baseColor = "#ffffff";
    QString borders;
    if (i % QSize == 0) borders += "border-top: 2px solid #4A90E2;";
    if (j % QSize == 0) borders += "border-left: 2px solid #4A90E2;";
    if (i == BSize - 1) borders += "border-bottom: 2px solid #4A90E2;";
    if (j == BSize - 1) borders += "border-right: 2px solid #4A90E2;";
    QString style = QString(
                        "QLineEdit { font-size: 18px; font-weight: bold; border: 1px solid #B0BEC5; "
                        "color: #4A90E2; background-color: #ffffff; %1 } "
                        "QLineEdit:read-only { color: #1A3C5A; } "
                        ).arg(borders);

    if (error) style += "QLineEdit { border: 2px solid red; }";
    cell->setStyleSheet(style);
}

void MainWindow::MakeBoard() {
    std::memset(board, 0, sizeof(board));//заполняет память, выделенную под board, нулями
    FillBoard(0, 0);//заполнет поле цифрами, начиная с клетки (0,0)
    UniqPuzzle(40);//по умолчанию удаляется 40 клеток
    ShowBoard();
    ResetTimer();//таймер сбрасывается до нуля
}

bool MainWindow::FillBoard(int row, int col) {
    if (row == BSize) return true;
    int nextRow = (col == BSize - 1) ? row + 1 : row;
    int nextCol = (col + 1) % BSize;

    QVector<int> nums;
    for (int i = 1; i <= BSize; i++) nums.append(i);//заполнение nums цифрами 1-9
    std::shuffle(nums.begin(), nums.end(), *QRandomGenerator::global());//в nums цифры переставляются случайным образом

    for (int num : nums) {
        if (SafePlace(row, col, num)) {
            board[row][col] = num;//если цифра допустима, то она записывается в клетку
            if (FillBoard(nextRow, nextCol)) return true;
            board[row][col] = 0;
        }
    }
    return false;//нужно попробовать другую цифру
}

bool MainWindow::SafePlace(int row, int col, int num) {
    //Проверка строки
    for (int i = 0; i < BSize; i++) {
        if (i != col && board[row][i] == num) return false;
    }
    //Проверка столбца
    for (int i = 0; i < BSize; i++) {
        if (i != row && board[i][col] == num) return false;
    }
    //Проверка квадрата 3*3
    int rowStart = (row / QSize) * QSize;
    int colStart = (col / QSize) * QSize;
    for (int i = rowStart; i < rowStart + QSize; i++) {
        for (int j = colStart; j < colStart + QSize; j++) {
            if ((i != row || j != col) && board[i][j] == num) return false;
        }
    }
    return true;
}

void MainWindow::UniqPuzzle(int removeCount) {
    //Создание массива с координатами всех клеток
    QVector<QPair<int, int>> cells;
    for (int i = 0; i < BSize; i++) {
        for (int j = 0; j < BSize; j++) {
            cells.append({i, j});
        }
    }
    std::shuffle(cells.begin(), cells.end(), *QRandomGenerator::global());//координаты в случайном порядке

    for (const auto& [i, j] : cells) {
        int backup = board[i][j];
        board[i][j] = 0;
        int count = 0;//Количество решений устанавливается на 0
        SolveCount(count);//в count записывает количество решений для текущего состояния доски
        if (count != 1) board[i][j] = backup;//если количество решений не 1, то значение клетки восстанавливается
        if (--removeCount <= 0) break;
    }
}

bool MainWindow::SolveCount(int& count) {
    if (count > 1) return true;
    for (int row = 0; row < BSize; row++) {
        for (int col = 0; col < BSize; col++) {
            //Заполнение пустой клетки цифрой
            if (board[row][col] == 0) {
                for (int num = 1; num <= BSize; num++) {
                    if (SafePlace(row, col, num)) {
                        board[row][col] = num;
                        SolveCount(count);
                        board[row][col] = 0;
                    }
                }
                return false;//если ни одна цифра не подошла, то возвращает false
            }
        }
    }
    count++;//если все клетки заполнены, то количество решенйи увеличивается
    return count <= 1;
}

void MainWindow::ShowBoard() {
    for (int i = 0; i < BSize; i++) {
        for (int j = 0; j < BSize; j++) {
            CellStyle(cells[i][j], i, j);
            if (board[i][j]) {
                cells[i][j]->setText(QString::number(board[i][j]));
                cells[i][j]->setReadOnly(true);
            } else {
                cells[i][j]->clear();
                cells[i][j]->setReadOnly(false);
            }
        }
    }
}

void MainWindow::CheckSolution() {
    //Сброс стилей ячеек
    for (int i = 0; i < BSize; i++) {
        for (int j = 0; j < BSize; j++) {
            CellStyle(cells[i][j], i, j);
        }
    }
    for (int i = 0; i < BSize; i++) {
        for (int j = 0; j < BSize; j++) {
            QString val = cells[i][j]->text();
            board[i][j] = val.isEmpty() ? 0 : val.toInt();
        }
    }
    //Проверка на ошибки и пустые клетки
    bool errors = false;
    for (int i = 0; i < BSize; i++) {
        for (int j = 0; j < BSize; j++) {
            if (board[i][j] == 0) {
                errors = true;
                continue;
            }
            int temp = board[i][j];
            board[i][j] = 0;
            if (!SafePlace(i, j, temp)) {
                CellStyle(cells[i][j], i, j, true);
                errors = true;
            }
            board[i][j] = temp;
        }
    }
    if (!errors && RightSolution()) {
        GameTimer->stop();
        QSettings settings("MyCompany", "Sudoku");
        QString difficulty = DifficultyBox->currentText();
        int bestTime = settings.value(difficulty, 999999).toInt();
        if (seconds < bestTime && seconds > 0) {
            settings.setValue(difficulty, seconds);
            settings.sync();
            QMessageBox::information(this, QString::fromUtf8(u8"Судоку"),QString::fromUtf8(u8"Новый рекорд: %1 секунд!").arg(seconds));
        }
        QMessageBox::information(this, QString::fromUtf8(u8"Судоку"),QString::fromUtf8(u8"Судоку решено верно."));
    }
    else {
        QMessageBox::warning(this, QString::fromUtf8(u8"Судоку"),QString::fromUtf8(u8"Судоку решено неверно или не полностью."));
    }
}

bool MainWindow::RightSolution() {
    for (int i = 0; i < BSize; i++) {
        for (int j = 0; j < BSize; j++) {
            if (board[i][j] == 0) continue; //переход к следующей клетке
            int temp = board[i][j];
            board[i][j] = 0;
            if (!SafePlace(i, j, temp)) {
                board[i][j] = temp;
                return false;
            }
            board[i][j] = temp;
        }
    }
    return true;
}

void MainWindow::NewGame() {
    int removeCounts[] = {30, 40, 50};
    int removeCount = removeCounts[DifficultyBox->currentIndex()];//в зависимости от индекса выбранной сложности удаляется 30, 40 или 50 клеток
    std::memset(board, 0, sizeof(board));//memset заполняет весь массив нулями, эффективно очищая доску
    FillBoard(0, 0);//вызов функции FillBoard, начиная с клетки с координатой (0,0)
    UniqPuzzle(removeCount);
    ShowBoard();
    ResetTimer();
}

void MainWindow::ResetTimer() {
    GameTimer->stop();//работа таймера прекращается
    seconds = 0;//сброс счётчика
    TimerLabel->setText("00:00");
    GameTimer->start(1000);//запуск таймера с интервалом 1000 миллисекунд
}

void MainWindow::UpdateTimer() {
    seconds++;
    QTime displayTime(0, 0, 0);
    displayTime = displayTime.addSecs(seconds);//addSecs(seconds) добавляет указанное количество секунд (seconds) к текущему значению displayTime
    TimerLabel->setText(displayTime.toString("mm:ss"));
}

void MainWindow::ShowHighScores() {
    QDialog* dialog = new QDialog(this);
    dialog->setStyleSheet("background-color: #E6F0FA;");
    dialog->setWindowTitle(QString::fromUtf8(u8"Рекорды"));//задаёт заголовок окна
    dialog->setFixedSize(300, 200);
    QVBoxLayout* dialogLayout = new QVBoxLayout(dialog);//вертикальная компановка
    //Создание таблицы рекордов
    QTableWidget* highScoresTable = new QTableWidget(3, 2, dialog);//создаётся три строки и два столбца
    highScoresTable->setHorizontalHeaderLabels({QString::fromUtf8(u8"Сложность"), QString::fromUtf8(u8"Лучшее время")});
    highScoresTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//запрет на редактирование ячеек таблицы
    highScoresTable->setSelectionMode(QAbstractItemView::NoSelection);//отключает выделение ячеек, делая таблицу только для просмотра.
    highScoresTable->verticalHeader()->setVisible(false);//скрывает номера строк
    highScoresTable->setStyleSheet(
        "QTableWidget { font-size: 14px; background-color: #FFFFFF; color: #1A3C5A; "
        "border: 1px solid #4A90E2; } "
        "QHeaderView::section { background-color: #42A5F5; color: #FFFFFF; }");
    QSettings settings("MyCompany", "Sudoku");//создаётся объект QSettings с организацией "SudokuRecords" и приложением "Sudoku"
    QStringList difficulties = {QString::fromUtf8(u8"Низкая сложность"),
                                QString::fromUtf8(u8"Средняя сложность"),
                                QString::fromUtf8(u8"Высокая сложность")};
    //заполнение таблицы рекордов
    for (int i = 0; i < difficulties.size(); ++i) {
        highScoresTable->setItem(i, 0, new QTableWidgetItem(difficulties[i]));
        int bestTime = settings.value(difficulties[i], 999999).toInt();
        QString timeStr = bestTime == 999999 ? QString::fromUtf8(u8"Нет рекорда") : QTime(0, 0, 0).addSecs(bestTime).toString("mm:ss");
        highScoresTable->setItem(i, 1, new QTableWidgetItem(timeStr));
    }
    highScoresTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//настройка ширины таблицы
    dialogLayout->addWidget(highScoresTable);
    QPushButton* closeButton = new QPushButton(QString::fromUtf8(u8"Закрыть"), dialog);
    closeButton->setStyleSheet(
        "QPushButton { background-color: #42A5F5; color: white; "
        "font-size: 16px; font-weight: bold; padding: 10px; border: none; "
        "border-radius: 5px; } "
        "QPushButton:hover { background-color: #1E88E5; }");
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    dialogLayout->addWidget(closeButton);
    dialog->exec();//открывает диалог как модальное окно, блокируя взаимодействие с главным окном, пока диалог не закрыт
    delete dialog;
}
