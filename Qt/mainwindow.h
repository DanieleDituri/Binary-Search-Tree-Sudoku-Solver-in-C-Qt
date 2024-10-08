#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_clearButton_clicked();
    void on_resolveButton_clicked();
    void on_cellTextChanged(const QString &text);
    bool checkIfAllCellsAreValid();
    void on_prevButton_clicked();
    void on_nextButton_clicked();

private:
    Ui::MainWindow *ui;
    QLineEdit *sudokuCells[9][9];
    QList<int> results;
    QList<int> rRow;
    QList<int> rCol;
    int rIndex;
    void setSudokuSolved(bool solved);
    void clearSudoku();
    bool isSafe(int row, int col, int num);
    bool solveSudoku(int row, int col);
};

#endif // MAINWINDOW_H
