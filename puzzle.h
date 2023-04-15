#include <bits/stdc++.h>
using namespace std;

class puzzle{

	vector<vector<int> > grid;
	vector<int> ar;
	vector<int> nums;
	const int GRID_ROWS=9;
	const int GRID_COLS=9;					// TODO: Use this to make 4x4
public:

	puzzle();
	void display();							// Debugging Purpose
	bool try_to_solve(int x, int y);		// tries to solve an empty sudoku board
	bool checkBox(int num , bool);
	bool checkCol(int num , bool);
	bool checkRow(int num , bool);
	pair<int, int>  getPos(int cell);		// cell no -> {x,y}
	bool validate(vector<int> & , bool);
	int randRange(int low, int high);
    void hidecells(vector<int> &, int lvl);
	int getIndex(int, int);
	int mirror(int cell);					// Used for hiding the mirror element along the diagonal
	void generator(vector<int> &, int lvl);
	bool checker(int cell, int val, bool full);
	void setValue(int cell ,int val);
};
