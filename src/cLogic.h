#pragma once
#include "CMyTimer.h"
#include <list>

#define MAX_ATTACKPORT 4		//maximum enemy type

#define DRAW_TIMER 1			//The ID of the paint timerID
#define PLAY_TIMER 2			//Timer ID for player movement		
#define FIRE_TIMER 3			//The timer ID of the shell movement
#define ATTACKPORT_TIMER 5		//Enemy Spawn Timer ID
#define CHECK_TIMER 6


#define PLAY_MOVE_SPEED 10		//The speed at which the player plane moves
#define FIRE_MOVE_SPEED 15		//The velocity of the projectile's flight movement
#define ATTACKPORT_MOVE_SPEED 15//The speed at which the enemy flies and moves

#define HERO_AIPORT_ATTACK 25

enum AirportDirection
{
    Dir_Up,
    Dir_Down,
    Dir_Right,
    Dir_Left,
    Dir_Non
};

struct stObject					//shells, bullets, enemy
{
    int x;
    int y;
    int attackVaule;
    int attackPortHpVaule;
    int attackType;
};

typedef list<stObject>ListObject;
typedef list<stObject>::iterator itListObject;
class cLogic :cMyTimer
{
public:
    cLogic();
    ~cLogic();


    void Begin(HWND hWnd);			//trigger start game

    virtual int OnTimer(int id, int nParam, string str = "");//Handle increased timers

    void DrawAll();

    void DrawBackGround(HDC hdc);

    void DrawHeroPlay(HDC hdc);

    void KeyDown(WPARAM wParam);

    void KeyUp(WPARAM wParam);

    void Move();

    void PlayMove();

    void Fire();

    void FireMove();					//shell movement

    void DrawFire(HDC hdc);				//draw shell

    void DrawAttackPort(HDC hdc);

    void RandAttackPort();				//Randomly generated enemy aircraft

    void CreateAttackPort(int type);	//Create Aircraft Formation

    void AttackPortMove();				//enemy movement

    void DrawTest(HDC hdc);				//test

    void AttackCheck();					//Check if the aircraft is attacked

private:

    int m_nX;			                    //Player X axis direction
    int m_nY;			                    //Player Y axis direction
    HWND m_hWnd;
    AirportDirection m_AiportDir;           //record the direction of the aircraft
    bool m_bDown;			                //Determine if a key is pressed
    ListObject m_ListFireObject;	        //Cannonball List structure
    ListObject m_ListAirportObject;	        //Enemy list structure
    HBITMAP m_hAttackPort[MAX_ATTACKPORT];	//Enemy image handle
};
