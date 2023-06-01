#include "stdafx.h"
#include "cMyTimer.h"
#include "windows.h"
#include "process.h"

cMyTimer::cMyTimer()
{

}

cMyTimer::~cMyTimer()
{

}

void CheckTimerRun(void* p)
{
    cMyTimer* pTimer = (cMyTimer*)p;
    if (pTimer == NULL)
    {
        _endthread();
        return;
    }
    while (1)
    {

        pTimer->CheckAddTimer();
        //Delete the timer that needs to be deleted
        pTimer->CheckDelTimer();

        //Check if there is a timer to run

        if (!pTimer->TimerRun())
        {
            _endthread();
        }

        Sleep(20);
    }
}

//add timer
void cMyTimer::AddTimer(unsigned timeId, unsigned timeElapse, int param, char* p)
{
    if (timeElapse == 0)
    {
        return;
    }
    stTimer stTimerTemp;
    stTimerTemp.bDel = false;
    stTimerTemp.id = timeId;
    stTimerTemp.iParam = param;
    stTimerTemp.timeElapse = timeElapse;
    stTimerTemp.timeLastRun = GetTickCount();//Get the time from booting, in milliseconds
    if (p != NULL)
    {
        stTimerTemp.strParam = p;
    }
    m_tempList.push_back(stTimerTemp);
    //m_timerList.push_back(stTimerTemp);
    if (m_tempList.size() == 1)
    {
        //Indicates that this is the first timer
        _beginthread(CheckTimerRun, 0, this);
    }
}

//delete timer
void cMyTimer::DeleteTimer(unsigned id)
{
    for (itTimerLisst it = m_timerList.begin(); it != m_timerList.end(); ++it)
    {
        if (it->id == id)
        {
            it->bDel = true;
            return;
        }
    }
}

void cMyTimer::CheckDelTimer()
{
    for (itTimerLisst it = m_timerList.begin(); it != m_timerList.end();)
    {
        if (it->bDel)
        {
            m_timerList.erase(it);
            it = m_timerList.begin();
            continue;
        }
        ++it;
    }
}

bool cMyTimer::TimerRun()
{
    if (m_timerList.size() == 0)
    {
        return true;
    }
    //Compare the last running time of the timer with the current time to see if timeElapse has elapsed
    unsigned timeNow = GetTickCount();
    for (itTimerLisst it = m_timerList.begin(); it != m_timerList.end(); ++it)
    {
        if (timeNow - it->timeLastRun >= it->timeElapse)
        {
            it->timeLastRun = timeNow;
            int newTimeElapse = OnTimer(it->id, it->iParam, it->strParam);

            if (OnTimer(it->id, it->iParam, it->strParam) == 0)
            {
                it->bDel = true;
            }
        }
    }
    return true;
}

void cMyTimer::CheckAddTimer()
{
    for (itTimerLisst it = m_tempList.begin(); it != m_tempList.end(); ++it)
    {
        m_timerList.push_back(*it);
    }
    m_tempList.clear();
}