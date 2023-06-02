/*
  Machine Project Group #4 S20
  Members:
          Cabungcal, Mary Joselle M.
          Ramos, Irish Jane M.
          Rejano, Hans Martin F.
          Uy, Gleezell Vina A.
*/

#include <stdio.h>

#define ROW 7
#define COL 5
#define TRUE 1
#define FALSE 0

//Function prototypes
int isS(int P[][2], int x, int y);
int isP(int P[][2], int x, int y);
void getPrev(int P[][2], int *x, int *y);
void getNext(int P[][2], int *x, int *y);
int isS(int P[][2], int x, int y);
int isAlpha(int Alpha[][2], int x, int y, int P[][2]);
int isBeta(int Beta[][2], int x, int y, int P[][2]);
int isFree(int P[][2], int x, int y, int Alpha[][2], int Beta[][2]);
int searchAlpha(int Alpha[][2], int x, int y, int *r);
void replaceAlpha(int Alpha[][2], int x, int y, int c, int d);
int searchBeta(int Beta[][2], int x, int y, int *r);
void replaceBeta(int Beta[][2], int x, int y, int c, int d);
void deleteAlpha(int Alpha[][2], int c, int d, int *pAlphaCnt);
void deleteBeta(int Beta[][2], int c, int d, int *pBetaCnt);
int isOver(int nAlphaCnt, int nBetaCnt, int Alpha[][2], int Beta[][2],
           int Y[][2], int E[][2], int P[][2]);
void gameOver(int nAlphaCnt, int nBetaCnt, int Alpha[][2], int Beta[][2],
              int Y[][2], int E[][2], int nDraw);
void displayPlayers(int Alpha[][2], int Beta[][2], int P[][2]);


int main(void) 
{
  // variable declaration
  int P[ROW * COL][2];
  int i, j, k;
  int a, b, c, d;
  int ok, aTurn, nValid;
  int Alpha[5][2];
  int Beta[5][2];
  
  int E[5][2] = {{6, 2}, // {(x, y) ∈ S | x ≥ 6}
                 {6, 4},
                 {7, 1},
                 {7, 3},
                 {7, 5}};

  int Y[5][2] = {{1, 1}, // {(x, y) ∈ S | x ≤ 2}
                 {1, 3},
                 {1, 5},
                 {2, 2},
                 {2, 4}};
  int nAlphaCnt = 5;
  int nBetaCnt = 5;
  // variable initialize
  ok = FALSE;
  aTurn = TRUE;

  // copies starting locations of Alpha, Alpha = E
  for (i = 0; i < 5; i++) 
  {
    for (j = 0; j < 2; j++) 
    {
      Alpha[i][j] = E[i][j];
    }
  }
  // copies starting locations of Beta, Beta = Y
  for (i = 0; i < 5; i++) 
  {
    for (j = 0; j < 2; j++) 
    {
      Beta[i][j] = Y[i][j];
    }
  }

  printf("==================================\n"); // Divider
  printf("    --- COORDINATES  GUIDE --- \n");
  // Coordinates of the Board

  //Displaying Set P
  i = 0; // first sqr of the Board
  for (j = 0; j < ROW; j++) 
  {
    for (k = 0; k < COL; k++) 
    {
      P[i][0] = j + 1; // x
      P[i][1] = k + 1; // y
      printf("(%d, %d) ", P[i][0], P[i][1]);
      i++;
    }
    printf("\n");
  }

  //System States and Behavior
  do {
    ok = FALSE;
    nValid = 0;

    printf("========================\n"); // Divider

    displayPlayers(Alpha, Beta, P);
    printf("Alpha:%d\nBeta:%d\n", nAlphaCnt, nBetaCnt);
    if (aTurn)
      printf("Alpha's move\n");
    else
      printf("Beta's move\n");

    // (a,b) = prev
    getPrev(P, &a, &b); 
    printf("I got prev(%d, %d)\n", a, b);

    // (c,d) = next
    printf("next:\n");
    getNext(P, &c, &d); 
    printf("I got next(%d, %d)\n", c, d);

    //aTurn ∧ prev ∈ Alpha ∧ a = c + 1 ∧ (d = b ∨ d = b + 1 ∨ b = d + 1)
    if (aTurn && ((isAlpha(Alpha, a, b, P) && a == c + 1) &&
                  (d == b || d == b + 1 || b == d + 1))) 
    {
      ok = !ok; //ok = ¬ok
      nValid++;
    }

    //¬aTurn ∧ prev ∈ Beta ∧ c = a + 1 ∧ (d = b ∨ d = b + 1 ∨ b = d + 1) 
    if (!aTurn && (isBeta(Beta, a, b, P) && c == a + 1) &&
        (d == b || d == b + 1 || b == d + 1)) 
    {
      ok = !ok; //ok = ¬ok
      nValid++;
    }

    //ok ∧ aTurn ∧ next ∈ Free
    if (ok && aTurn && isFree(P, c, d, Alpha, Beta)) 
    { 
      replaceAlpha(Alpha, a, b, c, d); //Alpha = (Alpha − {prev}) ∪ {next}
      aTurn = !aTurn; //aTurn = ¬aTurn
      ok = !ok; //ok = ¬ok
      nValid++;
    }

    //ok ∧ ¬aTurn ∧ next ∈ Free
    if (ok && !aTurn && isFree(P, c, d, Alpha, Beta)) 
    {
      replaceBeta(Beta, a, b, c, d); //Beta = (Beta − {prev}) ∪ {next}
      aTurn = !aTurn; //aTurn = ¬aTurn
      ok = !ok; //ok = ¬ok
      nValid++;
    }

    //ok ∧ aTurn ∧ next ∈ Beta ∧ next !∈ S
    if (ok && aTurn && isBeta(Beta, c, d, P) && !isS(P, c, d)) {
      ok = !ok; //ok = ¬ok
    }

    //ok ∧ aTurn ∧ next ∈ Beta ∧ next ∈ S
    if (ok && aTurn && isBeta(Beta, c, d, P) && isS(P, c, d)) 
    {
      deleteBeta(Beta, c, d, &nBetaCnt); //Beta = Beta − {next}
      replaceAlpha(Alpha, a, b, c, d); //Alpha = (Alpha − {prev}) ∪ {next}
      aTurn = !aTurn; //aTurn = ¬aTurn
      ok = !ok; //ok = ¬ok
      nValid++;
    }

    //ok ∧ ¬aTurn ∧ next ∈ Alpha ∧ next !∈ S
    if (ok && !aTurn && isAlpha(Alpha, c, d, P) && !isS(P, c, d)) 
    {
      ok = !ok; //ok = ¬ok
    }

    // ok ∧ ¬aTurn ∧ next ∈ Alpha ∧ next ∈ S
    if (ok && !aTurn && isAlpha(Alpha, c, d, P) && isS(P, c, d)) 
    {
      deleteAlpha(Alpha, c, d, &nAlphaCnt); //Alpha = Alpha − {next}
      replaceBeta(Beta, a, b, c, d); //Beta = (Beta − {prev}) ∪ {next}
      aTurn = !aTurn; //aTurn = ¬aTurn
      ok = !ok; //ok = ¬ok
      nValid++;
    }

    if (nValid < 2)
      printf("Your move is Invalid.\n");

  } while (!isOver(nAlphaCnt, nBetaCnt, Alpha, Beta, Y, E, P)); //over initialized as false or 0

  printf("========================\n");

  displayPlayers(Alpha, Beta, P);

  gameOver(nAlphaCnt, nBetaCnt, Alpha, Beta, Y, E,
           isOver(nAlphaCnt, nBetaCnt, Alpha, Beta, Y, E, P));

  return 0;
}

/**
 * isP() checks if coordinate is in set P
 *
 * @param P the array of set P coordinates
 * @param x the x or R coordinate of the point
 * @param y the y or C coordinate of the point
 *
 * @return 1 if x and y is member of P, return 0 if not.
 */
int isP(int P[][2], 
        int x, 
        int y) 
{
  int i, j, k;

  i = 0;
  for (j = 0; j < ROW; j++) 
  {
    for (k = 0; k < COL; k++) 
    {
      if (P[i][0] == x && P[i][1] == y) 
      {
        return 1;
      }
      i++;
    }
  }

  return 0;
}

/**
 * getPrev() asks for a valid first pair of coordinates
 *
 * @param P the array of set P coordinates
 * @param *x a pointer that will contain x or R coordinate
 * @param *y a pointer that will contain y or C coordinate
 */
void getPrev  (int P[][2], 
              int *x, 
              int *y) 
{
  do 
  {
    printf("enter coordinate:\n");
    printf("a: ");
    scanf("%d", &*x);
    printf("b: ");
    scanf("%d", &*y);
    // ask until x,y is element of P
  } while (isP(P, *x, *y) != 1);
}

/**
 * getNext() asks the user the coordinates 
 * where the previous pair will move
 *
 * @param P the array of set P coordinates
 * @param *x a pointer that will contain x or R coordinate
 * @param *y a pointer that will contain y or C coordinate
 */
void getNext( int P[][2], 
              int *x, 
              int *y) {
  do 
  {
    printf("enter coordinate:\n");
    printf("c: ");
    scanf("%d", &*x);
    printf("d: ");
    scanf("%d", &*y);
    // ask until x,y is element of P
  } while (isP(P, *x, *y) != 1);
}

/**
 * isS() checks the input pair if it is a member of set S
 *
 * @param P the array of set P coordinates
 * @param x the x or R coordinate of the point
 * @param y the y or C coordinate of the point
 *
 * @return 1 if the point (x, y) is member of set S, and  return 0
 * otherwise.
 */
int isS(int P[][2], 
        int x, 
        int y) 
{
  int i;
  int j, k;

  i = 0;
  for (j = 0; j < ROW; j++) 
  {
    for (k = 0; k < COL; k++) 
    {
      if (P[i][0] == x && P[i][1] == y && x % 2 == y % 2) 
      {
        return 1;
      }
      i++;
    }
  }
  return 0;
}

/**
 * isAlpha() checks if the point (x,y) is a member of Alpha
 *
 * @param Alpha the array of Alpha points
 * @param x the x or R coordinate of the point
 * @param y the y or C coordinate of the point
 * @param P the array of set P coordinates
 *
 * @return 1 if point is member of Alpha, 0 if otherwise
 */
int isAlpha(int Alpha[][2], 
            int x, 
            int y, 
            int P[][2]) 
{
  int i;
  for (i = 0; i < 5; i++) 
  {
    //checks if the point (x,y) is a member of Alpha
    if (Alpha[i][0] == x && Alpha[i][1] == y) 
    {
      return 1;
    }
  }
  return 0;
}

/**
 * isBeta() checks if the point (x,y) is an beta point
 *
 * @param Beta The array of points that are in the beta set.
 * @param x the x or R coordinate of the point
 * @param y the y or C coordinate of the point
 * @param P the array of set P coordinates
 *
 * @return 1 if point is member of Beta, 0 if otherwise
 */
int isBeta( int Beta[][2], 
            int x,
            int y, 
            int P[][2]) 
{
  int i;

  for (i = 0; i < 5; i++) 
  {
    //checks if the point (x,y) is a member of Beta
    if (Beta[i][0] == x && Beta[i][1] == y) 
    {
      return 1;
    }
  }
  return 0;
}

/**
 * isFree() checks if a given point is on set Free 
 *
 * @param P the array of set P coordinates
 * @param x the x or R coordinate of the point
 * @param y the y or C coordinate of the point
 * @param Alpha the set of points that are in the Alpha set
 * @param Beta the set of points that are in the Beta set
 */
int isFree( int P[][2], 
            int x, 
            int y, 
            int Alpha[][2], 
            int Beta[][2]) 
{
  int i, j;

  for (i = 0; i < ROW; i++) 
  {
    for (j = 0; j < COL; j++) 
    {
      if ( isP(P, x, y) && 
          !(isAlpha(Alpha, x, y, P) || 
            isBeta(Beta, x, y, P))) 
      {
        return 1;
      }
    }
  }

  return 0;
}

/**
 * searchAlpha() searches the array Alpha for the coordinates x and y. If found,
 * the function returns nFound and stores the row and column index of the found
 * coordinates in the variables pointed to by r and c
 *
 * @param Alpha the array of coordinates in set Alpha
 * @param x the x or R coordinate of the point
 * @param y the y or C coordinate of the point
 * @param r row index
 * 
 * @return nFound value either 1 if found, 0 otherwise
 */
int searchAlpha(int Alpha[][2], int x, int y, int *r) 
{
  int i;
  int nFound = 0;

  for (i = 0; i < 5; i++) 
  {
    if (Alpha[i][0] == x && Alpha[i][1] == y) 
    {
      nFound = 1; //found
      *r = i; // store to *r index of row
    }
  }

  return nFound;
}

/**
 * replaceAlpha() replaces (a,b) coordinate if found in set Alpha with (c,d) coordinate
 * 
 * @param Alpha The array of coordinates in set Alpha
 * @param x the a in coordinate (a,b)
 * @param y the b in coordinate (a,b)
 * @param c the x in (c,d)
 * @param d the y in (c,d)
 */
void replaceAlpha(int Alpha[][2], 
                  int x, 
                  int y, 
                  int c, 
                  int d) 
{
  int row;

  // if coordinate (x,y) is found
  if (searchAlpha(Alpha, x, y, &row)) 
  {
    // x of next
    Alpha[row][0] = c;
    // y of next
    Alpha[row][1] = d;
  }
}

/**
 * The function searchBeta() searches the array Beta[][2] for the values x and
 * y. If the values are found, the function returns 1 and stores the row and
 * column indices of the found values in the variables pointed to by r and c. If
 * the values are not found, the function returns 0
 *
 * @param Beta the array of the set Beta
 * @param x the x or R coordinate of the point
 * @param y the y or C coordinate of the point
 * @param r row index
 *
 * @return 1 if found, return 0 if otherwise
 */
int searchBeta( int Beta[][2], 
                int x, 
                int y, 
                int *r) 
{
  int i;

  for (i = 0; i < 5; i++) 
  {
      if (Beta[i][0] == x && Beta[i][1] == y) 
      {
        *r = i; // store to *r index of row
        return 1;
      }
  }
  return 0;
}

/**
 * replaceBeta() replaces (a,b) coordinate with (c,d) coordinate if found in set Beta
 *
 * @param Beta the array of coordinates in set Beta
 * @param x the a in coordinate (a,b)
 * @param y the b in coordinate (a,b)
 * @param c the x in (c,d)
 * @param d the y in (c,d)
 */
void replaceBeta( int Beta[][2], 
                  int x, 
                  int y, 
                  int c, 
                  int d) 
{
  int row;

  // if coordinate (x,y) is found
  if (searchBeta(Beta, x, y, &row)) 
  {
    // x of next
    Beta[row][0] = c;
    // y of next
    Beta[row][1] = d;
  }
}

/**
 * deleteAlpha searches for the pair (c,d) in the array Alpha. If found,
 * it replaces it with the last pair in the array and decrements the count of
 * pairs in the array
 * 
 * @param Alpha The array of coordinates in set Alpha
 * @param c the x coordinate of the current position
 * @param d the y coordinate of the current position
 * @param pAlphaCnt the number of elements in Alpha
 */
void deleteAlpha( int Alpha[][2],  
                  int c, 
                  int d,
                  int *pAlphaCnt) 
{
  int row;

  if (searchAlpha(Alpha, c, d, &row) == 1) 
  {
    // replace with x of last element in Alpha
    Alpha[row][0] = Alpha[*pAlphaCnt - 1][0];
    Alpha[*pAlphaCnt - 1][0] = 0;
    // replace with y of last element in Alpha
    Alpha[row][1] = Alpha[*pAlphaCnt - 1][1];
    Alpha[*pAlphaCnt - 1][1] = 0;
    (*pAlphaCnt)--;
  }
}

/**
 * deleteBeta() searches for the pair (c,d) in the array Beta. If found,
 * it replaces it with the last pair in the array and decrements the count of
 * pairs in the array
 *
 * @param Beta the array of beta pairs
 * @param c the x coordinate of the point to be deleted
 * @param d the number of rows in the matrix
 * @param pBetaCnt pointer to the number of elements in Beta
 */
void deleteBeta(int Beta[][2],  
                int c, 
                int d,
                int *pBetaCnt) 
{
  int row;

  if (searchBeta(Beta, c, d, &row)) 
  {
    // replace with x of last element in Beta
    Beta[row][0] = Beta[*pBetaCnt - 1][0];
    Beta[*pBetaCnt - 1][0] = 0;
    // replace with y of last element in Beta
    Beta[row][1] = Beta[*pBetaCnt - 1][1];
    Beta[*pBetaCnt - 1][1] = 0;
    (*pBetaCnt)--;
  }
}

/**
 * isOver() checks If the count of set Alpha or set Beta is zero, 
 * or if the count of set Alpha or set Beta is not zero 
 * and the number of Alpha or Beta that can move is zero, then return 1
 * 
 * @param nAlphaCnt number of Alpha pieces
 * @param nBetaCnt number of Beta pieces
 * @param Alpha The array of set Alpha 
 * @param Beta The array of set Beta 
 * @param Y the current state of the board
 * @param E The enemy's current position
 * @param P the array of set P
 *
 * @return 1 if Alpha or Beta wins, return 2 if draw, return 0 if not yet over
 */

int isOver( int nAlphaCnt, 
            int nBetaCnt, 
            int Alpha[][2], 
            int Beta[][2],
            int Y[][2], 
            int E[][2], 
            int P[][2]) 
{
  int rDump;
  int nAlphaMove = nAlphaCnt;
  int nBetaMove = nBetaCnt;
  int nDrawA = 0, nDrawB = 0, nDrawF = 0;

  int i;

  for (i = 0; i < 5; i++) 
  {
    if (searchAlpha(Alpha, Y[i][0], Y[i][1], &rDump))
      nAlphaMove--;
  }

  for (i = 0; i < 5; i++) 
  {
    if (searchBeta(Beta, E[i][0], E[i][1], &rDump))
      nBetaMove--;
  }

  if (searchAlpha(Alpha, P[1][0], P[1][1], &rDump)) 
  {
    nAlphaMove--;
    nDrawA++;
  }

  if (searchAlpha(Alpha, P[3][0], P[3][1], &rDump)) 
  {
    nAlphaMove--;
    nDrawA++;
  }

  if (searchBeta(Beta, P[31][0], P[31][1], &rDump)) 
  {
    nBetaMove--;
    nDrawB++;
  }

  if (searchBeta(Beta, P[33][0], P[33][1], &rDump)) 
  {
    nBetaMove--;
    nDrawB++;
  }

  if ((nDrawB > 0 && nBetaMove == 0) || (nDrawA > 0 && nAlphaMove == 0))
    nDrawF = 1;

  if(nAlphaCnt == 0 || nBetaCnt == 0 ||
    (nAlphaCnt != 0 && nAlphaMove == 0 && nDrawF == 0) || // Alpha-Y
      (nBetaCnt != 0 && nBetaMove == 0 && nDrawF == 0)) // Beta - Y  
    return 1;

  else if (nDrawF)
    return 2;

  return 0;
}

/**
 * gameOver() checks If Alpha has no more moves, Beta wins. If Beta has no more moves, Alpha wins
 *
 * @param nAlphaCnt The number of Alpha pieces on the board
 * @param nBetaCnt number of Beta pieces
 * @param Alpha The Alpha player's pieces
 * @param Beta The Beta player's pieces
 * @param Y The array of coordinates of the Alpha pieces
 * @param E The array of the enemy's pieces
 * @param nDraw the Draw indicator of the game (if 2, Draw)
 */

void gameOver(int nAlphaCnt, 
              int nBetaCnt, 
              int Alpha[][2], 
              int Beta[][2], 
              int Y[][2], 
              int E[][2], 
              int nDraw) 
{
  int rDump;
  int nAlphaMove = nAlphaCnt;
  int nBetaMove = nBetaCnt;

  int i;

  for (i = 0; i < 5; i++) 
  {
    if (searchAlpha(Alpha, Y[i][0], Y[i][1], &rDump)) 
      nAlphaMove--;
  }

  for (i = 0; i < 5; i++) 
  {
    if (searchBeta(Beta, E[i][0], E[i][1], &rDump)) 
      nBetaMove--;
  }

  if (nBetaCnt == 0 || (nAlphaCnt != 0 && nAlphaMove == 0)) 
    printf("====== ALPHA WINS ======\n");

  else if (nAlphaCnt == 0 || (nBetaCnt != 0 && nBetaMove == 0)) 
    printf("======= BETA WINS ======\n");

  else if (nDraw == 2)
    printf("========= DRAW ========\n");
}

/**
 * displayPlayers() displays the board  
 *
 * @param Alpha The array of set Alpha
 * @param Beta the array of set Beta
 * @param P the array of set P
 */
void displayPlayers(int Alpha[][2], 
                    int Beta[][2], 
                    int P[][2]) 
{
  int rDump;
  int i, j, k;
  int l = 1;

  printf("  +---+---+---+---+---+\n");

  i = 0; // first sqr of the Board
  for (j = 0; j < ROW; j++) {

    printf("%d |", l);

    for (k = 0; k < COL; k++) 
    {
      if (searchAlpha(Alpha, P[i][0], P[i][1], &rDump)) 
        printf(" A |");
      
      else if (searchBeta(Beta, P[i][0], P[i][1], &rDump)) 
        printf(" B |");

      else if (isS(P, j + 1, k + 1)) 
        printf(" * |");
      
      else
        printf("   |");

      i++;
    }
    
    printf("\n");
    printf("  +---+---+---+---+---+\n");
    l++;
  }
  printf("    1   2   3   4   5   \n");
}
