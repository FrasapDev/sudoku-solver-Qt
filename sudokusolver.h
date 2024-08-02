#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

class SudokuSolver : public QMainWindow {
    Q_OBJECT

public:
    SudokuSolver(QWidget *parent = nullptr);
    ~SudokuSolver();

private slots:
    void solveSudoku();
    void resetGrid();
    void stepByStepSolve();

private:
    bool solve(int row, int col);
    bool isValid(int row, int col, int num);
    bool findEmptyCell(int &row, int &col);
    void updateGrid();
    void displayStep();

    QGridLayout *gridLayout;
    QLineEdit *cells[9][9];
    QPushButton *solveButton;
    QPushButton *resetButton;
    QPushButton *stepButton;
    int sudoku[9][9];
    int sudoku2[9][9];
    bool step;
    QList<QPair<int, int>> steps; // Lista dei passi per la risoluzione passo-passo
};

#endif // SUDOKUSOLVER_H
