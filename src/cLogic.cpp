#include "stdafx.h"
#include "cLogic.h"

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#pragma comment(lib,"Msimg32.lib")

cLogic::cLogic()
{
    m_hWnd = NULL;


    for (int i = 0; i < MAX_ATTACKPORT; i++)
    {
        wchar_t fileName[100];
        wsprintf(fileName, L"attackport%d.bmp", i + 1);
        m_hAttackPort[i] = (HBITMAP)::LoadImage(nullptr, fileName, IMAGE_BITMAP, 77, 71, LR_LOADFROMFILE);
    }
    DeleteObject(m_hAttackPort);
}
cLogic::~cLogic()
{

}

void cLogic::Begin(HWND hWnd)
{
    m_hWnd = hWnd;
    m_nX = 280;								//Defines the initial X axis of the hero plane
    m_nY = 500;
    m_AiportDir = Dir_Non;
    m_bDown = false;

    //±³¾°ÒôÀÖ
    PlaySound(L"background.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP);

    AddTimer(DRAW_TIMER, 50);				//drawing time

    AddTimer(PLAY_TIMER, 50);				//player moves

    AddTimer(FIRE_TIMER, 100);				//production of shells

    AddTimer(ATTACKPORT_TIMER, 1000);		//enemy spawn

    AddTimer(CHECK_TIMER, 50);				//Check if it has been attacked
}

int cLogic::OnTimer(int id, int nParam, string stParam)
{
    switch (id)
    {
    case DRAW_TIMER:
        DrawAll();				//all things draw here
        break;
    case PLAY_TIMER:
        Move();					//All Games Move
        break;
    case FIRE_TIMER:
        Fire();					//production of shells
        break;
    case ATTACKPORT_TIMER:
        RandAttackPort();		//Randomly spawn planes
    case CHECK_TIMER:
        AttackCheck();			//Check if you have been attacked

        break;
    default:
        break;
    }
    return 1;
}

//All Games Move
void cLogic::Move()
{
    PlayMove();				//player movement
    FireMove();				//movement of shells
    AttackPortMove();		//movement of enemy aircraft
}


//movement of enemy aircraft
void cLogic::AttackPortMove()
{

    for (itListObject itacctackport = m_ListAirportObject.begin(); itacctackport != m_ListAirportObject.end(); )
    {
        itacctackport->y = itacctackport->y + ATTACKPORT_MOVE_SPEED;
        if (itacctackport->y >= 590)
        {
            itacctackport = m_ListAirportObject.erase(itacctackport);
            continue;
        }
        itacctackport++;

    }
}
//player movement
void cLogic::PlayMove()
{
    switch (m_AiportDir)
    {
    case Dir_Up:
        m_nY = m_nY - PLAY_MOVE_SPEED;
        if (m_nY < 10)
            m_nY = 10;
        if (m_nY)
            break;
    case Dir_Down:
        m_nY = m_nY + PLAY_MOVE_SPEED;
        if (m_nY > 500)
            m_nY = 500;
        break;
    case Dir_Right:
        m_nX = m_nX + PLAY_MOVE_SPEED;
        if (m_nX > 500)
            m_nX = 500;
        break;
    case Dir_Left:
        m_nX = m_nX - PLAY_MOVE_SPEED;
        if (m_nX < 0)
            m_nX = 0;
        break;
    default:
        break;
    }
}
//shell movement
void cLogic::FireMove()
{
    for (itListObject it = m_ListFireObject.begin(); it != m_ListFireObject.end(); )
    {
        it->y = it->y - FIRE_MOVE_SPEED;
        if (it->y <= -27)
        {
            it = m_ListFireObject.erase(it);
            continue;
        }
        it++;
    }
}

void cLogic::KeyDown(WPARAM wParam)
{
    if (m_bDown)
    {
        return;
    }
    m_bDown = true;
    switch (wParam)
    {
    case VK_UP:
        m_AiportDir = Dir_Up;
        break;
    case VK_DOWN:
        m_AiportDir = Dir_Down;
        break;
    case VK_RIGHT:
        m_AiportDir = Dir_Right;
        break;
    case VK_LEFT:
        m_AiportDir = Dir_Left;
        break;
    default:
        break;
    }
}

void cLogic::KeyUp(WPARAM wParam)
{
    m_bDown = false;
    switch (wParam)
    {
    case VK_UP:
    case VK_DOWN:
    case VK_RIGHT:
    case VK_LEFT:
        m_AiportDir = Dir_Non;
        break;
    default:
        break;
    }
}


void cLogic::Fire()
{
    stObject FireObject;
    FireObject.y = m_nY - 27;
    FireObject.x = m_nX + (77 - 10) / 2;
    m_ListFireObject.push_back(FireObject);
}


void cLogic::RandAttackPort()
{
    int randType = 0;
    srand(GetTickCount());
    randType = rand() % 4 + 1;
    CreateAttackPort(randType);
    //Sleep(20);
}


void cLogic::CreateAttackPort(int type)
{
    srand(GetTickCount());
    int randValue = rand() % 4 + 1;
    switch (type)
    {
    case 1:
    {
        int x = randValue % 600;
        for (int i = 0; i < 4; i++)
        {
            stObject stAttackPort;
            stAttackPort.attackPortHpVaule = 150;
            stAttackPort.x = x;
            stAttackPort.y = -77 - i * 77 - i * 25;
            stAttackPort.attackType = type;
            m_ListAirportObject.push_back(stAttackPort);
        }
        break;
    }
    case 2:
    {
        int x = randValue % 400 + 1;
        for (int i = 0; i < 4; i++)
        {
            stObject stAttackPort;
            stAttackPort.attackPortHpVaule = 200;
            stAttackPort.x = x + (i % 2) * 100;
            stAttackPort.y = -77 - i * 77 - i * 25;
            stAttackPort.attackType = type;
            m_ListAirportObject.push_back(stAttackPort);
        }
        break;
    }
    case 3:
    {
        int x = 40;
        for (int i = 0; i < 4; i++)
        {
            stObject stAttackPort;
            stAttackPort.attackPortHpVaule = 140;
            stAttackPort.x = x + i * 100;
            stAttackPort.y = -77 - i * 77 - i * 25;
            stAttackPort.attackType = type;
            m_ListAirportObject.push_back(stAttackPort);
        }
        break;
    }
    case 4:
    {
        int x = 40;
        for (int i = 0; i < 4; i++)
        {
            stObject stAttackPort;
            stAttackPort.attackPortHpVaule = 200;
            stAttackPort.x = x + i * 100;
            stAttackPort.y = -77;
            stAttackPort.attackType = type;
            m_ListAirportObject.push_back(stAttackPort);
        }
        break;
    }
    default:
        break;
    }
}


void cLogic::DrawAll()
{
    HDC hdc = GetWindowDC(m_hWnd);
    if (hdc == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, L"fail", L"fail", MB_OK);
    }
    HDC dcMem = CreateCompatibleDC(NULL);
    HBITMAP hMap = CreateCompatibleBitmap(hdc, 600, 590);
    SelectObject(dcMem, hMap);
    DrawBackGround(dcMem);
    DrawHeroPlay(dcMem);
    DrawAttackPort(dcMem);
    DrawFire(dcMem);
    //DrawTest(dcMem);


    BitBlt(hdc, 10, 10, 600, 590, dcMem, 0, 0, SRCCOPY);
    DeleteObject(dcMem);
    ReleaseDC(m_hWnd, hdc);
    DeleteObject(hMap);
}


void cLogic::DrawBackGround(HDC hdc)
{
    HDC dcTemp;
    dcTemp = CreateCompatibleDC(hdc);
    HBITMAP hBackGround = (HBITMAP)::LoadImage(NULL, L"BG.bmp", IMAGE_BITMAP, 600, 590, LR_LOADFROMFILE);
    SelectObject(dcTemp, hBackGround);
    BitBlt(hdc, 10, 10, 600, 590, dcTemp, 0, 0, SRCCOPY);
    DeleteObject(dcTemp);
    DeleteObject(hBackGround);
}


void cLogic::DrawHeroPlay(HDC hdc)
{
    HDC dcTemp;
    dcTemp = CreateCompatibleDC(hdc);
    HBITMAP hHeroPlay = (HBITMAP)::LoadImage(nullptr, L"airport.bmp", IMAGE_BITMAP, 71, 77, LR_LOADFROMFILE);
    SelectObject(dcTemp, hHeroPlay);
    TransparentBlt(hdc, m_nX, m_nY, 71, 77, dcTemp, 0, 0, 71, 77, RGB(0, 0, 0));
    DeleteObject(dcTemp);
    DeleteObject(hHeroPlay);
}


void cLogic::DrawFire(HDC hdc)
{
    HDC dcTemp;
    dcTemp = CreateCompatibleDC(hdc);
    HBITMAP hFire = (HBITMAP)::LoadImage(nullptr, L"fire.bmp", IMAGE_BITMAP, 10, 27, LR_LOADFROMFILE);
    SelectObject(dcTemp, hFire);
    for (itListObject it = m_ListFireObject.begin(); it != m_ListFireObject.end(); ++it)
    {
        TransparentBlt(hdc, it->x, it->y, 10, 27, dcTemp, 0, 0, 10, 27, RGB(0, 0, 0));
    }
    DeleteObject(dcTemp);
    DeleteObject(hFire);
}


void cLogic::DrawAttackPort(HDC hdc)
{
    HDC dcTemp;
    dcTemp = CreateCompatibleDC(hdc);
    for (itListObject it = m_ListAirportObject.begin(); it != m_ListAirportObject.end(); ++it)
    {
        SelectObject(dcTemp, m_hAttackPort[it->attackType - 1]);
        TransparentBlt(hdc, it->x, it->y, 77, 71, dcTemp, 0, 0, 77, 71, RGB(0, 0, 0));
    }
    DeleteObject(dcTemp);
}


//The drawing of the test does not need to be run in the program
void cLogic::DrawTest(HDC hdc)
{
    HDC dcTemp;
    dcTemp = CreateCompatibleDC(hdc);
    //HBITMAP hTest = (HBITMAP)::LoadImage(NULL, L"attackport4.bmp", IMAGE_BITMAP, 77, 71, LR_LOADFROMFILE);
    SelectObject(dcTemp, m_hAttackPort[3]);
    //BitBlt(hdc, 200, 300, 70, 77, dcTemp, 0, 0, SRCPAINT);
    TransparentBlt(hdc, 200, 300, 77, 71, dcTemp, 0, 0, 77, 71, RGB(0, 0, 0));
    DeleteObject(dcTemp);
}


void cLogic::AttackCheck()
{
    for (itListObject itFire = m_ListFireObject.begin(); itFire != m_ListFireObject.end();)
    {
        bool bAttackJude = false;
        for (itListObject iAttackport = m_ListAirportObject.begin(); iAttackport != m_ListAirportObject.end(); iAttackport++)
        {
            if (iAttackport->y - itFire->y >= -71 + 10)
            {
                if (itFire->x - iAttackport->x <= 80 - 10 && itFire->x - iAttackport->x >= -10 + 10)
                {
                    bAttackJude = true;
                    iAttackport->attackPortHpVaule -= HERO_AIPORT_ATTACK;
                    if (iAttackport->attackPortHpVaule <= 0)
                    {
                        iAttackport = m_ListAirportObject.erase(iAttackport);
                        break;
                    }
                }
            }

        }
        if (bAttackJude == true)
        {
            itFire = m_ListFireObject.erase(itFire);
            continue;
        }
        itFire++;
    }
}
