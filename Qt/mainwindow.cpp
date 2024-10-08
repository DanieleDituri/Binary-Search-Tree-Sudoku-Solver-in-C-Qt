#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myvalidator.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QFrame>
#include <QFont>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            sudokuCells[row][col] = nullptr;
        }
    }

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QGridLayout *sudokuLayout = new QGridLayout();
    sudokuLayout->setSpacing(0);
    mainLayout->addLayout(sudokuLayout);

    QPushButton *resolveButton = new QPushButton("Resolve", ui->resolveButton);
    connect(resolveButton, &QPushButton::clicked, this, &MainWindow::on_resolveButton_clicked);

    QPushButton *prevButton = new QPushButton("Previous", ui->prevButton);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::on_prevButton_clicked);

    QPushButton *nextButton = new QPushButton("Next", ui->nextButton);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::on_nextButton_clicked);


    buttonLayout->addWidget(prevButton);
    buttonLayout->addWidget(nextButton);

    QPushButton *clearButton = new QPushButton("Clear", ui->clearButton);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);

    mainLayout->addWidget(resolveButton);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(clearButton);


    QGridLayout *singleSudokuLayout[9];
    QFrame *singleSudokuFrame[9];

    for (int i = 0; i < 9; ++i)
    {
        singleSudokuFrame[i] = new QFrame;
        singleSudokuFrame[i]->setFrameShape(QFrame::Box);
        singleSudokuFrame[i]->setLineWidth(3);
        singleSudokuLayout[i] = new QGridLayout(singleSudokuFrame[i]);
        singleSudokuLayout[i]->setSpacing(0);

    }

    QFont boldFont;
    boldFont.setBold(true);
    boldFont.setPointSize(16);

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            sudokuCells[row][col] = new QLineEdit;
            sudokuCells[row][col]->setAlignment(Qt::AlignCenter);
            sudokuCells[row][col]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            sudokuCells[row][col]->setMaxLength(1);
            sudokuCells[row][col]->setValidator(new MyValidator(sudokuCells[row][col]));
            sudokuCells[row][col]->setFont(boldFont);
            sudokuCells[row][col]->setMinimumWidth(60);
            sudokuCells[row][col]->setMinimumHeight(60);

            int subgridIndex = (row / 3) * 3 + (col / 3);
            int subgridRow = row % 3;
            int subgridCol = col % 3;

            singleSudokuLayout[subgridIndex]->addWidget(sudokuCells[row][col], subgridRow, subgridCol);

            connect(sudokuCells[row][col], &QLineEdit::textChanged, this, &MainWindow::on_cellTextChanged);
        }
    }

    int tmp = 0;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            sudokuLayout->addWidget(singleSudokuFrame[tmp],i, j);
            ++tmp;
        }
    }


    setWindowTitle("Sudoku");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clearButton_clicked()
{
    qDebug() << "Clear button clicked!";
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Clear Sudoku", "Are you sure you want to delete the sudoku?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        clearSudoku();
    }
}

void MainWindow::clearSudoku()
{
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (sudokuCells[row][col] != nullptr)
            {
                qDebug() << "Clearing cell: " <<row << col << sudokuCells[row][col]->text();
                sudokuCells[row][col]->clear();

                results.clear();
                rRow.clear();
                rCol.clear();
            }
            else
                qDebug() << "Null pointer at cell: " <<row << col;
        }
    }
    setSudokuSolved(false);
}

void MainWindow::on_cellTextChanged(const QString &text)
{
    QLineEdit *senderCell = qobject_cast<QLineEdit*>(sender());
    if (!senderCell) return;

    int row, col;
    bool found = false;

    for (int r = 0; r < 9 && !found; ++r)
    {
        for (int c = 0; c < 9 && !found; ++c)
        {
            if (sudokuCells[r][c] == senderCell)
            {
                row = r;
                col = c;
                found = true;
            }
        }
    }

    if (text.isEmpty())
    {
        QPalette palette = senderCell->palette();
        palette.setColor(QPalette::Text, Qt::black);
        senderCell->setPalette(palette);
        return;
    }

    int num = text.toInt();
    if (isSafe(row, col, num))
    {
        QPalette palette = senderCell->palette();
        palette.setColor(QPalette::Text, Qt::black);
        senderCell->setPalette(palette);
    } else
    {
        QPalette palette = senderCell->palette();
        palette.setColor(QPalette::Text, Qt::red);
        senderCell->setPalette(palette);
    }
}

bool MainWindow::isSafe(int row, int col, int num)
{
    QString numStr = QString::number(num);

    for (int x = 0; x < 9; ++x) {
        if (x != col && sudokuCells[row][x]->text() == numStr) {
            return false;
        }
    }

    for (int x = 0; x < 9; ++x) {
        if (x != row && sudokuCells[x][col]->text() == numStr) {
            return false;
        }
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int r = startRow + i;
            int c = startCol + j;
            if ((r != row || c != col) && sudokuCells[r][c]->text() == numStr) {
                return false;
            }
        }
    }

    return true;
}

bool MainWindow::solveSudoku(int row, int col)
{
    if (row == 9 - 1 && col == 9)
    {
        return true;
    }

    if (col == 9)
    {
        row++;
        col = 0;
    }

    if (!sudokuCells[row][col]->text().isEmpty())
    {
        return solveSudoku(row, col + 1);
    }

    for (int num = 1; num <= 9; ++num)
    {
        if (isSafe(row, col, num))
        {
            sudokuCells[row][col]->setText(QString::number(num));
            results.append(num);
            rRow.append(row);
            rCol.append(col);
            if (solveSudoku(row, col + 1))
            {
                return true;
            }
            sudokuCells[row][col]->clear();
            results.append(0);
            rRow.append(row);
            rCol.append(col);
        }
    }

    return false;
}

bool MainWindow::checkIfAllCellsAreValid()
{
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (sudokuCells[row][col])
            {
                QPalette palette = sudokuCells[row][col]->palette();
                if (palette.color(QPalette::Text) == Qt::red)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

void MainWindow::on_resolveButton_clicked() {
    if (checkIfAllCellsAreValid())
    {
        if (!solveSudoku(0, 0))
        {
            QMessageBox::warning(this, "Sudoku", "No solution exists!");
        }
        else
        {
            rIndex = results.size() - 1;
            setSudokuSolved(true);
        }
    }
    else
    {
        QMessageBox::warning(this, "Sudoku", "Invalid cells are present, delete them manually or press clear so you can then resolve it");
    }
}

void MainWindow::on_prevButton_clicked()
{
    if(results.isEmpty() || rIndex < 0)
    {
        QMessageBox::warning(this, "Previous", "No previous moves are present");
    }
    else
    {
        qDebug() << rIndex;
        if(results[rIndex] == 0){
            sudokuCells[rRow[rIndex]][rCol[rIndex]]->clear();
            --rIndex;
            qDebug() << "Previuos clear";
        }
        else
        {
            if(sudokuCells[rRow[rIndex]][rCol[rIndex]]->text().toInt() == results[rIndex])
            {
                sudokuCells[rRow[rIndex]][rCol[rIndex]]->clear();
            }
            else
            {
                sudokuCells[rRow[rIndex]][rCol[rIndex]]->setText(QString::number(results[rIndex]));
            }
            --rIndex;
            qDebug() << "Previous write";
        }
    }
}

void MainWindow::on_nextButton_clicked()
{
    if(results.isEmpty() || rIndex == results.size() - 1)
    {
        QMessageBox::warning(this, "Next", "No next moves are present");
    }
    else
    {
        ++rIndex;
        qDebug() << rIndex;
        if(results[rIndex] == 0){
            sudokuCells[rRow[rIndex]][rCol[rIndex]]->clear();
            qDebug() << "Next clear";
        }
        else
        {
            sudokuCells[rRow[rIndex]][rCol[rIndex]]->setText(QString::number(results[rIndex]));
            qDebug() << "Next write";
        }
    }
}

void MainWindow::setSudokuSolved(bool solved)
{
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (sudokuCells[row][col]) {
                sudokuCells[row][col]->setReadOnly(solved);
            }
        }
    }
}
