#ifndef FUEL_H
#define FUEL_H
#include <iostream>
using namespace std;
// default capacity of a tank in kg
const int MINCAP = 2000;
const int DEFCAP = 5000;
class Grader;  // this class is for grading purposes, no need to do anything
class Tester;  // this is your tester class, you add your test functions in this class
class Pump;    // forward declaration
class FuelSys; // forward declaration
class Tank
{
public:
    friend class Tester;
    friend class Grader;
    friend class FuelSys;
    Tank();
    Tank(int ID, int tankCap, int tankFuel = 0,
         Pump *pumpList = nullptr, Tank *nextTank = nullptr)
    {
        m_tankID = ID;
        m_tankCapacity = tankCap;
        m_tankFuel = tankFuel;
        m_pumps = pumpList;
        m_next = nextTank;
    }

private:
    int m_tankID;
    int m_tankCapacity; // maximum capacity of the tank
    int m_tankFuel;     // current amount of fuel in the tank
    Pump *m_pumps;
    Tank *m_next;
};
class Pump
{
public:
    friend class Tester;
    friend class Grader;
    friend class FuelSys;
    Pump();
    Pump(int ID, int target, Pump *nextPump = nullptr)
    {
        m_pumpID = ID;
        m_target = target;
        m_next = nextPump;
    }

private:
    int m_pumpID;
    int m_target; // ID of the target tank for transfer
    Pump *m_next;
};
class FuelSys
{
public:
    friend class Tester;
    friend class Grader;
    FuelSys();
    ~FuelSys();
    // overloaded assignment operator
    const FuelSys &operator=(const FuelSys &rhs);
    // add to the tank list
    bool addTank(int tankID, int capacity, int tankFuel);
    // remove from the tank list
    bool removeTank(int tankID);
    // add pump to the pump list of the tank
    bool addPump(int tankID, int pumpID, int targetTank);
    // remove from the pump list of the tank
    bool removePump(int tankID, int pumpID);
    // fill the tank with fuel
    bool fill(int tankID, int fuel);
    // transfer fuel from the tank through the pump
    bool drain(int tankID, int pumpID, int fuel);
    // if the ID is found, it must become the next of current
    bool findTank(int tankID);
    // return the sum of fuel in all tanks
    int totalFuel() const;
    // the dump function is provided to facilitate debugging
    // using dump function for test cases is not accepted
    void dumpSys() const;
    void dumpPumps(Pump *pumps) const;

private:
    Tank *m_current;

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************

    // deletes all the Pumps in a given Tank
    void clearPumps(Tank *); // deletes the singly linked list in a given Tank
    bool exist(int);         // returns if a Tank w/ a specified ID exists in FuelSys
    bool exist(int, Tank *); // returns if a Pump w/ a specified ID exists in a given Tank
    Tank *find(int);         // returns the address of a Tank @ a given tankID
    Pump *find(int, int);    // returns the address of a Pump in a given Tank with a given pumpID
    void clear();            // deletes all data
};
#endif
