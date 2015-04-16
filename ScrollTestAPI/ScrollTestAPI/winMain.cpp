#include <Windows.h>
#include "scroll.h"

#define CLIENT_WIDTH	1024
#define CLIENT_HEIGHT	768

HINSTANCE hInst;	//인스턴스 핸들을 저장하기 위한 전역변수
HWND g_hWnd;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	static TCHAR szClassName[] = L"winMain";	//윈도우 클래스 이름
	static TCHAR szTitle[] = L"for scroll test";			//타이틀 바에 출력될 문자열

	MSG msg;			//메세지 구조체
	HWND hWnd;		//윈도우 핸들
	WNDCLASSEX wc;	//윈도우 클래스 구조체
	hInst = hInstance;	//인스스 핸들 저장
	//------------------------------------------------------ :: 윈도우 구조체에 값을 지정한다. ::-----------------------------------
	wc.cbSize = sizeof(WNDCLASSEX);							//윈도우 클래스 구조체의 크기
	wc.style = CS_HREDRAW | CS_VREDRAW;						//클래스 스타일

	//윈도우 프로시저를 윈도우클래스 구조체에 등록

	wc.lpfnWndProc = WndProc;								//윈도우 프로시저 지정
	wc.cbClsExtra = 0;										//윈도우 클래스 데이터 영역
	wc.cbWndExtra = 0;										//윈도우의 데이터 영역
	wc.hInstance = hInstance;								//인스턴스 핸들
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//아이콘 핸들
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//사용할 커서 핸들
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//바탕색 브러시
	wc.lpszMenuName = NULL;									//메뉴 이름
	wc.lpszClassName = szClassName;							//윈도우 클래스 이름
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);			//작은 아이콘 핸들
	//------------------------------------------------------ :: 윈도우 클래스를 등록한다.
	RegisterClassEx(&wc);								//   :: 윈도우 클래스 등록
	//------------------------------------------------------ :: 프레임 윈도우를 생성
	hWnd = CreateWindow(
		szClassName,									//윈도우 클래스 이름
		szTitle,										//타이틀 바에 출력될 문자열, 앞에서 지정함
		WS_OVERLAPPEDWINDOW,							//윈도우 스타일, 앞에서 지정
		CW_USEDEFAULT,									//윈도우 좌측 상단의 X 좌표
		CW_USEDEFAULT,									//윈도우 좌측 상단의 Y 좌표
		CLIENT_WIDTH,									//윈도우의 폭
		CLIENT_HEIGHT,									//윈도우의 높이
		NULL,											//부모 핸들의 높이
		NULL,											//메뉴, 혹은 자식 윈도우의 식별자
		hInstance,										//윈도우를 생성한 인스턴스 핸들,  WinMain의 첫번째 인자
		NULL											//CREATESTRUCT 구조체를 통해 전달되는 값
		);

	if (!hWnd)						//윈도우 생성에 실패 시
		return(FALSE);				//프로그램 종료
	else
		g_hWnd = hWnd;
	//------------------------------------------------------ :: 프레임 윈도우를 화면에 출력
	ShowWindow(hWnd, nCmdShow);						//윈도우를 화면에 나타낸다.
	UpdateWindow(hWnd);								//윈도우의 클라이언트 영역을 칠한다.
	//------------------------------------------------------ :: 메시지 루프 : 루프를 돌며 메세지 큐로부터 메세지를 얻어와 처리한다.
	while (GetMessage(&msg, NULL, 0, 0))			//윈도우를 화면에 나타낸다.
	{
		TranslateMessage(&msg);						//키보드 메세지 번역
		DispatchMessage(&msg);						//메세지를 해당 윈도우 프로시저로 보낸다.
	}
	return msg.wParam;
}

void numFix(LPWSTR DestMsg, int fixnum)
{
	TCHAR Fix[20] = L"0\0", returnMsg[20];
	int count = 0;

	while (fixnum >0)
	{
		int endnum = fixnum % 10;
		fixnum /= 10;
		Fix[count] = '0' + endnum;
		count++;
		if (count % 4 == 3 && fixnum > 0)
		{
			Fix[count] = ',';
			count++;
		}
	}
	if (count == 0)
		count++;
	for (int fa = 0; fa<count; fa++)
		returnMsg[fa] = Fix[count - fa - 1];
	returnMsg[count] = '\0';

	//	printf("%s\n", returnMsg);

	lstrcpy(DestMsg, returnMsg);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int iMaxValue = 12515;
	static scroll scr;

	scr.OnProcessingMouseMessage(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_CREATE:
		scr.initialize(scroll::SCRType::SCR_H, iMaxValue, 250, 250 - 24, { 12, 220 },145);

		break;
	case WM_LBUTTONDOWN:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_RBUTTONDOWN:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_MOUSEMOVE:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		case 'Q':
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		RECT rcClient;
		GetWindowRect(hWnd, &rcClient);
		rcClient = { 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
		auto LayDC = CreateCompatibleDC(hdc);
		auto Lay = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
		SelectObject(LayDC, Lay);
		FillRect(LayDC, &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));
		//-----------------------------------------------------

		scr.Draw(LayDC, rcClient);
		TCHAR str[256], str_num[20];
		numFix(str_num, (int)scr.GetNowSrcPosition());
		wsprintf(str, L"now value : %s", str_num);
		TextOut(LayDC, 50, 200, str, lstrlen(str));
		numFix(str_num, iMaxValue);
		wsprintf(str, L"max value : %s", str_num);
		TextOut(LayDC, 50, 180, str, lstrlen(str));
		//-----------------------------------------------------
		
		BitBlt(hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, LayDC, 0, 0, SRCCOPY);

		DeleteObject(Lay);
		DeleteDC(LayDC);
		EndPaint(hWnd, &ps);
		break;

	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);	//남은 메세지는 DefWindowProc()가 처리
	}

	return 0;
}