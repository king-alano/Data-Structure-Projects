// UMBC - CMSC 341 - Spring 2023 - Proj1
/*
  auth: AJ Boyd (aboyd3)
  date: 2/22/23
  desc: full implemenation of the FuelSys class
*/
#include "fuel.h"

// constructor intitalizes m_current to nullptr
FuelSys::FuelSys()
{
    m_current = nullptr;
}

// destructor destroys each Tank and Pump in the FuelSys
FuelSys::~FuelSys()
{
    clear();
}

// adds a new Tank to cicular linked list (after m_current)
bool FuelSys::addTank(int tankID, int tankCap, int tankFuel = 0)
{
    // validate tankID is unique
    if (exist(tankID))
        return false;

    // make sure tankCap is abouve MINCAP
    if (tankCap < MINCAP)
        return false;

    // make sure tankFuel is at least 0, but not bigger than capacity
    if (tankFuel < 0 || tankFuel > tankCap)
        return false;

    // make sure tankID is at least 0
    if (tankID < 0)
        return false;

    // if Tank is valid, construct a new Tank obj and add it after m_current
    Tank *newTank = new Tank(tankID, tankCap, tankFuel);

    // if FuelSys is empty prior to insertion
    if (m_current == nullptr)
    {
        m_current = newTank;           // m_current becomes newTank
        m_current->m_next = m_current; // and it points back to itself
        return true;                   // return true for successful insertion!
    }
    else
    {
        newTank->m_next = m_current->m_next; // newTank points to the "front" of the list
        m_current->m_next = newTank;         // m_current points to the newTank
        m_current = newTank;                 // newTank becomes the new m_current
        return true;                         // return true for successful insertion!
    }
}

// removes a Tank with a specified tankID and deletes its data (including its Pumps)
bool FuelSys::removeTank(int tankID)
{
    // if FuelSys is empty, return false
    if (m_current == nullptr)
        return false;

    if (exist(tankID))
    { // the Tank To Remove (TTR) must exist in FuelSys
        Tank *prev = m_current;
        Tank *t = m_current->m_next;

        while (t != m_current && t != nullptr)
        { // iterate thru FuelSys until TTR is found
            if (t->m_tankID == tankID)
            {
                Tank *bridge = t->m_next; // when TTR is found, create a bridge Tank
                if (t->m_pumps)
                { // if TTR has Pumps, delete those
                    clearPumps(t);
                }
                prev->m_next = bridge; // set previous Tank to bridge Tank
                delete t;              // delete t
                return true;           // return true for successful removal!
            }
            prev = t;
            t = t->m_next;
        }

        // check if they wanna remove m_current and follow the same algorithm
        if (m_current->m_tankID == tankID)
        {
            Tank *bridge = m_current->m_next;
            if (m_current->m_pumps)
            {
                clearPumps(t);
            }
            prev->m_next = bridge;
            if (m_current != prev)
            { // if there are more Tanks in FuelSys, change m_current to prev
                m_current = prev;
            }
            else
            {
                m_current = nullptr; // if m_current is the only Tank, set it to nullptr
            }
            delete t;    // delete the memory address of the old m_current
            return true; // return true for successful deleteion
        }
    }
    return false; // if a Tank with that ID DNE in FuelSys, return false
}

// helper function deletes all the Pumps in a given Tank
void FuelSys::clearPumps(Tank *t)
{
    // Tank *t = m_current;
    Pump *curr = t->m_pumps;

    while (curr != nullptr)
    {
        Pump *next = curr->m_next;
        delete curr;
        curr = next;
    }
    t->m_pumps = nullptr;
}

// function finds a Tank and moves it to after m_current
bool FuelSys::findTank(int tankID)
{
    if (m_current)
    {
        Tank *t = m_current->m_next;
        Tank *prev = m_current;
        while (t != m_current)
        {
            if (t->m_tankID == tankID)
            {                     // once Tank To Find (TTF) has been found
                m_current = prev; // the Tank before the TTF becomes the new m_current
                return true;      // return true for successful movement!
            }
            prev = t;
            t = t->m_next;
        }

        // same thing for m_current
        if (m_current->m_tankID == tankID)
        {
            m_current = prev;
            return true;
        }

        return false; // if TTF DNE in FuelSys, return false
    }
    return false; // if FuelSys is empty, return false
}

// adds a Pump with a given pumpID and target ID at a given Tank
bool FuelSys::addPump(int tankID, int pumpID, int targetTank)
{
    if (pumpID < 0)
        return false; // if pumpID is less than 0, return false;

    if (tankID == targetTank)
        return false; // a Pump's source Tank and target Tank cannot be the same

    // both tankID and targetTank must exist
    if (exist(tankID) && exist(targetTank))
    {
        Tank *t = m_current->m_next; // iterate through circularly linked list
        while (t != m_current)
        {
            if (t->m_tankID == tankID)
            { // if current Tank matches our ID
                if (exist(pumpID, t))
                    return false; // if a Pump with that ID already exists in this Tank, return false

                Pump *p = new Pump(pumpID, targetTank); // dynamically create a new Pump obj and add it to end
                if (t->m_pumps == nullptr)
                { // if this is the Tank's first pump
                    t->m_pumps = p;
                }
                else
                { // else, add it to the front of the singly linked list
                    p->m_next = t->m_pumps;
                    t->m_pumps = p; // add to front
                }
                return true; // return true for successful addition!
            }
            t = t->m_next;
        }

        // if wanting to add to m_current
        if (m_current->m_tankID == tankID)
        {
            if (exist(pumpID, t))
                return false;

            Pump *p = new Pump(pumpID, targetTank);
            if (t->m_pumps == nullptr)
            {
                t->m_pumps = p;
            }
            else
            {
                p->m_next = t->m_pumps;
                t->m_pumps = p;
            }
            return true;
        }
        // return false; //false to avoid end of control warning
    }
    return false; // false if not found
}

// test to make sure this works on m_current
// removes a specified Pump in a specified Tank
bool FuelSys::removePump(int tankID, int pumpID)
{
    if (m_current)
    {
        if (exist(tankID) == false) // the Tank must exist
            return false;
        if (exist(pumpID, find(tankID)) == false) // the Pump must exist
            return false;

        Tank *t = m_current->m_next; // iterate through circularly linked list
        while (t != m_current)
        {
            if (t->m_tankID == tankID)
            {
                if (t->m_pumps)
                {
                    Pump *prev = t->m_pumps;
                    Pump *curr = prev->m_next;

                    if (prev->m_pumpID == pumpID)
                    { // if head of Pump LL is to be removed
                        t->m_pumps = curr;
                        delete prev;
                        return true; // return true for successful deletion
                    }
                    else
                    {
                        while (curr != nullptr)
                        { // iterate thru m_pumps
                            if (curr->m_pumpID)
                            {
                                Pump *bridge = curr->m_next;
                                prev->m_next = bridge;
                                delete curr; // delete curr
                                return true; // return true for successful deleteion!
                            }
                            prev = curr;
                            curr = curr->m_next; // move prev and curr
                        }
                        return false; // if Pump was not found, return false
                    }
                }
                else
                {
                    return false; // if Pump list is empty, return false
                }
            }
            t = t->m_next;
        }

        // do same thing for m_current
        if (m_current->m_tankID == tankID)
        {
            Pump *prev = m_current->m_pumps;
            Pump *curr = prev->m_next;

            if (prev->m_pumpID == pumpID)
            { // if head of Pump LL is to be removed
                m_current->m_pumps = curr;
                delete prev;
                return true; // return true for successful deletion
            }
            else
            {
                while (curr != nullptr)
                { // iterate thru m_pumps
                    if (curr->m_pumpID)
                    {
                        Pump *bridge = curr->m_next;
                        prev->m_next = bridge;
                        delete curr; // delete curr
                        return true; // return true for successful deleteion
                    }
                    prev = curr;
                    curr = curr->m_next; // move prev and curr
                }
                return false; // if Pump was not found, return false
            }
        }
        else
        {
            return false; // if Tank was not found, return false
        }
    }
    return false; // if FuelSys is empty, return false
}

// adds up and returns the total amount of fuel across all Tanks
int FuelSys::totalFuel() const
{
    int total = 0; // intialize total variable to 0
    if (m_current)
    {
        Tank *t = m_current->m_next;
        while (t != m_current && t != nullptr)
        {                           // iterate thru circularly linked list--that t != nullptr is important!!
            total += t->m_tankFuel; // add to total variable
            t = t->m_next;
        }
        total += m_current->m_tankFuel; // add m_current's fuel to tota
    }
    return total; // return final output
}

// still needs work: add a helper function that returns a Tank at a given tankID
bool FuelSys::drain(int tankID, int pumpID, int fuel)
{
    if (m_current && exist(tankID) && exist(pumpID, find(tankID)))
    { // m_current, tankID, and pumpID must exist
        if (fuel < 0)
            return false; // fuel cannot be a negative number

        Tank *t = m_current->m_next;
        while (t != m_current && t != nullptr)
        {
            if (t->m_tankID == tankID)
            { // once source Tank has been found, find target Tank (thru pumpID)
                Pump *p = t->m_pumps;
                while (p != nullptr)
                {
                    if (p->m_pumpID == pumpID)
                    {
                        if (exist(p->m_target))
                        { // the target Tank must exist
                            Tank *target = find(p->m_target);
                            int space = target->m_tankCapacity - target->m_tankFuel;
                            if (fuel > t->m_tankFuel)
                            { // if amount of fuel to be drained, exceeds the source Tank's amount, only drain what's left
                                fuel = t->m_tankFuel;
                            }
                            if (fuel > space)
                            { // if amount of fuel exceeds the space in target Tank, fill target Tank to capacity
                                fuel = space;
                            }
                            target->m_tankFuel += fuel; // adds fuel to target Tank
                            t->m_tankFuel -= fuel;      // subtract the amount of transfered fuel from source Tank
                            return true;
                        }
                        else
                        {
                            return false; // if target Tank DNE, return false
                        }
                    }
                    p = p->m_next;
                }
                return false; // if pump not found, return false
            }
            t = t->m_next;
        }
        // do same thing for m_current
        if (m_current->m_tankID == tankID)
        {
            Pump *p = t->m_pumps;
            while (p != nullptr)
            {
                if (p->m_pumpID == pumpID)
                {
                    if (exist(p->m_target))
                    { // the target Tank must exist
                        Tank *target = find(p->m_target);
                        int space = target->m_tankCapacity - target->m_tankFuel;

                        if (fuel > t->m_tankFuel)
                        {
                            fuel = t->m_tankFuel;
                        }
                        if (fuel >= space)
                        {
                            fuel = space;
                        }
                        target->m_tankFuel += fuel;
                        t->m_tankFuel -= fuel;
                        return true; // return true for successful drain
                    }
                    p = p->m_next;
                }
                return false; // if pump not found, return false
            }
        }
    }
    return false; // if FuelSys is empty or tankID DNE, return false
}

// fills the Tank with a given ID with a given amount of fuel
// if the amount of fuel exceeds the empty space in the Tank, fill to capacity
// if Tank DNE, return false
bool FuelSys::fill(int tankID, int fuel)
{
    if (m_current && exist(tankID))
    {
        if (fuel < 0)
            return false; // fuel cannot be a negative number

        Tank *t = m_current->m_next;
        while (t != m_current && t != nullptr)
        {
            if (t->m_tankID == tankID)
            {
                int space = t->m_tankCapacity - t->m_tankFuel; // calculates the amount of empty space in Tank
                if (fuel >= space)
                { // if fuel exceeds or matches that amount, fill to capacity
                    t->m_tankFuel = t->m_tankCapacity;
                }
                else
                { // else, just add that much fuel to the Tank
                    t->m_tankFuel += fuel;
                }
                return true; // retdurn true for successful fueling!
            }
            t = t->m_next;
        }
        // do same thing for m_current
        if (m_current->m_tankID == tankID)
        {
            int space = t->m_tankCapacity - t->m_tankFuel;
            if (fuel >= space)
            {
                t->m_tankFuel = t->m_tankCapacity;
            }
            else
            {
                t->m_tankFuel += fuel;
            }
            return true;
        }
    }
    return false; // if FuelSys is empty or Tank DNE, return false
}

// overloaded assignment operator provides a deep copy of data
// still needs work: although the Tank data is being copied correctly, Pumps are not for some reason
const FuelSys &FuelSys::operator=(const FuelSys &rhs)
{
    if (&rhs == this)
    {
        return *this;
    }
    else
    {
        clear(); // clear possible existing data

        if (rhs.m_current)
        {
            Tank *copyTank = rhs.m_current->m_next;

            // constantly loop thru rhs and copy data
            while (copyTank != rhs.m_current)
            {
                addTank(copyTank->m_tankID, copyTank->m_tankCapacity, copyTank->m_tankFuel);

                if (copyTank->m_pumps != nullptr)
                {
                    Tank *t = m_current;
                    Pump *copyPump = copyTank->m_pumps;
                    Pump *prev = nullptr;

                    while (copyPump != nullptr)
                    {
                        // ngl this gave me some issue, but you can't use addPump because the Tank's might not exist in lhs before insertion resulting in a failure
                        // so we gotta do this by hand

                        if (prev)
                        { // add to end of t's Pumps LL
                            prev->m_next = new Pump(copyPump->m_pumpID, copyPump->m_target);
                            prev = prev->m_next; // move prev to end of Pump list
                        }
                        else
                        { // add to head of t's Pumps LL
                            prev = new Pump(copyPump->m_pumpID, copyPump->m_target);
                            t->m_pumps = prev;
                        }
                        copyPump = copyPump->m_next;
                    }
                }

                copyTank = copyTank->m_next;
            }

            // add m_current
            addTank(rhs.m_current->m_tankID, rhs.m_current->m_tankCapacity, rhs.m_current->m_tankFuel);

            // add m_current's Pumps
            if (rhs.m_current->m_pumps)
            {
                Pump *copyPump = copyTank->m_pumps;
                Pump *prev = nullptr;
                Tank *t = m_current;
                while (copyPump != nullptr)
                {
                    // ngl this gave me some issue, but you can't use addPump because the Tank's might not exist in lhs before insertion resulting in a failure
                    // so we gotta do this by hand

                    if (prev)
                    { // add to end of t's Pumps LL
                        prev->m_next = new Pump(copyPump->m_pumpID, copyPump->m_target);
                    }
                    else
                    { // add to head of t's Pumps LL
                        prev = new Pump(copyPump->m_pumpID, copyPump->m_target);
                        t->m_pumps = prev;
                    }
                    copyPump = copyPump->m_next;
                }
            }
        }

        return *this; // final return
    }
}

// starter function that displays the FuelSys
void FuelSys::dumpSys() const
{
    if (m_current != nullptr)
    {
        Tank *tempTank = m_current->m_next; // we start at front item
        // we continue visting nodes until we reach the cursor
        while (m_current != nullptr && tempTank != m_current)
        {
            cout << "Tank " << tempTank->m_tankID << "(" << tempTank->m_tankFuel << " kg)" << endl;
            // now dump the targets for all pumps in this tank
            // we need to traverse the list of pumps
            dumpPumps(tempTank->m_pumps);
            tempTank = tempTank->m_next;
        }
        // at the end we visit the cursor (current)
        // this also covers the case that there is only one item
        cout << "Tank " << m_current->m_tankID << "(" << m_current->m_tankFuel << " kg)" << endl;
        dumpPumps(tempTank->m_pumps);
        cout << "The current tank is " << m_current->m_tankID << endl;
    }
    else
        cout << "There is no tank in the system!\n\n";
}

void FuelSys::dumpPumps(Pump *pumps) const
{
    // this traverses the linked list to the end
    Pump *tempPump = pumps;
    while (tempPump != nullptr)
    {
        cout << " => pump " << tempPump->m_pumpID << "(To tank " << tempPump->m_target << ")" << endl;
        tempPump = tempPump->m_next;
    }
}

// helper function returns if a Tank w/ a specified ID exists in FuelSys
bool FuelSys::exist(int tankID)
{
    if (m_current)
    {
        if (tankID == m_current->m_tankID)
        {
            return true;
        }

        Tank *t = m_current->m_next;
        while (t != m_current)
        {
            if (t->m_tankID == tankID)
            {
                return true;
            }
            t = t->m_next;
        }
        return false;
    }
    return false;
}

// overloaded helperfunction returns if a Pump w/ a specified ID exists in a given Tank's Pump list
bool FuelSys::exist(int pumpID, Tank *t)
{
    if (m_current && t)
    { // both m_current and the Tank must exist
        Pump *curr = t->m_pumps;
        while (curr != nullptr)
        {
            if (curr->m_pumpID == pumpID)
                return true; // if a Pump with a matching ID is found, return true
            curr = curr->m_next;
        }
    }
    return false;
}

// helper function that returns the address of a Tank @ a given tankID
Tank *FuelSys::find(int tankID)
{
    if (m_current)
    {
        Tank *t = m_current->m_next;
        while (t != m_current)
        {
            if (t->m_tankID == tankID)
                return t;
            t = t->m_next;
        }
        if (m_current->m_tankID == tankID)
            return m_current;
    }

    return nullptr;
}

// overloaded helper function that returns the memory address of a Pump in a given Tank with a given pumpID
Pump *FuelSys::find(int tankID, int pumpID)
{
    if (m_current)
    {
        Tank *t = m_current->m_next;
        while (t != m_current)
        {
            if (t->m_tankID == tankID)
            {
                Pump *p = t->m_pumps;
                while (p != nullptr)
                {
                    if (p->m_pumpID == pumpID)
                    {
                        return p;
                    }
                    p = p->m_next;
                }
            }
            t = t->m_next;
        }

        if (m_current->m_tankID == tankID)
        {
            Pump *p = t->m_pumps;
            while (p != nullptr)
            {
                if (p->m_pumpID == pumpID)
                {
                    return p;
                }
                p = p->m_next;
            }
        }
    }

    return nullptr;
}

// helper function that deletes all data
void FuelSys::clear()
{
    if (m_current)
    {
        Tank *t = m_current->m_next;

        // iterate thru circularly linked list and delete each Tank
        while (t != m_current)
        {
            Tank *curr = t;
            if (curr->m_pumps)
            { // if current Tank has Pumps in it, ya gotta delete them too
                clearPumps(curr);
            }
            t = t->m_next;
            delete curr;
        }
        // delete m_current--by the end of the loop, t will be m_current
        if (m_current->m_pumps)
        {
            clearPumps(m_current);
        }
        delete t;
        m_current = nullptr;
    }
}
