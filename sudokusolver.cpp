#include <bits/stdc++.h>
using namespace std;

bool checkBlankCell(int grid[9][9], int& row, int& col);
bool checkPossibility(int grid[9][9], int row, int col, int digit);
bool isSolved(int grid[9][9]);
void printGrid(int grid[9][9]);

int main() {
    int grid[9][9] =  {{ 0, 0, 9, 0, 4, 0, 8, 0, 0 },
                       { 0, 6, 0, 5, 0, 8, 0, 0, 2 },
                       { 5, 0, 0, 0, 3, 0, 0, 0, 0 },
                       { 0, 4, 0, 0, 0, 0, 0, 7, 0 },
                       { 7, 0, 0, 2, 0, 5, 9, 0, 0 },
                       { 0, 0, 0, 3, 0, 0, 0, 0, 0 },
                       { 0, 0, 1, 0, 0, 0, 0, 0, 6 },
                       { 4, 0, 0, 7, 0, 9, 2, 0, 0 },
                       { 0, 0, 0, 0, 8, 0, 0, 0, 0 }};
    //for (int i = 0; i < 9; i++) {
    //    for (int j = 0; j < 9; j++)
    //        cin >> grid[i][j];
    //}
    cout << endl;
    if (isSolved(grid) == true) printGrid(grid);
    else cout << "404 not found";
    return 0;
}

bool checkPossibility(int grid[9][9], int row, int col, int digit){
    bool row_existed = false, col_existed = false, subgrid_existed = false;
    for (int col = 0; col < 9; col++) {
        if (grid[row][col] == digit) {
            row_existed = true;
            break;
        }
    }
    for (int row = 0; row < 9; row++) {
        if (grid[row][col] == digit) {
            col_existed = true;
            break;
        }
    }
    int xrow = row - row%3, xcol = col - col%3;
    for (int row = xrow; row < xrow + 3; row++) {
        for (int col = xcol; col < xcol + 3; col++)
            if (grid[row][col] == digit) {
                subgrid_existed = true;
                break;
            }
    }
    return (!row_existed && !col_existed && !subgrid_existed && grid[row][col] == 0);
}

bool checkBlankCell(int grid[9][9], int& row, int& col){
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++)
            if (grid[row][col] == 0) return true;
    }
    return false;
}

bool isSolved(int grid[9][9]) {
    int row, col;
    if (!checkBlankCell(grid, row, col))
        return true;
    for (int digit = 1; digit <= 9; digit++) {
        if (checkPossibility(grid, row, col, digit)) {
            grid[row][col] = digit;
            if (isSolved(grid))
                return true;
            grid[row][col] = 0;
        }
    }
    return false;
}

void printGrid(int grid[9][9])
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
            cout << grid[row][col] << " ";
        cout << endl;
    }
}



