#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int MAX_ROWS = 20;
const int MAX_COLS = 20;
const char WALL = '#';
const char EMPTY = ' ';
const char PLAYER = '^';
const char SNAKE = '~';
const char GOAL = 'X';

struct Location {
    int row;
    int col;
};

class MazeGame {
public:
    MazeGame(const string& mapFileName);
    void play();

private:
    char gameMap[MAX_ROWS][MAX_COLS];
    int numRows;
    int numCols;
    int numObj;

    Location player;
    Location snake;
    Location goal;
    vector<Location> moveHistory;

    void loadMap(const string& mapFileName);
    void printMap();
    bool isValidMove(int newRow, int newCol);
    bool isGameOver();
    bool movePlayer(char move);
    bool moveSnake();
    void undo();
    bool isWin();
};

MazeGame::MazeGame(const string& mapFileName) {
    loadMap(mapFileName);
    moveHistory.clear();
}

void MazeGame::loadMap(const string& mapFileName) {
    ifstream mapFile(mapFileName);
    if (!mapFile) {
        cerr << "Error: File not found." << endl;
        exit(1);
    }
// modified file input;
    mapFile >>numObj >> numRows >> numCols;
    for(int i=0;i<MAX_ROWS;i++){
        for(int j=0;j<MAX_COLS;j++){
            gameMap[i][j]=EMPTY;
        }
    }

   
    for(int i=0;i<numObj;i++ ){
        int r,c,code;
        mapFile>>r>>c>>code;
        if(code==0){//player location
        player.row=r;
        player.col=c;
        gameMap[r][c]=PLAYER;

        }
        else if (code==1){ //snake
        snake.row=r;
        snake.col=c;
        gameMap[r][c]=SNAKE;

        }
        else if(code==2){//goal
        goal.row=r;
        goal.col=c;
        gameMap[r][c]=GOAL;

        }
        else if(code==3){
            gameMap[r][c]=WALL;
            //wall
        }
    }
    for(int i=0;i<numRows;i++){ gameMap[i][0]=WALL;gameMap[i][numCols-1]=WALL;}
    for(int i=0;i<numCols;i++){ gameMap[0][i]=WALL;gameMap[numRows-1][i]=WALL;}
    

    mapFile.close();
}

void MazeGame::printMap() {
    system("cls");  // Clear the terminal (Unix-based systems)
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            cout << gameMap[row][col] << ' ';
        }
        cout << endl;
    }
}

bool MazeGame::isValidMove(int newRow, int newCol) {
    return (newRow >= 0 && newRow < numRows && newCol >= 0 && newCol < numCols &&
            gameMap[newRow][newCol] != WALL);
}

bool MazeGame::movePlayer(char move) {
    int newRow = player.row;
    int newCol = player.col;

    if (move == 'w') newRow--;
    else if (move == 's') newRow++;
    else if (move == 'a') newCol--;
    else if (move == 'd') newCol++;

    if (isValidMove(newRow, newCol)) {
        moveHistory.push_back(player);
        gameMap[player.row][player.col] = EMPTY;
        player.row = newRow;
        player.col = newCol;
        gameMap[player.row][player.col] = PLAYER;
        return true;
    }

    return false;
}


bool MazeGame::moveSnake() {
  // Calculate the new snake position

  int newRow = snake.row;
  int newCol = snake.col;

  // If the snake is moving towards the player, update its position
  if (player.row < newRow) newRow -= 1;
  else if (player.row > newRow) newRow += 1;
  else if (player.col < newCol) newCol -= 1;
  else if (player.col > newCol) newCol += 1;

  // Update the snake's position
  if(gameMap[newRow][newCol]!=WALL)
 { gameMap[snake.row][snake.col] = EMPTY;
  snake.row = newRow;
  snake.col = newCol;
  gameMap[snake.row][snake.col] = SNAKE;}

  // Check if the snake has bitten the player
  if (player.row == snake.row && player.col == snake.col) {
    return true;
  } else {
    return false;
  }
}



void MazeGame::undo() {
    if (!moveHistory.empty()) {
        Location previous = moveHistory.back();
        moveHistory.pop_back();
        gameMap[player.row][player.col] = EMPTY;
        player = previous;
        gameMap[player.row][player.col] = PLAYER;
    }
}

bool MazeGame::isWin() {
    return (player.row == goal.row && player.col == goal.col);
}

bool MazeGame::isGameOver() {

    return (player.row == snake.row && player.col == snake.col);
}


void MazeGame::play() {
    while (true) {
        printMap();

        
        if (isWin()) {
            cout << "Congratulations! You won!" << endl;
            break;
        }

        char move;
        cout << "Enter move (w to up/s to down/a to left/d to right/u to undo): ";
        cin >> move;

        if (move == 'u') {
            undo();
        } else if (move == 'w' || move == 'a' || move == 's' || move == 'd') {
            movePlayer(move);
            // moveSnake(); // Implement snake movement
        }
        
      if (moveSnake()) {
            cout << "Game Over! Snake bit you." << endl;
            break;
        }
    }
}


int main() {
    const string mapFileName = "maze.txt"; // Change this to your map file
    MazeGame game(mapFileName);
    game.play();
    return 0;
}