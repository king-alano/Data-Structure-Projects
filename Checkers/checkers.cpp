// UMBC - CMSC 341 - Spring 2023 - Proj0
/*
  auth: AJ Boyd (aboyd3)
  date: 1/31/23
  desc: implimentation of the pubic methods for the Checkers class.
*/
#include "checkers.h"

// default constructor intializes MVs to be empty values
Checkers::Checkers()
{
    m_numRows = 0;
    m_numColumns = 0;
    m_game = nullptr;
    m_currPlayer = NONE;
    m_whiteDisks = 0;
    m_blackDisks = 0;
}

Checkers::Checkers(int rows, int columns)
{
    // validate rows and columns
    // rows and columns must be equal, greater than or equal to 8, and even
    // if not, make empty object
    if ((rows != columns) ||
        (rows < DEFBOARDSIZE || columns < DEFBOARDSIZE) ||
        (rows % 2 != 0 || columns % 2 != 0))
    {
        m_numRows = 0;
        m_numColumns = 0;
        m_game = nullptr;
        m_currPlayer = NONE;
        m_whiteDisks = 0;
        m_blackDisks = 0;
    }
    else
    {
        m_numRows = rows;
        m_numColumns = columns;
        m_currPlayer = BPLAYER;
        m_whiteDisks = (columns / 2) * 3;
        m_blackDisks = m_whiteDisks;
        m_game = new CELL *[m_numRows];
        for (int i = 0; i < m_numRows; i++)
        {
            m_game[i] = new CELL[m_numColumns];
        }
    }
}

Checkers::~Checkers()
{
    clear();
}

void Checkers::clear()
{
    for (int i = 0; i < m_numRows; i++)
    {
        delete[] m_game[i];
        m_game[i] = nullptr;
    }
    delete[] m_game;
    m_game = nullptr;
}

// initializes the gameboard
void Checkers::initBoard()
{
    if (m_game)
    {
        for (int i = 0; i < m_numRows; i++)
        {
            for (int j = 0; j < m_numColumns; j++)
            {
                if (i % 2 == 0)
                {
                    j % 2 == 0 ? m_game[i][j] = WCELL : m_game[i][j] = BCELL;
                }
                else
                {
                    j % 2 == 0 ? m_game[i][j] = BCELL : m_game[i][j] = WCELL;
                }
            }
        }
    }
}

void Checkers::reportLostDisks()
{
    int init = (m_numColumns / 2) * 3;
    int currentW = 0;
    int currentB = 0;

    for (int i = 0; i < m_numRows; i++)
    {
        for (int j = 0; j < m_numColumns; j++)
        {
            if (m_game[i][j] == WDISK || m_game[i][j] == WKING)
            {
                currentW++;
            }
            else if (m_game[i][j] == BDISK || m_game[i][j] == BKING)
            {
                currentB++;
            }
        }
    }

    cout << "White player lost disks: " << (init - currentW) << endl;
    cout << "Black player lost disks: " << (init - currentB) << endl;
}

// puts pieces onto newly made gameboard
void Checkers::initGame()
{
    if (m_game)
    {
        // add white disks
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < m_numColumns; j++)
            {
                if (m_game[i][j] == BCELL)
                {
                    m_game[i][j] = WDISK;
                }
            }
        }

        // add black disks
        for (int i = m_numRows - 1; i > m_numRows - 4; i--)
        {
            for (int j = 0; j < m_numColumns; j++)
            {
                if (m_game[i][j] == BCELL)
                {
                    m_game[i][j] = BDISK;
                }
            }
        }
    }
}

// validates a proper mvoe
/*reqs:
  -destination's coords need to be in the range: [0,m_numRows)
  -destination needs to be a BCELL
  -origin needs to have the player's disk on it
*/
bool Checkers::play(pair<int, int> origin, pair<int, int> destination)
{
    // better param names
    int x0 = origin.first;
    int xf = destination.first;
    int y0 = origin.second;
    int yf = destination.second;

    if (m_game)
    {
        // validate bounds
        // if either coordinte is out of range of the board, return false
        if ((x0 < 0 || x0 >= m_numRows) ||
            (y0 < 0 || y0 >= m_numRows) ||
            (xf < 0 || xf >= m_numRows) ||
            (yf < 0 || yf >= m_numRows))
        {
            return false;
        }
        // validate origin
        // if there isn't a piece @ origin coords, return false
        if (m_game[x0][y0] == WCELL || m_game[x0][y0] == BCELL)
        {
            return false;
        }
        // given there IS a piece @ origin coords, that piece and m_currPlayer's color must match
        // if not, return false
        if ((m_currPlayer == BPLAYER && (m_game[x0][y0] == WDISK || m_game[x0][y0] == WKING)) ||
            (m_currPlayer == WPLAYER && (m_game[x0][y0] == BDISK || m_game[x0][y0] == BKING)))
        {
            return false;
        }
        // validate destination
        // if the destination coord is NOT a BCELL, return false
        if (m_game[xf][yf] != BCELL)
        {
            return false;
        }
        // validate movement
        // kings have weird movement, i validate that in a seperate helper function
        if (m_game[x0][y0] == WKING || m_game[x0][y0] == BKING)
        {
            bool valid = validKing(x0, y0, xf, yf);
            if (valid == false)
            {
                return false;
            }
            else
            {
                // make move

                return true;
            }
        }
        /*regular moves:
          --if Black is moving, they must be moving up 1 and to the side either -1 or 1
          --if White is moving, they must be moving down 1 and to the side either -1 or 1
          captures:
          --if Black is moving, they must be moving up 2 and to the side either -2 or 2
          --if White is moving, they must be moving down 2 and to the side either -2 or 2
          --also, there must be an enemy piece in between the final coord to capture
        */
        if (m_currPlayer == BPLAYER && xf < x0)
        {
            //    cout << "black attempts to move" << endl;
            // test black move
            if (diff(xf, x0) == -1 && abs(diff(yf, y0)) == 1)
            {
                // if piece moves to the opposite side, promote it to a king
                (xf == 0) ? m_game[xf][yf] = BKING : m_game[xf][yf] = m_game[x0][y0];
                m_game[x0][y0] = BCELL; // origin becomes empty BCELL
                m_currPlayer = WPLAYER; // change players
                cout << "valid move!" << endl;
                return true;
            }
            // test black captures
            else if (diff(xf, x0) == -2 && abs(diff(yf, y0)) == 2)
            {
                if (diff(yf, y0) == -2)
                {
                    if (m_game[x0 - 1][y0 - 1] == WDISK || m_game[x0 - 1][y0 - 1] == WKING)
                    {
                        (xf == 0) ? m_game[xf][yf] = BKING : m_game[xf][yf] = m_game[x0][y0]; // move piece; watch for capture-promotion
                        m_game[x0][y0] = BCELL;                                               // replace origin with empty BCELL
                        m_game[x0 - 1][y0 - 1] = BCELL;                                       // replace captured white piece with BCELL
                        m_currPlayer = WPLAYER;                                               // change player turn
                        m_whiteDisks--;                                                       // decrease white pieces
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    if (m_game[x0 - 1][y0 + 1] == WDISK || m_game[x0 - 1][y0 + 1] == WKING)
                    {
                        (xf == 0) ? m_game[xf][yf] = BKING : m_game[xf][yf] = m_game[x0][y0];
                        m_game[x0][y0] = BCELL;
                        m_game[x0 - 1][y0 + 1] = BCELL;
                        m_currPlayer = WPLAYER;
                        m_whiteDisks--;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }

        else if (m_currPlayer == WPLAYER && xf > x0)
        {
            // test white move
            if (diff(xf, x0) == 1 && abs(diff(yf, y0)) == 1)
            {
                (xf == m_numRows - 1) ? m_game[xf][yf] = WKING : m_game[xf][yf] = m_game[x0][y0]; // move piece; watch for promotion
                m_game[x0][y0] = BCELL;                                                           // origin becomes empty BCELL
                m_currPlayer = BPLAYER;                                                           // change players
                return true;
            }
            // test white capture
            else if (diff(xf, x0) == 2 && abs(diff(yf, y0)) == 2)
            {
                if (diff(xf, x0) == 2 && abs(diff(yf, y0)) == 2)
                {
                    if (diff(yf, y0) == -2)
                    {
                        if (m_game[x0 + 1][y0 - 1] == BDISK || m_game[x0 + 1][y0 - 1] == BKING)
                        {                                                                                     // check if black piece is between origin and destination
                            (xf == m_numRows - 1) ? m_game[xf][yf] = WKING : m_game[xf][yf] = m_game[x0][y0]; // move piece; watch for capture-promotion
                            m_game[x0][y0] = BCELL;                                                           // replace origin with empty BCELL
                            m_game[x0 + 1][y0 - 1] = BCELL;                                                   // replace captured black piece with empty BCELL
                            m_currPlayer = BPLAYER;                                                           // change players
                            m_blackDisks--;                                                                   // lower number of black pieces
                            return true;
                        }
                        return false;
                    }
                    else
                    {
                        if (m_game[x0 + 1][y0 + 1] == BDISK || m_game[x0 + 1][y0 + 1] == BKING)
                        {
                            (xf == m_numRows - 1) ? m_game[xf][yf] = WKING : m_game[xf][yf] = m_game[x0][y0];
                            m_game[x0][y0] = BCELL;
                            m_game[x0 + 1][y0 + 1] = BCELL;
                            m_currPlayer = BPLAYER;
                            m_blackDisks--;
                            return true;
                        }
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
        return false;
    }
    else
    {
        return false;
    }
}

void Checkers::setGameState(CELL game[], int n, PLAYER player)
{
    if (n == m_numRows * m_numColumns)
    {
        // reset number of disks
        m_whiteDisks = 0;
        m_blackDisks = 0;

        // loop through 1D array and map it to the 2D array m_game
        for (int i = 0; i < n; i++)
        {
            // if not empty, replace
            if (game[i] != "")
            {
                m_game[i / m_numRows][i % m_numRows] = game[i];
                // if index is a piece, add to respective piece total
                if (m_game[i / m_numRows][i % m_numRows] == WDISK || m_game[i / m_numRows][i % m_numRows] == WKING)
                {
                    m_whiteDisks++;
                }
                else if (m_game[i / m_numRows][i % m_numRows] == BDISK || m_game[i / m_numRows][i % m_numRows] == BKING)
                {
                    m_blackDisks++;
                }
            }
        }

        // sets player
        m_currPlayer = player;
    }
}

// given the starting and final coords, determine if the move is valid given a king's movement rules
bool Checkers::validKing(int x0, int y0, int xf, int yf)
{
    // if King does regular move
    if (abs(diff(xf, x0)) == 1 && abs(diff(xf, x0)) == 1)
    {
        m_game[xf][yf] = m_game[x0][y0];                                             // destination equals a KING
        m_game[x0][y0] = BCELL;                                                      // origin is now an empty cell
        (m_currPlayer == BPLAYER) ? m_currPlayer = WPLAYER : m_currPlayer = BPLAYER; // toggle player
        return true;
    }
    // if King does a capture
    else if (abs(diff(xf, x0)) == 2 && abs(diff(xf, x0)) == 2)
    {
        // Black King
        if (m_currPlayer == BPLAYER)
        {
            if (diff(xf, x0) == -2 && diff(yf, y0) == -2)
            {
                if (m_game[x0 - 1][y0 - 1] == WDISK || m_game[x0 - 1][y0 - 1] == WKING)
                {                                    // captured piece must be a White piece
                    m_game[xf][yf] = m_game[x0][y0]; // destination should be BKING
                    m_game[x0 - 1][y0 - 1] = BCELL;  // captured piece should be empty cell
                    m_game[x0][y0] = BCELL;          // origin should be empty cell
                    m_whiteDisks--;                  // lower White's disks
                    m_currPlayer = WPLAYER;          // change player
                    return true;
                }
            }
            else if (diff(xf, x0) == -2 && diff(yf, y0) == 2)
            {
                if (m_game[x0 - 1][y0 + 1] == WDISK || m_game[x0 - 1][y0 - 1] == WKING)
                {
                    m_game[xf][yf] = m_game[x0][y0];
                    m_game[x0 - 1][y0 + 1] = BCELL;
                    m_game[x0][y0] = BCELL;
                    m_whiteDisks--;
                    m_currPlayer = WPLAYER;
                    return true;
                }
            }
            else if (diff(xf, x0) == 2 && diff(yf, y0) == -2)
            {
                if (m_game[x0 + 1][y0 - 1] == WDISK || m_game[x0 - 1][y0 - 1] == WKING)
                {
                    m_game[xf][yf] = m_game[x0][y0];
                    m_game[x0 + 1][y0 - 1] = BCELL;
                    m_whiteDisks--;
                    m_game[x0][y0] = BCELL;
                    m_currPlayer = WPLAYER;
                    return true;
                }
            }
            else if (diff(xf, x0) == 2 && diff(yf, y0) == 2)
            {
                if (m_game[x0 + 1][y0 + 1] == WDISK || m_game[x0 - 1][y0 - 1] == WKING)
                {
                    m_game[xf][yf] = m_game[x0][y0];
                    m_game[x0 + 1][y0 + 1] = BCELL;
                    m_game[x0][y0] = BCELL;
                    m_whiteDisks--;
                    m_currPlayer = WPLAYER;
                    return true;
                }
            }
            else
            {
                return false;
            }
            return false;
        }
        // White King
        else if (m_currPlayer == WPLAYER)
        {
            if (diff(xf, x0) == -2 && diff(yf, y0) == -2)
            {
                if (m_game[x0 - 1][y0 - 1] == BDISK || m_game[x0 - 1][y0 - 1] == BKING)
                {
                    m_game[xf][yf] = m_game[x0][y0]; // destination is now a WKING
                    m_game[x0 - 1][y0 - 1] = BCELL;  // captured piece is now an empty BCELL
                    m_game[x0][y0] = BCELL;          // origin is now BCELL
                    m_blackDisks--;                  // black disks go down
                    m_currPlayer = BPLAYER;          // toggle turn
                    return true;
                }
            }
            else if (diff(xf, x0) == -2 && diff(yf, y0) == 2)
            {
                if (m_game[x0 - 1][y0 + 1] == BDISK || m_game[x0 - 1][y0 - 1] == BKING)
                {
                    m_game[xf][yf] = m_game[x0][y0];
                    m_game[x0 - 1][y0 + 1] = BCELL;
                    m_game[x0][y0] = BCELL;
                    m_blackDisks--;
                    m_currPlayer = BPLAYER;
                    return true;
                }
            }
            else if (diff(xf, x0) == 2 && diff(yf, y0) == -2)
            {
                if (m_game[x0 + 1][y0 - 1] == BDISK || m_game[x0 - 1][y0 - 1] == BKING)
                {
                    m_game[xf][yf] = m_game[x0][y0];
                    m_game[x0 + 1][y0 - 1] = BCELL;
                    m_game[x0][y0] = BCELL;
                    m_blackDisks--;
                    m_currPlayer = BPLAYER;
                    return true;
                }
            }
            else if (diff(xf, x0) == 2 && diff(yf, y0) == 2)
            {
                if (m_game[x0 + 1][y0 + 1] == BDISK || m_game[x0 - 1][y0 - 1] == BKING)
                {
                    m_game[xf][yf] = m_game[x0][y0];
                    m_game[x0 + 1][y0 + 1] = BCELL;
                    m_game[x0][y0] = BCELL;
                    m_blackDisks--;
                    m_currPlayer = BPLAYER;
                    return true;
                }
            }
            else
            {
                return false;
            }
            return false;
        }
    }
    else
    {
        return false;
    }
    return false;
}

// helper function that validates a king's movement
// helper function to retun the absolute value of a difference
int Checkers::diff(int fin, int naught)
{
    return fin - naught;
}

void Checkers::dumpGame()
{
    if (m_numRows > 0 && m_numColumns > 0 && m_game != nullptr)
    {
        cout << "  ";
        for (int k = 0; k < m_numColumns; k++)
        {
            cout << k << " ";
        }
        cout << endl;
        for (int i = 1; i <= m_numRows; i++)
        {
            cout << i - 1 << " ";
            for (int j = 1; j <= m_numColumns; j++)
            {
                cout << m_game[i - 1][j - 1];
            }
            cout << endl;
        }
        cout << endl;
    }
}