#pragma once
#include "string"
#include "list"
using namespace std;

struct stTimer
{
    unsigned id;
    unsigned timeElapse;	//Time interval between timer runs
    unsigned timeLastRun;	//The time the timer last ran
    int iParam;				//Reserved parameters
    string strParam;		//Reserved parameters
    bool bDel;				//whether to be deleted
    stTimer()
    {
        timeElapse = 0;
        timeLastRun = 0;
        id = 0;
        iParam = 0;
        bDel = false;
    }
};

typedef list<stTimer> TimerList;
typedef list<stTimer>::iterator itTimerLisst;

class cMyTimer
{
public:
    cMyTimer();
    ~cMyTimer();
    //add timer
    void AddTimer(unsigned timeId, unsigned timeElapse, int param = 0, char* p = NULL);

    //delete timer
    void DeleteTimer(unsigned id);

    //Timer logic processing
    //The returned value is the new interval time of the timer
    virtual int OnTimer(int id, int iParam, string str) = 0;

    //Detect whether there is a timer that needs to be deleted
    void CheckDelTimer();

    //Detect and run timer
    bool TimerRun();

    //Check if a new list has been added
    void CheckAddTimer();


private:
    TimerList m_timerList;	//save all timers

    TimerList m_tempList;	//cache list
};