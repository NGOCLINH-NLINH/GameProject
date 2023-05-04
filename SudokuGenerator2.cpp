#include "SudokuGenerator2.h"

Generator::Generator()
     :mGrid(nullptr) , mGridSolution(nullptr)
    {
        double SRNd = sqrt(N);
        SRN = (int)SRNd;
        mGrid = new int[81];

    }

    void Generator::copyGrid(int* grid)
    {
        for (int i = 0; i < 81; i++)
        {
            grid[i] = mGrid[i];
        }
    }

    void Generator::setElement(int row, int col, int num)
    {
        mGrid[row * 9 + col] = num;
    }

    int Generator::getElement(int row, int col)
    {
        return mGrid[row * 9 + col];
    }

    int Generator::randRange(int low, int high)
    {

        return (int)(rand() / (RAND_MAX / (double)(high - low))) + low;
    }

    void Generator::fillDiagonal()
    {
        for (int i = 0; i < N; i = i + SRN)
        {
            fillBox(i, i);
        }
    }

    bool Generator::unUsedInBox(int rowStart, int colStart, int num)
    {
        for (int i = 0; i < SRN; i++) {
            for (int j = 0; j < SRN; j++) {
                if (getElement(rowStart+i, colStart+j) == num) {
                    return false;
                }
            }
        }
        return true;
    }

    void Generator::fillBox(int row, int col)
    {
        int num;
        for (int i = 0; i < SRN; i++) {
            for (int j = 0; j < SRN; j++) {
                do {
                    num = randomGenerator(N);
                } while (!unUsedInBox(row, col, num));

                  setElement(row+i, col+j, num);
            }
        }
    }

    int Generator::randomGenerator(int num)
    {
        return (int)floor((float)(rand() / double(RAND_MAX) * num + 1));
    }

    bool Generator::CheckIfSafe(int i, int j, int num)
    {
        return (unUsedInRow(i, num) && unUsedInCol(j, num) && unUsedInBox(i - i % SRN, j - j % SRN, num));
    }

    bool Generator::unUsedInRow(int i, int num)
    {
        for (int j = 0; j < N; j++) {
              if (getElement(i, j) == num) return false;
        }
        return true;
    }

    bool Generator::unUsedInCol(int j, int num)
    {
        for (int i = 0; i < N; i++) {
              if (getElement(i, j) == num) return false;
        }
        return true;
    }

    bool Generator::fillRemaining(int i, int j)
    {
        if (j >= N && i < N - 1) {
            i = i + 1;
            j = 0;
        }
        if (i >= N && j >= N) {
            return true;
        }
        if (i < SRN) {
            if (j < SRN) {
                j = SRN;
            }
        }
        else if (i < N - SRN) {
            if (j == (int)(i / SRN) * SRN) {
                j = j + SRN;
            }
        }
        else {
            if (j == N - SRN) {
                i = i + 1;
                j = 0;
                if (i >= N) {
                    return true;
                }
            }
        }
        for (int num = 1; num <= N; num++) {
            if (CheckIfSafe(i, j, num)) {
                setElement(i, j, num);
                if (fillRemaining(i, j + 1)) {
                    return true;
                }
                setElement(i, j, 0);
            }
        }
        return false;
    }

    void Generator::removeDigits(int lvl)
    {
        int given = 0;
        // given numbers based on level
        if (lvl == 1){
            given = randRange(37, 41);
        }

        if (lvl == 2){
            given = randRange(32, 36);
        }

        if (lvl == 3){
            given = randRange(27, 31);
        }
        int cnt = 81 - given;
        while (cnt != 0) {

            int cellId = rand() % 81;
            int i = (cellId / N);
            int j = cellId % 9;

            if (getElement(i,j) != 0) {
                cnt--;
                setElement(i,j, 0);
            }
        }
    }

    void Generator::printSudoku()
    {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << getElement(i,j) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

void Generator::generate(int* grid, int* solutionGrid, int lvl){

        mGrid = grid;
        mGridSolution = solutionGrid;

        srand((unsigned int)time(NULL));
        //mGrid = solutionGrid;

        fillDiagonal();
        fillRemaining(0, SRN);
        copyGrid(mGridSolution);
        printSudoku();
        cout << endl;

        removeDigits(lvl);
        printSudoku();

}

//int main()
//{
//    Generator G;
//    int lvl = 2;
//    int generatedGrid[81] = {};
//    int solutionGrid[81] = {};
//    G.generate(generatedGrid, solutionGrid, lvl);
//    return 0;
//}


