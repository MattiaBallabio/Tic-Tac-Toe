#include <iostream>
#include <random>

using namespace std;

random_device rd;
mt19937 mt(rd());

const char
  LETTERS[3] = {'O',' ','X'};

int
  board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

const int
  corners[4] = {0, 2, 6, 8},
  other[4] = {1, 3, 5, 7};

//Used for optimization when checking for winning tiles
const int winningPatterns[8][3] {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},
    {0, 4, 8},
    {2, 4, 6}
  };
 
//=============================================================================
// void displayBoard()
//   display the board on the screen
//

void displayBoard() {
  
  //Clear the screen
  cout << "\033[2J\033[H"; 
  
  cout << "   |   |\n";
  for (int i = 0; i < 9; ++i) {
    //Print "O" for -1, "X" for 1, or the cell number for 0
    if (board[i] == 0) {
      cout << " " << i;
    } else {
      cout << " " << "O X"[board[i] + 1];
    }

    //End of a row
    if (i % 3 == 2) { 
      cout << "\n   |   |\n";
      if (i < 6) cout << "---+---+---\n   |   |\n";
    } else {
      cout << " |";
    }
  }

}

//=============================================================================
// bool playerWins(int playerLetter)
//   determine if player has won the game
//
// Parameters
//   playerLetter - 1 for X, -1 for O
//
// Returns
//   true if player has won, false if not
//

bool playerWins(int playerLetter) {

  for (int i = 0; i < 8; i++) {
    if (board[winningPatterns[i][0]] +
        board[winningPatterns[i][1]] +
        board[winningPatterns[i][2]] == playerLetter * 3)
      return true;
  }
  return false;
}
  
//=============================================================================
// bool canWin(int playerLetter,int &winningSquare)
//   determine if player can win on this move
//
// Parameters
//   playerLetter  - 1 for X, -1 for O
//   winningSquare - if player can win, this is filled in with the square to
//                   take in order to win
//
// Returns
//   true if player can win on this move, false if player cannot win
//

bool canWin(int playerLetter,int &winningSquare) {

  for (int i = 0; i < 8; i++) {
    if (board[winningPatterns[i][0]] +
        board[winningPatterns[i][1]] +
        board[winningPatterns[i][2]] == playerLetter * 2) {

      for (int j = 0; j < 3; j++) {
        if (board[winningPatterns[i][j]] == 0) {
          winningSquare = winningPatterns[i][j];
          return true;
        }
      }
    }
  }

  return false; // placeholder to make compiler less unhappy
}

//=============================================================================
// void takeRandomSquare(int squares[], int computerLetter)
//   determines whether a corner or other square needs to be picked
//
// Parameters
//   squares[]      - uses either corners[] or other[]
//   computerLetter - 1 if player is X, -1 if player is O
//

void takeRandomSquare(const int squares[], int computerLetter) {

  int choice;
  do {
      //Uses choice random variable
      uniform_int_distribution<> dis(0,3);
      choice = dis(mt);

      //Checks if random tile is available
      if (board[squares[choice]] == 0) {
        board[squares[choice]] = computerLetter;
        break;
      }
    } while (true);
}

//=============================================================================
// void getComputerMove(int computerLetter)
//   determine next move for the computer and place move on the board
//   win/block -> center -> corner -> other
//
// Parameters
//   computerLetter - 1 if player is X, -1 if player is O
//

void getComputerMove(int computerLetter) {
  int
    winningSquare,
    choice;

  //Win the game or block winning move
  if (canWin(computerLetter,winningSquare) || canWin(-computerLetter,winningSquare)) {
    board[winningSquare] = computerLetter;
  }
  //Take corner
  else if (board[4] == 0) {
    board[4] = computerLetter;
  }
  
  //Take corners
  else if (board[0] == 0 || board[2] == 0 || board[6] == 0 || board[8] == 0) {
    takeRandomSquare(corners, computerLetter);
  }

  //Take remaining squares
  else {
    takeRandomSquare(other, computerLetter);
  }
}

//=============================================================================
// void getHumanMove(int humanLetter)
//   get one move from the keyboard, allow for exiting the program
//   also, place move on the board
//
// Parameters
//   humanLetter - 1 if player is X, -1 if player is O
//

void getHumanMove(int humanLetter) {

  //Display board
  displayBoard();
  
  //Do ... while choice not valid
  do {
    cout << "Enter the number of the cell you want to pick: ";
      
    //Make the user select a valid tile
    int a;
    do {
      cin >> a;

      //Making sure it's an existing tiles
      if (a >= 0 && a <= 8) break;
      else cout << "Please try again with a value from 0 to 8" << endl;
    } while (true);

    //Making sure the tile isn't taken
    if (board[a] == 0) {
      board[a] = humanLetter;
      break;
    }
    else {
      cout << "The cell you entered is already taken." << endl;
    }

  } while (true);

}

//=============================================================================
// int main()
//   the main code for tic-tac-toe
//

int main() {
  //Variables
  bool
    isHumanTurn;
  int
    humanPlayer,
    computerPlayer,
    choice;
    
  displayBoard();
  
  //Randomly choose X player
  uniform_int_distribution<> dis(0,1);
  humanPlayer = 1 - 2 * dis(mt);
  computerPlayer = -humanPlayer;

  if (humanPlayer == 1) isHumanTurn = true;
  
  //Loop 9 times at most (9 tiles)
  for (int i = 0; i < 9; i++) {

      //Deciding whose move it is
      if (isHumanTurn) {
        getHumanMove(humanPlayer);
      }
      else {
        getComputerMove(computerPlayer);
      }
      
      //See if player won the game
      if (playerWins(humanPlayer)) {
        displayBoard();
        cout << "Player wins." << endl;
        break;
      }
      
      //See if computer won the game
      if (playerWins(computerPlayer)) {
        displayBoard();
        cout << "Computer wins." << endl;
        break;
      }

      //Change over to other player
      isHumanTurn = !isHumanTurn;
  }

  if (!playerWins(humanPlayer) && !playerWins(computerPlayer)) {
    displayBoard();
    cout << "Draw." << endl;
  }

  return 0;
}