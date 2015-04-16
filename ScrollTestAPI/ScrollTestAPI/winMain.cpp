#include <Windows.h>
#include "scroll.h"

#define CLIENT_WIDTH	1024
#define CLIENT_HEIGHT	768

HINSTANCE hInst;	//�ν��Ͻ� �ڵ��� �����ϱ� ���� ��������
HWND g_hWnd;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	static TCHAR szClassName[] = L"winMain";	//������ Ŭ���� �̸�
	static TCHAR szTitle[] = L"for scroll test";			//Ÿ��Ʋ �ٿ� ��µ� ���ڿ�

	MSG msg;			//�޼��� ����ü
	HWND hWnd;		//������ �ڵ�
	WNDCLASSEX wc;	//������ Ŭ���� ����ü
	hInst = hInstance;	//�ν��� �ڵ� ����
	//------------------------------------------------------ :: ������ ����ü�� ���� �����Ѵ�. ::-----------------------------------
	wc.cbSize = sizeof(WNDCLASSEX);							//������ Ŭ���� ����ü�� ũ��
	wc.style = CS_HREDRAW | CS_VREDRAW;						//Ŭ���� ��Ÿ��

	//������ ���ν����� ������Ŭ���� ����ü�� ���

	wc.lpfnWndProc = WndProc;								//������ ���ν��� ����
	wc.cbClsExtra = 0;										//������ Ŭ���� ������ ����
	wc.cbWndExtra = 0;										//�������� ������ ����
	wc.hInstance = hInstance;								//�ν��Ͻ� �ڵ�
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//������ �ڵ�
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//����� Ŀ�� �ڵ�
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//������ �귯��
	wc.lpszMenuName = NULL;									//�޴� �̸�
	wc.lpszClassName = szClassName;							//������ Ŭ���� �̸�
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);			//���� ������ �ڵ�
	//------------------------------------------------------ :: ������ Ŭ������ ����Ѵ�.
	RegisterClassEx(&wc);								//   :: ������ Ŭ���� ���
	//------------------------------------------------------ :: ������ �����츦 ����
	hWnd = CreateWindow(
		szClassName,									//������ Ŭ���� �̸�
		szTitle,										//Ÿ��Ʋ �ٿ� ��µ� ���ڿ�, �տ��� ������
		WS_OVERLAPPEDWINDOW,							//������ ��Ÿ��, �տ��� ����
		CW_USEDEFAULT,									//������ ���� ����� X ��ǥ
		CW_USEDEFAULT,									//������ ���� ����� Y ��ǥ
		CLIENT_WIDTH,									//�������� ��
		CLIENT_HEIGHT,									//�������� ����
		NULL,											//�θ� �ڵ��� ����
		NULL,											//�޴�, Ȥ�� �ڽ� �������� �ĺ���
		hInstance,										//�����츦 ������ �ν��Ͻ� �ڵ�,  WinMain�� ù��° ����
		NULL											//CREATESTRUCT ����ü�� ���� ���޵Ǵ� ��
		);

	if (!hWnd)						//������ ������ ���� ��
		return(FALSE);				//���α׷� ����
	else
		g_hWnd = hWnd;
	//------------------------------------------------------ :: ������ �����츦 ȭ�鿡 ���
	ShowWindow(hWnd, nCmdShow);						//�����츦 ȭ�鿡 ��Ÿ����.
	UpdateWindow(hWnd);								//�������� Ŭ���̾�Ʈ ������ ĥ�Ѵ�.
	//------------------------------------------------------ :: �޽��� ���� : ������ ���� �޼��� ť�κ��� �޼����� ���� ó���Ѵ�.
	while (GetMessage(&msg, NULL, 0, 0))			//�����츦 ȭ�鿡 ��Ÿ����.
	{
		TranslateMessage(&msg);						//Ű���� �޼��� ����
		DispatchMessage(&msg);						//�޼����� �ش� ������ ���ν����� ������.
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
	static int iMaxValue = 255;
	static scroll scr_r, scr_g, scr_b;

	scr_r.OnProcessingMouseMessage(hWnd, uMsg, wParam, lParam);
	scr_g.OnProcessingMouseMessage(hWnd, uMsg, wParam, lParam);
	scr_b.OnProcessingMouseMessage(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_CREATE:
		scr_r.initialize(scroll::SCRType::SCR_V, iMaxValue, 200, 200, { 220, 50 }, 145, 10, 7.5);
		scr_g.initialize(scroll::SCRType::SCR_V, iMaxValue, 200, 200, { 245, 50 }, 145, 10, 7.5);
		scr_b.initialize(scroll::SCRType::SCR_V, iMaxValue, 200, 200, { 270, 50 }, 145, 10, 7.5);

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
//		Rectangle(LayDC, 0, 150, 250, 220);
		scr_r.Draw(LayDC, rcClient, RGB((int)scr_r.GetNowSrcPosition(), 0, 0));
		scr_g.Draw(LayDC, rcClient, RGB(0, (int)scr_g.GetNowSrcPosition(), 0));
		scr_b.Draw(LayDC, rcClient, RGB(0, 0, (int)scr_b.GetNowSrcPosition()));
		TCHAR str[256], str_num[20];
		numFix(str_num, (int)scr_r.GetNowSrcPosition());
		wsprintf(str, L"now value(R) : %s", str_num);
		TextOut(LayDC, 50, 240, str, lstrlen(str));
		numFix(str_num, (int)scr_g.GetNowSrcPosition());
		wsprintf(str, L"now value(G) : %s", str_num);
		TextOut(LayDC, 50, 220, str, lstrlen(str));
		numFix(str_num, (int)scr_b.GetNowSrcPosition());
		wsprintf(str, L"now value(B) : %s", str_num);
		TextOut(LayDC, 50, 200, str, lstrlen(str));
		numFix(str_num, iMaxValue);
		wsprintf(str, L"max value : %s", str_num);
		TextOut(LayDC, 50, 180, str, lstrlen(str));
		SetDCBrushColor(LayDC, RGB((int)scr_r.GetNowSrcPosition(), (int)scr_g.GetNowSrcPosition(), (int)scr_b.GetNowSrcPosition()));
		RECT rcColorPallet = { 50, 50, 150, 150 };
		FillRect(LayDC, &rcColorPallet, (HBRUSH)GetStockObject(DC_BRUSH));

		//-----------------------------------------------------
		
		BitBlt(hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, LayDC, 0, 0, SRCCOPY);

		DeleteObject(Lay);
		DeleteDC(LayDC);
		EndPaint(hWnd, &ps);
		break;

	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);	//���� �޼����� DefWindowProc()�� ó��
	}

	return 0;
}