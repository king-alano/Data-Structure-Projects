#ifndef CHECKERS_H
#define CHECKERS_H
#include <iostream>
#include <string>
#include <math.h>
using namespace std;
class Grader; // this class is for grading purposes, no need to do anything
class Tester; // this is your tester class, you add your test functions in this class

// the following define statemetns are used for graphical representation.
#define WDISK "\033[7mWD\033[0m" // represents white disk
#define BDISK "\033[7mBD\033[0m" // represents black disk
#define WKING "\033[7mWK\033[0m" // represents white king
#define BKING "\033[7mBK\033[0m" // represents black king
#define WCELL "  "               // represents white cell
#define BCELL "\033[7m  \033[0m" // represents black cell
#define DEFBOARDSIZE 8           // default board size is 8 x 8
enum PLAYER
{
    WPLAYER,
    BPLAYER,
    NONE
};
typedef string CELL;

class Checkers
{
public:
    friend class Grader;
    friend class Tester;
    Checkers();
    Checkers(int rows, int columns);
    ~Checkers();
    void clear();
    void initBoard(); // creates the board
    void initGame();  // creates the initial state of disks
    // the following creates a board with the provided state of disks
    void setGameState(CELL game[], int n, PLAYER player);
    bool play(pair<int, int> origin, pair<int, int> destination);
    void dumpGame();        // for debugging purposes
    void reportLostDisks(); // reports current number of lost disks per player

private:
    int m_numRows;       // can be any sizes
    int m_numColumns;    // can be any sizes
    CELL **m_game;       // the 2d structure to store game information
    PLAYER m_currPlayer; // must be toggled at every turn
    int m_whiteDisks;    // number of current white disks, must be updated at every capture
    int m_blackDisks;    // number of current black disks, must be updated at every capture

    /******************************************
     * Private function declarations go here! *
     ******************************************/
    int diff(int, int);                 // returns the difference between two integers
    bool validKing(int, int, int, int); // validates a king's movement
};
#endif
