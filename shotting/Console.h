
//////////////////////////////////////////////////////////////
// Windows 의 콘솔 화면에서 커서 제어.
//
//////////////////////////////////////////////////////////////
#ifndef __CONSOLE__
#define __CONSOLE__

#define dfSCREEN_WIDTH	81		// 콘솔 가로 80칸 + NULL
#define dfSCREEN_HEIGHT	24		// 콘솔 세로 25칸 + NULL
#define BULLET_COUNT 10			//플레이어 총알
#define M_BULLET_COUNT 20		//몬스터 총알
//-------------------------------------------------------------
// 콘솔 제어를 위한 준비 작업.
//
//-------------------------------------------------------------
void cs_Initial(void);

//-------------------------------------------------------------
// 콘솔 화면의 커서를 X, Y 좌표로 이동시킨다.
//
//-------------------------------------------------------------
void cs_MoveCursor(int iPosX, int iPosY);

//-------------------------------------------------------------
// 콘솔 화면을 조기화 한다.
//
//-------------------------------------------------------------
void cs_ClearScreen(void);

//--------------------------------------------------------------------
// 스테이지에 따라서 데이터파싱
//--------------------------------------------------------------------
char* DataInfo(const char* file_path);

//--------------------------------------------------------------------
// 게임 시작시 전체 데이터 초기화용.
//--------------------------------------------------------------------
void Initial(void);


//////////////////////////////////////////////////////////////////////
//
// 출력부 함수들....
//
//////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------
// 버퍼의 내용을 화면으로 찍어주는 함수.
//
// 적군,아군,총알 등을 szScreenBuffer 에 넣어주고, 
// 1 프레임이 끝나는 마지막에 본 함수를 호출하여 버퍼 -> 화면 으로 그린다.
//--------------------------------------------------------------------
void Buffer_Flip(void);
//--------------------------------------------------------------------
// 화면 버퍼를 지워주는 함수
//
// 매 프레임 그림을 그리기 직전에 버퍼를 지워 준다. 
// 안그러면 이전 프레임의 잔상이 남으니까
//--------------------------------------------------------------------
void Buffer_Clear(void);

//--------------------------------------------------------------------
// 버퍼의 특정 위치에 원하는 문자를 출력.
//
// 입력 받은 X,Y 좌표에 아스키코드 하나를 출력한다. (버퍼에 그림)
//--------------------------------------------------------------------
void Sprite_Draw(int iX, int iY, char chSprite);

//////////////////////////////////////////////////////////////////////
//
// 플레이어 처리부 함수들
//
//////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------
// 키보드입력을 받아서 플레이어를 처리한다.
//
// ESC 를 누를 경우 false 리턴, (종료처리)
//--------------------------------------------------------------------
bool KeyProcess(void);

//--------------------------------------------------------------------
// 스크린 버퍼에 플레이어 그리기
//--------------------------------------------------------------------
void Draw_Player(void);

//게임데이터 가져오기
char* DataInfo(const char* file_path);

//버퍼의 특정위치에 원하는 문자열을 출력
void String_Draw(int iX, int iY, const char* chSprite);

//플레이어의 총쏘기
void Player();

//몬스터 생성, 몬스터 공격
void Monster(void);

//몬스터, 몬스터 공격 그리기
void Draw_Monster();

//게임로직
void MainGame();

#endif

