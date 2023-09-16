#include "stdafx.h"
#include "Console.h"
#include "enum.h"
#include "gameCode.h"
#pragma comment(lib, "winmm.lib")

using namespace std;

int main(void)
{
	G_scene = Scene::TITLE;
	//--------------------------------------------------------------------
	// ������ ���� ����
	// �� ������  1�� ���� 1������ �̴�. FPS 50���� ���߱�
	//--------------------------------------------------------------------
	StartTime = timeGetTime();
	
	while (1)
	{
		CurrentTime = timeGetTime();
		
		if (escape)
			return 0;

		if (deltaTime >= ((1/50.0f) * 1000.0f))
		{
			MainGame();
			LoopCounter++;
			TickTimer += deltaTime;
			deltaTime = 0.0f;
		}

		EndTime = timeGetTime() - CurrentTime;
		deltaTime += EndTime;
		if (TickTimer >= 1000.0f)
		{
			cout << "\n���罺������ : " << stage << endl;
			//cout << "\nTime : " << Timecounter << endl;
			cout << "\nFPS : " << (LoopCounter) << endl;
			//cout << "deltaTime : " << deltaTime << endl;
			TickTimer = 0.0f;
			LoopCounter = 0;
			Timecounter++;
		}
	}
}