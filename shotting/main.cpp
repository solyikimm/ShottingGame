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
	// 게임의 메인 루프
	// 이 루프가  1번 돌면 1프레임 이다. FPS 50으로 맞추기
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
			cout << "\n현재스테이지 : " << stage << endl;
			//cout << "\nTime : " << Timecounter << endl;
			cout << "\nFPS : " << (LoopCounter) << endl;
			//cout << "deltaTime : " << deltaTime << endl;
			TickTimer = 0.0f;
			LoopCounter = 0;
			Timecounter++;
		}
	}
}