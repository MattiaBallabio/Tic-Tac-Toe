#include <iostream>
#include <cstdlib>
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

  cout << "\033[2J\033[H"; // clear the screen
  
  cout << "   |   |\n"
          " ";

  cout << "O0X"[board[0]+1];
  
  cout << " | ";
          
  cout << "O1X"[board[1]+1];
  
  cout << " | ";
  
  cout << "O2X"[board[2]+1];
    
  cout << "\n"
          "   |   |\n"
          "---+---+---\n"
          "   |   |\n"
          " ";
          
  cout << "O3X"[board[3]+1];
  
  cout << " | ";
          
  cout << "O4X"[board[4]+1];
  
  cout << " | ";
  
  cout << "O5X"[board[5]+1];
  
  cout << "\n"
          "   |   |\n"
          "---+---+---\n"
          "   |   |\n"
          " ";
          
  cout << "O6X"[board[6]+1];
  
  cout << " | ";
          
  cout << "O7X"[board[7]+1];
  
  cout << " | ";
  
  cout << "O8X"[board[8]+1];
  
  cout << "\n"
          "   |   |\n";
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
// void takeRandomSquare(int squares[])
//   determines whether a corner or other square needs to be picked
//
// Parameters
//   squares[]      - uses either corners[] or other[]
//   computerLetter - 1 if player is X, -1 if player is O
//

void takeRandomSquare(const int squares[], int computerLetter) {

  int choice;
  do {
      //uses choice random variable
      uniform_int_distribution<> dis(0,3);
      choice = dis(mt);

      //checks if random tile is available
      if (board[squares[choice]] == 0) {
        board[squares[choice]] = computerLetter;
        break;
      }
    } while (true);
}

//=============================================================================
// void getComputerMove(int computerLetter)
//   determine next move for the computer and place move on the board
//
// Parameters
//   computerLetter - 1 if player is X, -1 if player is O
//

void getComputerMove(int computerLetter) {
  int
    winningSquare,
    choice;

  // if computer can win, make the winning move
  if (canWin(computerLetter,winningSquare)) {
    board[winningSquare] = computerLetter;
  }
  // else if human can win on the next move, make the blocking move
  else if (canWin(-computerLetter,winningSquare)) {
    board[winningSquare] = computerLetter;
  }
  // else if center is open, take it
  else if (board[4] == 0) {
    board[4] = computerLetter;
  }
  
  // else if a corner is open, take it
  else if (board[0] == 0 || board[2] == 0 || board[6] == 0 || board[8] == 0) {
    takeRandomSquare(corners, computerLetter);
  }

  // else take any open square
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

      //TODO: Make sure user can enter q or Q to quit the program

      //Making sure it's an existing tile
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