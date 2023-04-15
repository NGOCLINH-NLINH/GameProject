#include "puzzle.h"


puzzle::puzzle(){

	ar = vector<int> (9, 0);
	grid = vector<vector<int> > (9, vector<int>(9,0));
	nums = vector<int> (81, 0);
    for(int i=0; i < 81; i++){
		nums[i]=i;
	}

    for(int i=0; i<9; i++){
        ar[i]= i+1;
    }
}

bool puzzle::validate(vector<int> &visited, bool full=0){
	for(int i=1;i<=9;i++){
    	if(visited[i]>1)return 0;
    	if(full && visited[i]!=1)return 0;
    	else visited[i]=0;
    }
    return 1;
}

pair<int, int> puzzle::getPos(int cell){
	int x,y;
	x=cell/9;
	y=cell%9;
	return {x,y};
}

int puzzle::getIndex(int row, int col){
	return row * 9 + col;
}

void puzzle::display(){
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			cout << grid[i][j] << " ";
		}
		cout << endl;
	}
}
bool puzzle::checkRow(int num, bool full=0){
	int x=num;
	vector<int> visited(9+1);

	for(int i=0; i<9; i++){
	    visited[grid[x][i]]++;
	}

	return(validate(visited, full));
}

bool puzzle::checkCol(int num, bool full=0){
	int y=num;
	vector<int> visited(9+1);

	for(int i=0; i<9; i++){
	    visited[grid[i][y]]++;
	}

	return(validate(visited, full));
}

bool puzzle::checkBox(int num, bool full=0){
	int x = num - (num%3), y=3*(num%3);
	vector<int> visited(9+1);

	for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            visited[grid[x+i][y+j]]++;
        }
    }

	return(validate(visited, full));
}

// function to check if val can be a right guess for cell or not
// set the 'full' flag to check for complete grid
bool puzzle::checker(int cell, int val, bool full=0){

	if(!full){
		int x=0,y=0;
		x=getPos(cell).first;
		x=getPos(cell).second;

		return ( checkRow(x) && checkCol(y) && checkBox(x-(x%3)+(y/3)));
	}
	else {
		for(int i=0;i<9;i++){
			if( !checkRow(i,1) || !checkCol(i,1) || !checkBox(i,1))
                return 0;
		}
		return 1;
	}

}

void puzzle::setValue(int cell ,int val){
	int x=0,y=0;
	x=getPos(cell).first;
	y=getPos(cell).second;
	grid[x][y]=val;
}

bool puzzle::try_to_solve(int x, int y){
    if(x>=9)return 1;
    if(y>=9 || grid[x][y]!=0){
        return try_to_solve( x+1, 0);
    }
    vector<int> visited(9+1);
    for(int i=0; i<9; i++) visited[grid[i][y]]=1;
    for(int i=0; i<9; i++) visited[grid[x][i]]=1;
    int x_box = x - (x%3), y_box = y - (y%3);

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            visited[grid[x_box + i][y_box + j]]=1;
        }
    }
    for(auto it:ar){
        if(visited[it]==0){
            grid[x][y]=it;
            if(try_to_solve(x, y+1)){
                return 1;
            }
        }
    }
    grid[x][y]=0;
    return 0;
}

int puzzle::mirror(int cell){
	int x=0,y=0;
	x=getPos(cell).first;
	y=getPos(cell).second;
	return getIndex(y,x);
}

int puzzle::randRange(int low, int high)
{
    return (int)(rand() / (RAND_MAX / (double)(high - low))) + low;
}

void puzzle::hidecells(vector<int> & sudokuGrid, int lvl){
	int cnt=0, given = 0;
	switch(lvl) {
    case 1://easy
        {
            given = randRange(40, 43);
        }

    case 2://medium
        {
            given = randRange(34, 37);
        }

    case 3://hard
        {
            given = randRange(28, 31);
        }
    }
	for(int i=0; i < 81 && cnt < 81 - given; i++){
		if(sudokuGrid[nums[i]]!=0){
			sudokuGrid[nums[i]]=0;
			sudokuGrid[mirror(nums[i])]=0;
			pair<int, int> coor = getPos(nums[i]);
			grid[coor.first][coor.second]=0;
			grid[coor.second][coor.first]=0;
			cnt+=2;
		}
	}
}

void puzzle::generator(vector<int> & sudokuGrid, int lvl){

   	// to creat a permutation
	mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

   	shuffle(ar.begin(), ar.end() , rng);
   	shuffle(nums.begin(), nums.end() , rng);

   	for(int row=0; row<9; row++){
		for(int col=0; col<9; col++){
			grid[row][col]=0;
		}
	}

    bool done=0;
    while(!done){
	    if(try_to_solve(0, 0))
	    	done = 1;
	    else{
	    	cout<<"Couldn't create a puzzle, try again"<<endl;
	    }
	}
	int cell=0;
	for(int row=0; row<9; row++){
		for(int col=0; col<9; col++){
			sudokuGrid[cell++]=grid[row][col];
		}
	}
    display();

	hidecells(sudokuGrid, lvl);
	display();
}
