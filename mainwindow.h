#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QTableWidget>
#include <QSettings>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    static constexpr int BSize = 9; // размер доски
    static constexpr int QSize = 3; // размер квадрата/блока
    QLineEdit* cells[BSize][BSize];
    int board[BSize][BSize];
    QComboBox* DifficultyBox;
    QTimer* GameTimer;
    QTime* StartTime;
    QLabel* TimerLabel;
    int seconds;

    void SetupUI();
    void CellStyle(QLineEdit* cell, int i, int j, bool error = false);
    void MakeBoard();
    bool FillBoard(int row, int col);
    bool SafePlace(int row, int col, int num);
    void UniqPuzzle(int removeCount);
    bool SolveCount(int& count);
    void ShowBoard();
    bool RightSolution();
    void ResetTimer();
    void ShowHighScores();
    void SetupMainWindow();
    void ShowStartWindow();

private slots:
    void CheckSolution();
    void NewGame();
    void UpdateTimer();
};

#endif // MAINWINDOW_H
