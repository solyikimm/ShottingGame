#pragma once
#include "stdafx.h"
#include "enum.h"
#define _CRT_SECURE_NO_WARNINGS
//--------------------------------------------------------------------
// �������� ���ڸ� ������ ���������� �����͵��� �־���
//--------------------------------------------------------------------
char buff[200]; //����
char re_data[200];
char* DataInfo(const char* file_path)
{
	FILE* file;

	//���� ���� ����
	fopen_s(&file, file_path, "r");
	if (file == NULL)
	{
		printf("������ �� �� �����ϴ�.\n");
		return NULL;
	}

	memset(re_data, 0, sizeof(re_data));

	// ���� ���� �о���� �����Ѵ�.
	while (fgets(buff, sizeof(buff), file) != NULL)
	{
		strcat_s(re_data, sizeof(re_data), buff);
	}

	// ���� �ݱ�
	fclose(file);
	return re_data;
}

int stage = 1;
int maxStage = 0;
int monsterCnt = 0;
char pattern[20] = {};

char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

//���Ӿ�
Scene G_scene;
bool escape = false;

// �÷��̾� �Ѿ�
st_BULLET bullet[BULLET_COUNT];

//--------------------------------------------------------------------
// �÷��̾� �ν��Ͻ� ����, (1�ο� �̴� �ϳ��� ����)
//--------------------------------------------------------------------
st_PLAYER g_stPlayer;


//--------------------------------------------------------------------
// ����,���� ���� �ν��Ͻ� ����
//--------------------------------------------------------------------
st_MONSTER g_stMonster[20];
st_BULLET m_bullet[M_BULLET_COUNT];
int e_index = 0;

//���� ������ ����
int coordi_X[4] = {};
int coordi_Y[4] = {};

//--------------------------------------------------------------------
// ���� ���۽� ��ü ������ �ʱ�ȭ��.
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
		token = strtok_s(NULL, ",", &context);  //���������� 1�̶�� Stage1.txt�� ������
	}

	char grade[2] = {};
	char patType[10] = {};
	char* stageTxt = DataInfo(token);
	sscanf_s(stageTxt, "%[^,], %s", grade, (unsigned int)sizeof(grade), patType, (unsigned int)sizeof(patType)); // ,������ �����о��

	char* enemyInfo = DataInfo("enemyInfo.txt");
	while (*enemyInfo != '\0') 
	{
		if (*enemyInfo == grade[0]) // ù ���ڰ� grade�� ��ġ�ϴ��� Ȯ��
		{
			enemyInfo += 2; // �������� ������ �̵�
			sscanf_s(enemyInfo, "%[^\n]", pattern, (unsigned int)sizeof(pattern));  // ��ġ�ϴ� ���� ���ڿ� ����
		}
		enemyInfo++;
	}

	/*
	pattern_1
	(22,5) (17,5) (12,5) (17,5)
	pattern_2
	(22,10) (17,5) (12,10) (17,5)
	*/

	// patType�� �´� �����͸� ������.txt���� �о��
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

	// �÷��̾� ������ �ʱ�ȭ
	g_stPlayer.iHP = 10;
	g_stPlayer.iVisible = true;
	g_stPlayer.iX = 39;
	g_stPlayer.iY = 19;

	// �÷��̾� �Ѿ� �ʱ�ȭ
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

	// ���� ������ �ʱ�ȭ
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

	// ���� �Ѿ� �ʱ�ȭ
	for (int i = 0; i < M_BULLET_COUNT; i++) 
	{
		if (m_bullet != nullptr) {
			m_bullet[i].iVisible = false;
		}
	}
}

//--------------------------------------------------------------------
// ������ ������ ȭ������ ����ִ� �Լ�.
//
// ����,�Ʊ�,�Ѿ� ���� szScreenBuffer �� �־��ְ�, 
// 1 �������� ������ �������� �� �Լ��� ȣ���Ͽ� ���� -> ȭ�� ���� �׸���.
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
// ȭ�� ���۸� �����ִ� �Լ�
//
// �� ������ �׸��� �׸��� ������ ���۸� ���� �ش�. 
// �ȱ׷��� ���� �������� �ܻ��� �����ϱ�
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
// ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���.
//
// �Է� ���� X,Y ��ǥ�� �ƽ�Ű�ڵ� �ϳ��� ����Ѵ�. (���ۿ� �׸�)
//--------------------------------------------------------------------
void Sprite_Draw(int iX, int iY, char chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
		return;

	szScreenBuffer[iY][iX] = chSprite;
}

//������ Ư�� ��ġ�� ���ϴ� ���ڿ��� ���
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
// Ű�����Է��� �޾Ƽ� �÷��̾ ó���Ѵ�.
//
// ESC �� ���� ��� false ����, (����ó��)
//--------------------------------------------------------------------
bool KeyProcess(void)
{
	// ����Ű.
	if (GetAsyncKeyState(VK_RETURN) & 0x8001)
	{
		G_scene = Scene::LOAD;
	}

	// ���� ����Ű.
	if (GetAsyncKeyState(VK_LEFT))
	{
		g_stPlayer.iX -= 1;
	}
	// ������ ����Ű.
	if (GetAsyncKeyState(VK_RIGHT))
	{
		g_stPlayer.iX += 1;
	}
	// ���� ����Ű.
	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		g_stPlayer.iY--;
	}
	// �Ʒ��� ����Ű.
	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		g_stPlayer.iY++;
	}

	//-------------------------------------------------------------
	// �÷��̾� �̵� �ݰ� ����.
	// ���� ȭ�鿡�� �÷��̾ �̵� ������ ������ �����Ѵ�.
	//-------------------------------------------------------------

	g_stPlayer.iX = max(g_stPlayer.iX, 0);
	g_stPlayer.iX = min(g_stPlayer.iX, 79);
	g_stPlayer.iY = max(g_stPlayer.iY, 0);
	g_stPlayer.iY = min(g_stPlayer.iY, 23);

	if (G_scene == Scene::GAME)
	{
		// ��Ʈ�� Ű. (�̻��� Ű)
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

	// ESC Ű. (����)
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		// ���� ���, exit����
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
// ��ũ�� ���ۿ� �÷��̾� �׸���
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

//�÷��̾��� �Ѿ� �����
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
		// ��ũ�� ���� ������ �Ѿ� falseó��
		if (bullet[i].iY == 0)
		{
			bullet[i].iVisible = false;
			bullet[i].iX = 0;
			bullet[i].iY = 0;
		}

		// ���� óġ�ϸ� �Ѿ� falseó��
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

//����
int coordiIndex = 0;
void Monster(void)
{
	if (TickTimer == 300)
	{
		//���� ������ ����
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

	// ��ũ�� �ؿ� ������ �Ѿ� falseó��
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
		// �÷��̾ óġ�ϸ� ���ӿ���
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

//���� �׸���
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

//���ӷ���
void MainGame()
{
	// Ű���� �Է�
	KeyProcess();
	Buffer_Clear();
	// ������
	switch (G_scene)
	{
	case Scene::TITLE:
	{
		cs_Initial();
		const char* title = "��text shooting game��";
		const char* title2 = "�����Ϸ��� ����Ű��, ����� ESC�� ��������";
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
		// ������
		Draw_Player();
		Draw_Monster();
		Monster();
		Player();
	}break;
	case Scene::CLEAR:
	{
		const char* clear_1 = "��Stage Clear��";
		const char* clear_2 = "���� ���������� ������ ���͸� ��������";
		String_Draw(23, 5, clear_1);
		String_Draw(20, 6, clear_2);
	}break;
	case Scene::ALLCLEAR:
	{
		const char* clear_1 = "�ڡ�ALL Stage Clear�ڡ�";
		const char* clear_2 = "��� ���������� Ŭ���� �ϼ̽��ϴ�.";
		const char* clear_3 = "������ �����Ͻ÷��� ESC�� �����ּ���";
		String_Draw(23, 5, clear_1);
		String_Draw(20, 6, clear_2);
		String_Draw(19, 7, clear_3);
	}break;
	case Scene::GAMEOVER:
	{
		const char* gameover_1 = "Game Over �ФФ�";
		const char* gameover_2 = "�ٽõ��� �Ϸ��� ���͸� ��������";
		String_Draw(23, 5, gameover_1);
		String_Draw(20, 6, gameover_2);
	}break;
	}
	Buffer_Flip();
}