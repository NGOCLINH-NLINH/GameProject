#include <bits/stdc++.h>
using namespace std;


class Generator {
private:
    int* mGrid;
    int* mGridSolution;

public:
    int** mat;
    int N = 9;
    int SRN;
    Generator();

    void copyGrid(int* grid);

    void setElement(int row, int col, int num);

    int getElement(int row, int col);

    int randRange(int low, int high);

    void fillDiagonal();

    bool unUsedInBox(int rowStart, int colStart, int num);

    void fillBox(int row, int col);

    int randomGenerator(int num);

    bool CheckIfSafe(int i, int j, int num);

    bool unUsedInRow(int i, int num);

    bool unUsedInCol(int j, int num);

    bool fillRemaining(int i, int j);

    void removeKDigits(int lvl);

    void printSudoku();

    void generate(int* grid, int* solutionGrid, int lvl);

};
