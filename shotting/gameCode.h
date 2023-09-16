#pragma once
#include "stdafx.h"
#include "enum.h"
#define _CRT_SECURE_NO_WARNINGS
//--------------------------------------------------------------------
// 스테이지 숫자를 넣으면 스테이지의 데이터들이 넣어짐
//--------------------------------------------------------------------
char buff[200]; //버퍼
char re_data[200];
char* DataInfo(const char* file_path)
{
	FILE* file;

	//읽을 파일 열기
	fopen_s(&file, file_path, "r");
	if (file == NULL)
	{
		printf("파일을 열 수 없습니다.\n");
		return NULL;
	}

	memset(re_data, 0, sizeof(re_data));

	// 파일 내용 읽어오고 저장한다.
	while (fgets(buff, sizeof(buff), file) != NULL)
	{
		strcat_s(re_data, sizeof(re_data), buff);
	}

	// 파일 닫기
	fclose(file);
	return re_data;
}

int stage = 1;
int maxStage = 0;
int monsterCnt = 0;
char pattern[20] = {};

char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

//게임씬
Scene G_scene;
bool escape = false;

// 플레이어 총알
st_BULLET bullet[BULLET_COUNT];

//--------------------------------------------------------------------
// 플레이어 인스턴스 선언, (1인용 이니 하나만 하자)
//--------------------------------------------------------------------
st_PLAYER g_stPlayer;


//--------------------------------------------------------------------
// 몬스터,몬스터 공격 인스턴스 선언
//--------------------------------------------------------------------
st_MONSTER g_stMonster[20];
st_BULLET m_bullet[M_BULLET_COUNT];
int e_index = 0;

//몬스터 움직임 패턴
int coordi_X[4] = {};
int coordi_Y[4] = {};

//--------------------------------------------------------------------
// 게임 시작시 전체 데이터 초기화용.
//--------------------------------------------------------------------
void Initial() 
{
	char* stageInfo = DataInfo("stageinfo.txt");
	char* context = nullptr;
	char* token = strtok_s(stageInfo, ",", &context);
	maxStage = atoi(token);

	if (maxStage < stage)
		return;

	for (int i = 0; i < stage; i++) 
	{
		token = strtok_s(NULL, ",", &context);  //스테이지가 1이라면 Stage1.txt을 가져옴
	}

	char grade[2] = {};
	char patType[10] = {};
	char* stageTxt = DataInfo(token);
	sscanf_s(stageTxt, "%[^,], %s", grade, (unsigned int)sizeof(grade), patType, (unsigned int)sizeof(patType)); // ,단위로 끊어읽어옴

	char* enemyInfo = DataInfo("enemyInfo.txt");
	while (*enemyInfo != '\0') 
	{
		if (*enemyInfo == grade[0]) // 첫 문자가 grade와 일치하는지 확인
		{
			enemyInfo += 2; // 패턴으로 포인터 이동
			sscanf_s(enemyInfo, "%[^\n]", pattern, (unsigned int)sizeof(pattern));  // 일치하는 패턴 문자열 저장
		}
		enemyInfo++;
	}

	/*
	pattern_1
	(22,5) (17,5) (12,5) (17,5)
	pattern_2
	(22,10) (17,5) (12,10) (17,5)
	*/

	// patType에 맞는 데이터를 데이터.txt에서 읽어옴
	char* movePatternInfo = DataInfo("movepatterninfo.txt");
	char* ptr = strstr(movePatternInfo, patType);
	if (ptr != nullptr) 
	{
		ptr += strlen(patType);
		int index = 0;
		while (*ptr != '\0' && index < 4) 
		{
			if (*ptr == '(') 
			{
				sscanf_s(ptr, "(%d, %d)", &coordi_X[index], &coordi_Y[index]);
				index++;
				ptr++;
			}
			else {
				ptr++;
			}
		}
	}

	// 플레이어 데이터 초기화
	g_stPlayer.iHP = 10;
	g_stPlayer.iVisible = true;
	g_stPlayer.iX = 39;
	g_stPlayer.iY = 19;

	// 플레이어 총알 초기화
	for (int i = 0; i < BULLET_COUNT; i++) 
	{
		if (bullet != nullptr) {
			bullet[i].iPower = 3;
			bullet[i].iVisible = false;
			bullet[i].iX = 0;
			bullet[i].iY = 0;
		}
	}

	monsterCnt = 0;

	// 몬스터 데이터 초기화
	for (int i = 0; i < sizeof(pattern); i++)
	{
		g_stMonster[i].iHP = 0;
		g_stMonster[i].iVisible = false;
		g_stMonster[i].iX = 0;
		g_stMonster[i].iY = 0;

		if (pattern[i] == '*')
		{
			++monsterCnt;
			g_stMonster[i].iHP = 1;
			g_stMonster[i].iVisible = true;
			g_stMonster[i].iX = 27 + i;
			g_stMonster[i].iY = 10;
		}
	}

	// 몬스터 총알 초기화
	for (int i = 0; i < M_BULLET_COUNT; i++) 
	{
		if (m_bullet != nullptr) {
			m_bullet[i].iVisible = false;
		}
	}
}

//--------------------------------------------------------------------
// 버퍼의 내용을 화면으로 찍어주는 함수.
//
// 적군,아군,총알 등을 szScreenBuffer 에 넣어주고, 
// 1 프레임이 끝나는 마지막에 본 함수를 호출하여 버퍼 -> 화면 으로 그린다.
//--------------------------------------------------------------------
void Buffer_Flip(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		cs_MoveCursor(0, iCnt);
		printf(szScreenBuffer[iCnt]);
	}
}


//--------------------------------------------------------------------
// 화면 버퍼를 지워주는 함수
//
// 매 프레임 그림을 그리기 직전에 버퍼를 지워 준다. 
// 안그러면 이전 프레임의 잔상이 남으니까
//--------------------------------------------------------------------
void Buffer_Clear(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memset(szScreenBuffer[iCnt], ' ', dfSCREEN_WIDTH);
		szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}

//--------------------------------------------------------------------
// 버퍼의 특정 위치에 원하는 문자를 출력.
//
// 입력 받은 X,Y 좌표에 아스키코드 하나를 출력한다. (버퍼에 그림)
//--------------------------------------------------------------------
void Sprite_Draw(int iX, int iY, char chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
		return;

	szScreenBuffer[iY][iX] = chSprite;
}

//버퍼의 특정 위치에 원하는 문자열을 출력
void String_Draw(int iX, int iY, const char* chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
		return;

	size_t length = strlen(chSprite);
	for (int i = 0; i < length; i++)
	{
		szScreenBuffer[iY][iX + i] = chSprite[i];
	}
}


//--------------------------------------------------------------------
// 키보드입력을 받아서 플레이어를 처리한다.
//
// ESC 를 누를 경우 false 리턴, (종료처리)
//--------------------------------------------------------------------
bool KeyProcess(void)
{
	// 엔터키.
	if (GetAsyncKeyState(VK_RETURN) & 0x8001)
	{
		G_scene = Scene::LOAD;
	}

	// 왼쪽 방향키.
	if (GetAsyncKeyState(VK_LEFT))
	{
		g_stPlayer.iX -= 1;
	}
	// 오른쪽 방향키.
	if (GetAsyncKeyState(VK_RIGHT))
	{
		g_stPlayer.iX += 1;
	}
	// 위쪽 방향키.
	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		g_stPlayer.iY--;
	}
	// 아래쪽 방향키.
	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		g_stPlayer.iY++;
	}

	//-------------------------------------------------------------
	// 플레이어 이동 반경 제한.
	// 게임 화면에서 플레이어가 이동 가능한 구역을 제한한다.
	//-------------------------------------------------------------

	g_stPlayer.iX = max(g_stPlayer.iX, 0);
	g_stPlayer.iX = min(g_stPlayer.iX, 79);
	g_stPlayer.iY = max(g_stPlayer.iY, 0);
	g_stPlayer.iY = min(g_stPlayer.iY, 23);

	if (G_scene == Scene::GAME)
	{
		// 콘트롤 키. (미사일 키)
		if (GetAsyncKeyState(VK_CONTROL))
		{
			for (int i = 0; i < BULLET_COUNT; i++)
			{
				if (bullet != NULL && bullet[i].iVisible == false)
				{
					bullet[i].iX = g_stPlayer.iX;
					bullet[i].iY = g_stPlayer.iY - 1;
					bullet[i].iPower = 3;
					bullet[i].iVisible = true;
					break;
				}
			}
		}
	}

	// ESC 키. (종료)
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		// 종료 방법, exit금지
		escape = true;
	}

	return true;
}

float TickTimer = 0.0f;
int Timecounter = 0;
int LoopCounter = 0;
DWORD StartTime = 0;
DWORD CurrentTime = 0;
DWORD EndTime = 0;
DWORD Time = 0;
float deltaTime = 0;
float TatgerFPS = 50.0f;

//--------------------------------------------------------------------
// 스크린 버퍼에 플레이어 그리기
//--------------------------------------------------------------------
void Draw_Player(void)
{
	if (g_stPlayer.iVisible)
		Sprite_Draw(g_stPlayer.iX, g_stPlayer.iY, '#');

	for (int i = 0; i < BULLET_COUNT; i++)
	{
		if (bullet[i].iVisible && g_stPlayer.iVisible)
		{
			bullet[i].iY -= 1;
			Sprite_Draw(bullet[i].iX, bullet[i].iY, 'o');
		}

	}
}

//플레이어의 총알 만들기
void Player()
{
	if (monsterCnt <= 0)
	{
		if (stage >= maxStage)
			G_scene = Scene::ALLCLEAR;
		else
		{
			stage++;
			monsterCnt = 0;
			G_scene = Scene::CLEAR;
		}

	}

	for (int i = 0; i < BULLET_COUNT; i++)
	{
		// 스크린 끝에 닿으면 총알 false처리
		if (bullet[i].iY == 0)
		{
			bullet[i].iVisible = false;
			bullet[i].iX = 0;
			bullet[i].iY = 0;
		}

		// 적을 처치하면 총알 false처리
		for (int j = 0; j < sizeof(pattern); j++)
		{
			if (bullet[i].iY == g_stMonster[j].iY && bullet[i].iX == g_stMonster[j].iX
				&& g_stMonster[j].iVisible == true)
			{
				bullet[i].iVisible = false;
				g_stMonster[j].iVisible = false;
				e_index++;
				monsterCnt--;
			}
		}
	}
}

//몬스터
int coordiIndex = 0;
void Monster(void)
{
	if (TickTimer == 300)
	{
		//몬스터 움직임 패턴
		int alive = 0;
		for (int i = 0; i < sizeof(pattern); i++)
		{
			if (g_stMonster[i].iVisible)
			{
				if (coordiIndex > 3)
					coordiIndex = 0;

				g_stMonster[i].iX += coordi_X[coordiIndex];
				g_stMonster[i].iY += coordi_Y[coordiIndex];
				alive++;
			}
		}
		coordiIndex++;
	}

	if (e_index >= sizeof(pattern) - 1)
		e_index = 0;

	// 스크린 밑에 닿으면 총알 false처리
	if (m_bullet[e_index].iY >= dfSCREEN_HEIGHT || m_bullet[e_index].iPower == 0)
	{
		m_bullet[e_index].iVisible = false;
	}

	if (m_bullet[e_index].iVisible == false && g_stMonster[e_index].iVisible == true)
	{
		m_bullet[e_index].iX = g_stMonster[e_index].iX;
		m_bullet[e_index].iY = g_stMonster[e_index].iY;
		m_bullet[e_index].iPower = 3;
		m_bullet[e_index].iVisible = true;
	}
	else
	{
		e_index++;
	}

	for (int i = 0; i < sizeof(pattern); i++)
	{
		// 플레이어를 처치하면 게임오버
		if (m_bullet[i].iY == g_stPlayer.iY && m_bullet[i].iX == g_stPlayer.iX)
		{
			g_stPlayer.iHP -= m_bullet[i].iPower;

			if (g_stPlayer.iHP <= 0)
			{
				g_stPlayer.iVisible = false;
				bullet[i].iVisible = false;
				G_scene = Scene::GAMEOVER;
				return;
			}
		}
	}
}

//몬스터 그리기
void Draw_Monster()
{
	for (int i = 0; i < sizeof(pattern); i++)
	{
		if (g_stMonster[i].iVisible)
		{
			Sprite_Draw(g_stMonster[i].iX, g_stMonster[i].iY, '@');
		}
	}

	for (int i = 0; i < sizeof(pattern); i++)
	{
		if (m_bullet[i].iVisible && g_stMonster[i].iVisible)
		{
			m_bullet[i].iY += 1;
			Sprite_Draw(m_bullet[i].iX, m_bullet[i].iY, '*');
		}
	}
}

//게임로직
void MainGame()
{
	// 키보드 입력
	KeyProcess();
	Buffer_Clear();
	// 로직부
	switch (G_scene)
	{
	case Scene::TITLE:
	{
		cs_Initial();
		const char* title = "★text shooting game★";
		const char* title2 = "시작하려면 엔터키를, 종료시 ESC를 누르세요";
		String_Draw(22, 5, title);
		String_Draw(20, 6, title2);
	}break;
	case Scene::LOAD:
	{
		cs_Initial();
		Initial();
		G_scene = Scene::GAME;
	}break;
	case Scene::GAME:
	{
		// 랜더링
		Draw_Player();
		Draw_Monster();
		Monster();
		Player();
	}break;
	case Scene::CLEAR:
	{
		const char* clear_1 = "★Stage Clear★";
		const char* clear_2 = "다음 스테이지로 가려면 엔터를 누르세요";
		String_Draw(23, 5, clear_1);
		String_Draw(20, 6, clear_2);
	}break;
	case Scene::ALLCLEAR:
	{
		const char* clear_1 = "★★ALL Stage Clear★★";
		const char* clear_2 = "모든 스테이지를 클리어 하셨습니다.";
		const char* clear_3 = "게임을 종료하시려면 ESC를 눌러주세요";
		String_Draw(23, 5, clear_1);
		String_Draw(20, 6, clear_2);
		String_Draw(19, 7, clear_3);
	}break;
	case Scene::GAMEOVER:
	{
		const char* gameover_1 = "Game Over ㅠㅠㅠ";
		const char* gameover_2 = "다시도전 하려면 엔터를 누르세요";
		String_Draw(23, 5, gameover_1);
		String_Draw(20, 6, gameover_2);
	}break;
	}
	Buffer_Flip();
}