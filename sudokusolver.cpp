#include <QSpacerItem>
#include <QSizePolicy>
#include "sudokusolver.h"

SudokuSolver::SudokuSolver(QWidget *parent)
    : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            cells[row][col] = new QLineEdit(centralWidget);
            cells[row][col]->setAlignment(Qt::AlignCenter);
            cells[row][col]->setMaxLength(1);
            cells[row][col]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            gridLayout->addWidget(cells[row][col], row, col);
        }
    }

    solveButton = new QPushButton("Risolvi", centralWidget);
    resetButton = new QPushButton("Reset", centralWidget);
    stepButton = new QPushButton("Passo-Passo", centralWidget);

    solveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    resetButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    stepButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    gridLayout->addWidget(solveButton, 9, 0, 1, 3);
    gridLayout->addWidget(resetButton, 9, 3, 1, 3);
    gridLayout->addWidget(stepButton, 9, 6, 1, 3);

    // Aggiungi uno spacer item per controllare meglio il comportamento di ridimensionamento
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer, 10, 0, 1, 9);

    connect(solveButton, &QPushButton::clicked, this, &SudokuSolver::solveSudoku);
    connect(resetButton, &QPushButton::clicked, this, &SudokuSolver::resetGrid);
    connect(stepButton, &QPushButton::clicked, this, &SudokuSolver::stepByStepSolve);

    setCentralWidget(centralWidget);
    setWindowTitle("Sudoku Solver");
    resize(400, 450);
    step=false;
}

SudokuSolver::~SudokuSolver() {}

void SudokuSolver::solveSudoku() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (cells[row][col]->text().isEmpty()) {
                sudoku[row][col] = 0;
                sudoku2[row][col] = 0;
            } else {
                sudoku[row][col] = cells[row][col]->text().toInt();
                sudoku2[row][col] = cells[row][col]->text().toInt();
            }
        }
    }

    if (solve(0, 0)) {
        updateGrid();
        solveButton->setEnabled(false);
    } else {
        QMessageBox::information(this, "Errore", "Non esiste una soluzione per il Sudoku inserito.");
    }
}

void SudokuSolver::resetGrid() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            cells[row][col]->clear();
            cells[row][col]->setReadOnly(false);
        }
    }
    solveButton->setEnabled(true);
    steps.clear(); // Resetta i passi
    step=false;
}

void SudokuSolver::stepByStepSolve() {
    if(!(solveButton->isEnabled())){
        if(!step){
            for (int row = 0; row < 9; ++row) {
                for (int col = 0; col < 9; ++col) {
                    if(sudoku2[row][col]!=0){
                        cells[row][col]->setText(QString::number(sudoku2[row][col]));
                        cells[row][col]->setReadOnly(true);
                    }
                    else
                        cells[row][col]->setText("");
                }
            }
            step=true;
        }
        for (int row = 0; row < 9; ++row) {
                for (int col = 0; col < 9; ++col) {
                    if (cells[row][col]->text().isEmpty()) {
                        sudoku[row][col] = 0;
                    } else {
                        sudoku[row][col] = cells[row][col]->text().toInt();
                    }
                }
            }
        steps.clear();
            if (solve(0, 0)) {
                displayStep();
            } else {
                QMessageBox::information(this, "Errore", "Non esiste una soluzione per il Sudoku inserito.");
            }
    }
}

bool SudokuSolver::solve(int row, int col) {
    if (row == 9) {
        return true;
    }

    if (sudoku[row][col] != 0) {
        return solve(col == 8 ? row + 1 : row, (col + 1) % 9);
    }

    for (int num = 1; num <= 9; ++num) {
        if (isValid(row, col, num)) {
            sudoku[row][col] = num;
            steps.append(QPair<int, int>(row, col)); // Aggiungi il passo
            if (solve(col == 8 ? row + 1 : row, (col + 1) % 9)) {
                return true;
            }
            sudoku[row][col] = 0;
            steps.append(QPair<int, int>(row, col)); // Aggiungi il passo
        }
    }
    return false;
}

bool SudokuSolver::isValid(int row, int col, int num) {
    for (int i = 0; i < 9; ++i) {
        if (sudoku[row][i] == num || sudoku[i][col] == num ||
            sudoku[row / 3 * 3 + i / 3][col / 3 * 3 + i % 3] == num) {
            return false;
        }
    }
    return true;
}

void SudokuSolver::updateGrid() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            cells[row][col]->setText(QString::number(sudoku[row][col]));
            cells[row][col]->setReadOnly(true);
        }
    }
}

void SudokuSolver::displayStep() {
    if (!steps.isEmpty()) {
        auto step = steps.takeFirst();
        int row = step.first;
        int col = step.second;
        cells[row][col]->setText(QString::number(sudoku[row][col]));
    }
}
