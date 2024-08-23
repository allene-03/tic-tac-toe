#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Goal: Add two-player mode

int main() {

  // Declare basic variables/functions
  char board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
  char displayer[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
  char displayer_2[3][3] = { {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'} };
  int closer[3][3], turns = 0, meter; // Closer neccesary to not cause display error, doesn't do anything else
  char answer[3], icon, icon_p[2], other_icon, winner;
  srand((unsigned)time(NULL));
  void displayBoard(char displayObject[3][3]);
  int checkWin(char displayObject[3][3]);
  int doRandom(void);
  void computerMove(char displayObject[3][3], char icon, char plr_icon);
  int doMovements(char displayObject[3][3], int square, char icon);

  // Ask to view instructions
  printf("Do you want to view instructions for the mechanics of this game? Yes or no? ");
  scanf("%s", answer);

  for(int i = 0; answer[i]; i++){
    answer[i] = tolower(answer[i]);
  }

  // Do instructions
  if(strcmp(answer, "yes") == 0) {
    getchar();
    puts("\nInstructions: \n");
    printf("This is the standard board when starting.\n");
    displayBoard(displayer);
    printf("\n\n");
    printf("(Press enter to continue.)");
    getchar();

    printf("\nTo do a move, when asked for your turn enter the number of where you want your 'X or O' to go.\n");
    displayBoard(displayer_2);
    printf("\n(Press enter to continue.)");
    getchar();

    printf("\nFor example, the console will ask the user where they want to go. \n");
    printf("Which square do you want to go to?");
    puts("... 8\n");
    displayer[2][1] = 'X';
    displayBoard(displayer);
    printf("\nTherefore, you place a piece in square 8.\n");

    printf("\n(Press enter to start game.)");
    getchar();
  }
  else if(strcmp(answer, "no") == 0) {
    puts("Starting game...");
  }
  else {
    puts("Your answer was not understood. Defaulting to 'no'. ");
  }

  // Choose first player
  if(doRandom() == 1) {
    printf("\nBased on random selection, you will go first.");
    fflush(stdout);
    meter = 1;
  }
  else {
    printf("\n%s", "Based on random selection, computer will go first.");
    fflush(stdout);
    meter = 0;
  }
  sleep(1);

  // Choose icon
  printf("\nWhat do you want your icon to be? X or O? ");
  scanf("%s", icon_p);
  for(int i = 0; icon_p[i]; i++){
    icon_p[i] = tolower(icon_p[i]);
  }
  printf("\n");

  if(strcmp(icon_p, "x") == 0) {
    printf("Your icon is X.\n");
    icon = 'X';
    other_icon = 'O';
  }
  else if(strcmp(icon_p, "o") == 0) {
    printf("Your icon is O.\n");
    icon = 'O';
    other_icon = 'X';
  }
  else {
    printf("Invalid answer. Choosing icon for you...");
    fflush(stdout);
    if(doRandom() == 1) {
      printf("\nBased on random selection, you will be X.\n");
      fflush(stdout);
      icon = 'X';
      other_icon = 'O';
    }
    else {
      printf("\nBased on random selection, you will be O.\n");
      fflush(stdout);
      icon = 'O';
      other_icon = 'X';
    }
  }

  // Initialize board for simpler interface
  puts("\n");
  displayBoard(board);
  puts("\n");

  if(meter==0) {
    puts("The computer goes first!");
    sleep(1);
  }

  // Start game loop
  while((turns < 9)) {
    if(meter == 1) {  // Meter determines turns
      while(1==1) {
        int square;
        printf("%s\n", "Which square do you want to go to?");
        scanf("%i", &square);
        if(doMovements(board, square, icon)==0) {
          puts("\n");
          displayBoard(board);
          printf("%s%c%s%i.\n", "Player 1 has placed ", icon, " in square ", square);
          puts("\n");
          break;
        }
        else if(doMovements(board, square, icon)==2) {
          puts("That area is invalid. Please choose again.\n");
        }
        else if(doMovements(board, square, icon)==1) {
          puts("That area is already occupied. Please choose again.\n");
        }
        else {
          puts("Some error has occurred, please choose again.\n");
        }
      }

      if(checkWin(board)==1) {
        winner = 'X';
        break;
      }
      if(checkWin(board)==2) {
        winner = 'O';
        break;
      }

      turns+=1;
      meter-=1;
    }
    else {
      sleep(1); // Pause for computer response
      computerMove(board, other_icon, icon);
      if(checkWin(board)==1) {
        winner = 'X';
        break;
      }
      if(checkWin(board)==2) {
        winner = 'O';
        break;
      }
      turns+=1;
      meter+=1;
    }

    if(turns==8) { // End game
      winner = 'Z';
    }
  }

  // Choose winner - bit repetitive
  if(winner=='X') {
    if(icon == 'X') {
      puts("\nPlayer 1 is the winner."); // Only way for that to occur is by having two winning oppurtunities
    }
    else {
      puts("\nComputer is the winner.");
    }
  }
  else if(winner == 'O') {
    if(icon == 'O') {
      puts("\nPlayer 1 is the winner."); // Only way for that to occur is by having two winning oppurtunities
    }
    else {
      puts("\nComputer is the winner.");
    }
  }
  else if(winner == 'Z') {
    puts("\nPlayer 1 and the computer have tied!");
  }
  else {
    puts("Some error has occurred.");
  }
}

// Crucial function that displays board
void displayBoard(char displayObject[3][3]) {
  printf("   -------------\n");
  for(int x = 0; x < 3; x++) {
    printf("   ");
    for(int y = 0; y < 3; y++) {
      printf("| %c ", displayObject[x][y]);
    }
    printf("|\n   -------------\n");
  }
}

// Constantly checks who is winning if conditions for three in a row are met - hard coded
int checkWin(char displayObject[3][3]) {
  if(((displayObject[0][0] == 'X')&&(displayObject[1][0] == 'X')&&(displayObject[2][0] == 'X'))||((displayObject[0][1] == 'X')&&(displayObject[1][1] == 'X')&&(displayObject[2][1] == 'X'))
  ||((displayObject[0][2] == 'X')&&(displayObject[1][2] == 'X')&&(displayObject[2][2] == 'X'))||((displayObject[0][0] == 'X')&&(displayObject[0][1] == 'X')&&(displayObject[0][2] == 'X'))
  ||((displayObject[1][0] == 'X')&&(displayObject[1][1] == 'X')&&(displayObject[1][2] == 'X'))||((displayObject[2][0] == 'X')&&(displayObject[2][1] == 'X')&&(displayObject[2][2] == 'X'))
  ||((displayObject[0][0] == 'X')&&(displayObject[1][1] == 'X')&&(displayObject[2][2] == 'X'))||((displayObject[0][2] == 'X')&&(displayObject[1][1] == 'X')&&(displayObject[2][0] == 'X'))) {
    return 1;
  }
  else if(((displayObject[0][0] == 'O')&&(displayObject[1][0] == 'O')&&(displayObject[2][0] == 'O'))||((displayObject[0][1] == 'O')&&(displayObject[1][1] == 'O')&&(displayObject[2][1] =='O'
  ))||((displayObject[0][2] == 'O')&&(displayObject[1][2] == 'O')&&(displayObject[2][2] == 'O'))||((displayObject[0][0] == 'O')&&(displayObject[0][1] == 'O')&&(displayObject[0][2]
  =='O'))||((displayObject[1][0] == 'O')&&(displayObject[1][1] == 'O')&&(displayObject[1][2] == 'O'))||((displayObject[2][0] == 'O')&&(displayObject[2][1] == 'O')&&(displayObject[2][2]
  =='O'))||((displayObject[0][0] == 'O')&&(displayObject[1][1] == 'O')&&(displayObject[2][2] == 'O'))||((displayObject[0][2] == 'O')&&(displayObject[1][1] == 'O')&&(displayObject[2][0]
  == 'O'))) {
    return 2;
  }
  else {
    return 0;
  }
}

// Choose random int between 1/2
int doRandom(void) {
  return(rand() % 2 + 1);
}

// Hard-coded computer moves that assist it's success, and make it harder to beat - all hardcoded
void computerMove(char displayObject[3][3], char icon, char plr_icon) {
  int doMovements(char displayObject[3][3], int square, char icon);
  void displayBoard(char displayObject[3][3]);
  int square;

  puts("\n");

  // First segment - decide where to win
  if((displayObject[0][0] == icon)&&(displayObject[1][0] == icon)&&(displayObject[2][0]==' ')) {
    doMovements(displayObject, 7, icon);
    square = 7;
  }
  else if((displayObject[0][1] == icon)&&(displayObject[1][1] == icon)&&(displayObject[2][1]==' ')) {
    doMovements(displayObject, 8, icon);
    square = 8;
  }
  else if((displayObject[0][2] == icon)&&(displayObject[1][2] == icon)&&(displayObject[2][2]==' ')) {
    doMovements(displayObject, 9, icon);
    square = 9;
  }
  else if((displayObject[2][0] == icon)&&(displayObject[1][0] == icon)&&(displayObject[0][0]==' ')) {
    doMovements(displayObject, 1, icon);
    square = 1;
  }
  else if((displayObject[2][1] == icon)&&(displayObject[1][1] == icon)&&(displayObject[0][1]==' ')) {
    doMovements(displayObject, 2, icon);
    square = 2;
  }
  else if((displayObject[2][2] == icon)&&(displayObject[1][2] == icon)&&(displayObject[0][2]==' ')) {
    doMovements(displayObject, 3, icon);
    square = 3;
  }
  else if((displayObject[0][0] == icon)&&(displayObject[2][0] == icon)&&(displayObject[1][0]==' ')) {
    doMovements(displayObject, 4, icon);
    square = 4;
  }
  else if((displayObject[0][1] == icon)&&(displayObject[2][1] == icon)&&(displayObject[1][1]==' ')) {
    doMovements(displayObject, 5, icon);
    square = 5;
  }
  else if((displayObject[0][2] == icon)&&(displayObject[2][2] == icon)&&(displayObject[1][2]==' ')) {
    doMovements(displayObject, 6, icon);
    square = 6;
  }

  else if((displayObject[0][0] == icon)&&(displayObject[0][1] == icon)&&(displayObject[0][2]==' ')) {
    doMovements(displayObject, 3, icon);
    square = 3;
  }
  else if((displayObject[1][0] == icon)&&(displayObject[1][1] == icon)&&(displayObject[1][2]==' ')) {
    doMovements(displayObject, 6, icon);
    square = 6;
  }
  else if((displayObject[2][0] == icon)&&(displayObject[2][1] == icon)&&(displayObject[2][2]==' ')) {
    doMovements(displayObject, 9, icon);
    square = 9;
  }
  else if((displayObject[0][2] == icon)&&(displayObject[0][1] == icon)&&(displayObject[0][0]==' ')) {
    doMovements(displayObject, 1, icon);
    square = 1;
  }
  else if((displayObject[1][2] == icon)&&(displayObject[1][1] == icon)&&(displayObject[1][0]==' ')) {
    doMovements(displayObject, 4, icon);
    square  = 4;
  }
  else if((displayObject[2][2] == icon)&&(displayObject[2][1] == icon)&&(displayObject[2][0]==' ')) {
    doMovements(displayObject, 7, icon);
    square = 7;
  }
  else if((displayObject[0][0] == icon)&&(displayObject[0][2] == icon)&&(displayObject[0][1]==' ')) {
    doMovements(displayObject, 2, icon);
    square = 2;
  }
  else if((displayObject[1][0] == icon)&&(displayObject[1][2] == icon)&&(displayObject[1][1]==' ')) {
    doMovements(displayObject, 5, icon);
    square = 5;
  }
  else if((displayObject[2][0] == icon)&&(displayObject[2][2] == icon)&&(displayObject[2][1]==' ')) {
    doMovements(displayObject, 8, icon);
    square = 8;
  }

  else if((displayObject[0][0] == icon)&&(displayObject[1][1] == icon)&&(displayObject[2][2]==' ')) {
    doMovements(displayObject, 9, icon);
    square = 9;
  }
  else if((displayObject[0][2] == icon)&&(displayObject[1][1] == icon)&&(displayObject[2][0]==' ')) {
    doMovements(displayObject, 7, icon);
    square = 7;
  }
  else if((displayObject[2][0] == icon)&&(displayObject[1][1] == icon)&&(displayObject[0][2]==' ')) {
    doMovements(displayObject, 3, icon);
    square = 3;
  }
  else if((displayObject[2][2] == icon)&&(displayObject[1][1] == icon)&&(displayObject[0][0]==' ')) {
    doMovements(displayObject, 1, icon);
    square = 1;
  }
  else if((displayObject[0][0] == icon)&&(displayObject[2][2] == icon)&&(displayObject[1][1]==' ')) {
    doMovements(displayObject, 5, icon);
    square = 5;
  }
  else if((displayObject[0][2] == icon)&&(displayObject[2][0] == icon)&&(displayObject[1][1]==' ')) {
    doMovements(displayObject, 5, icon);
    square = 5;
  }

  // --- Second segment - decide where to defend
  else if((displayObject[0][0] == plr_icon)&&(displayObject[1][0] == plr_icon)&&(displayObject[2][0]==' ')) {
    doMovements(displayObject, 7, icon);
    square = 7;
  }
  else if((displayObject[0][1] == plr_icon)&&(displayObject[1][1] == plr_icon)&&(displayObject[2][1]==' ')) {
    doMovements(displayObject, 8, icon);
    square = 8;
  }
  else if((displayObject[0][2] == plr_icon)&&(displayObject[1][2] == plr_icon)&&(displayObject[2][2]==' ')) {
    doMovements(displayObject, 9, icon);
    square = 9;
  }
  else if((displayObject[2][0] == plr_icon)&&(displayObject[1][0] == plr_icon)&&(displayObject[0][0]==' ')) {
    doMovements(displayObject, 1, icon);
    square = 1;
  }
  else if((displayObject[2][1] == plr_icon)&&(displayObject[1][1] == plr_icon)&&(displayObject[0][1]==' ')) {
    doMovements(displayObject, 2, icon);
    square = 2;
  }
  else if((displayObject[2][2] == plr_icon)&&(displayObject[1][2] == plr_icon)&&(displayObject[0][2]==' ')) {
    doMovements(displayObject, 3, icon);
    square = 3;
  }
  else if((displayObject[0][0] == plr_icon)&&(displayObject[2][0] == plr_icon)&&(displayObject[1][0]==' ')) {
    doMovements(displayObject, 4, icon);
    square = 4;
  }
  else if((displayObject[0][1] == plr_icon)&&(displayObject[2][1] == plr_icon)&&(displayObject[1][1]==' ')) {
    doMovements(displayObject, 5, icon);
    square = 5;
  }
  else if((displayObject[0][2] == plr_icon)&&(displayObject[2][2] == plr_icon)&&(displayObject[1][2]==' ')) {
    doMovements(displayObject, 6, icon);
    square = 6;
  }

  else if((displayObject[0][0] == plr_icon)&&(displayObject[0][1] == plr_icon)&&(displayObject[0][2]==' ')) {
    doMovements(displayObject, 3, icon);
    square = 3;
  }
  else if((displayObject[1][0] == plr_icon)&&(displayObject[1][1] == plr_icon)&&(displayObject[1][2]==' ')) {
    doMovements(displayObject, 6, icon);
    square = 6;
  }
  else if((displayObject[2][0] == plr_icon)&&(displayObject[2][1] == plr_icon)&&(displayObject[2][2]==' ')) {
    doMovements(displayObject, 9, icon);
    square = 9;
  }
  else if((displayObject[0][2] == plr_icon)&&(displayObject[0][1] == plr_icon)&&(displayObject[0][0]==' ')) {
    doMovements(displayObject, 1, icon);
    square = 1;
  }
  else if((displayObject[1][2] == plr_icon)&&(displayObject[1][1] == plr_icon)&&(displayObject[1][0]==' ')) {
    doMovements(displayObject, 4, icon);
    square = 4;
  }
  else if((displayObject[2][2] == plr_icon)&&(displayObject[2][1] == plr_icon)&&(displayObject[2][0]==' ')) {
    doMovements(displayObject, 7, icon);
    square = 7;
  }
  else if((displayObject[0][0] == plr_icon)&&(displayObject[0][2] == plr_icon)&&(displayObject[0][1]==' ')) {
    doMovements(displayObject, 2, icon);
    square = 2;
  }
  else if((displayObject[1][0] == plr_icon)&&(displayObject[1][2] == plr_icon)&&(displayObject[1][1]==' ')) {
    doMovements(displayObject, 5, icon);
    square = 5;
  }
  else if((displayObject[2][0] == plr_icon)&&(displayObject[2][2] == plr_icon)&&(displayObject[2][1]==' ')) {
    doMovements(displayObject, 8, icon);
    square = 8;
  }

  else if((displayObject[0][0] == plr_icon)&&(displayObject[1][1] == plr_icon)&&(displayObject[2][2]==' ')) {
    doMovements(displayObject, 9, icon);
    square = 9;
  }
  else if((displayObject[0][2] == plr_icon)&&(displayObject[1][1] == plr_icon)&&(displayObject[2][0]==' ')) {
    doMovements(displayObject, 7, icon);
    square = 7;
  }
  else if((displayObject[2][0] == plr_icon)&&(displayObject[1][1] == plr_icon)&&(displayObject[0][2]==' ')) {
    doMovements(displayObject, 3, icon);
    square = 3;
  }
  else if((displayObject[2][2] == plr_icon)&&(displayObject[1][1] == plr_icon)&&(displayObject[0][0]==' ')) {
    doMovements(displayObject, 1, icon);
    square = 1;
  }
  else if((displayObject[0][0] == plr_icon)&&(displayObject[2][2] == plr_icon)&&(displayObject[1][1]==' ')) {
    doMovements(displayObject, 5, icon);
    square = 5;
  }
  else if((displayObject[0][2] == plr_icon)&&(displayObject[2][0] == plr_icon)&&(displayObject[1][1]==' ')) {
    doMovements(displayObject, 5, icon);
    square = 5;
  }
  // If none of the above are met, chooses randomly
  else {
    while(1==1) {
      square = rand()%9+1;
      if(doMovements(displayObject, square, icon)==0) {
        break;
      }
    }
  }
  displayBoard(displayObject);
  printf("%s%c%s%i.\n", "Computer has placed ", icon, " in square ", square);
  puts("\n");
}

// Checks if movements are viable (if spaces are empty)
int doMovements(char displayObject[3][3], int square, char icon) {
  int movement;
  if(square == 1) {
    if(displayObject[0][0] == ' ') {
      displayObject[0][0] = icon;
      return 0;
    }
    else {
      return 1;
    }
  }
  else if(square == 2) {
    if(displayObject[0][1] == ' ') {
      displayObject[0][1] = icon;
      return 0;
    }
    else {
      return 1;
    }
  }
  else if(square == 3) {
    if(displayObject[0][2] == ' ') {
      displayObject[0][2] = icon;
      return 0;
    }
    else {
      return 1;
    }
  }
  else if(square == 4) {
    if(displayObject[1][0] == ' ') {
      displayObject[1][0] = icon;
      return 0;
    }
    else {
      return 1;
    }
  }
  else if(square == 5) {
    if(displayObject[1][1] == ' ') {
      displayObject[1][1] = icon;
      return 0;
    }
    else {
      return 1;
    }
  }
  else if(square == 6) {
    if(displayObject[1][2] == ' ') {
      displayObject[1][2] = icon;
      return 0;
    }
    else {
      return 1;
    }
  }
  else if(square == 7) {
    if(displayObject[2][0] == ' ') {
      displayObject[2][0] = icon;
      return 0;
    }
    else {
      return 1;
    }
  }
  else if(square == 8) {
    if(displayObject[2][1] == ' ') {
      displayObject[2][1] = icon;
      return 0;
    }
    else {
      return 1;
    }
  }
  else if(square == 9) {
    if(displayObject[2][2] == ' ') {
      displayObject[2][2] = icon;
      return 0;
    }
    else {
      return 1;
    }
  }
  else {
    return 2;
  }
}
