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
		return DefWindowProc(hWnd, uMsg, wParam, lParam);	//���� �޼����� DefWindowProc()�� ó��
	}

	return 0;
}