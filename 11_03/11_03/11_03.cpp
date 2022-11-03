// 11_03.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "11_03.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 프로젝트의 단독 실행 : Ctrl + F5
    // 프로젝트의 디버깅 모드 실행 : F5

    // 코드 라인 별로 멈춤 위치(브레이크 포인트)를 지정 : F9
    // 한 라인씩 실행 : F10
    // 확인 완료 후,
    //  1. 프로그램의 동작 재개 : F5
    //  2. 프로그램의 동작 종료 : SHIFT + F5

    int x = 0;
    x++;
    x = x * 8 + 67;

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1103, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1103));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1103));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1103);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// 스레드 함수 : 원형이 고정 ==> DWORD WINAPI 함수명(LPVOID 변수명)
//  LPVOID : 포인터 자료형. CreateThread( )의 인수가 전달
//  반환 값이 DWORD 이므로 반드시 반환 값을 설정해야 합니다.

// 마우스의 좌표값을 전역 변수 선언
LPARAM g_lParam;

// 선을 그려주는 스레드 함수
DWORD WINAPI drawLines(LPVOID param)    // hWnd가 전달
{
    HWND hWnd = (HWND)param;    // 사용할 수 있는 hWnd 획득
    int x, y, i;
    HDC hdc;
    int hp = 35;
    // 문자열 버퍼 선언
    TCHAR buf[128] = { 0, };
    TCHAR per[512] = { 0, };

    hdc = GetDC(hWnd);

    x = LOWORD(g_lParam);
    y = HIWORD(g_lParam);

    wsprintf(per, L"안녕 배고파 ");


    for (i = 0; i < y; i++)
    {
        // 스레드의 종료 조건 확인 및 종료
        //hp--;
        if (0 == hp)
        {
            // 스레드를 종료
            // 1. 반복을 완전히 종료
            //break;
            // 2. 스레드를 종료
            ExitThread(0);
        }
        
        /// 화면 출력 : printf( " ", ... ) 주어진 형식으로 변수 값이 입력되어 화면 출력
        // sprintf 함수 : 주어진 형식으로 변수 값이 입력되어 버퍼에 출력
        wsprintf(buf, L"current(%d)/total(%d)", i, y);
        // 획득된 버퍼의 내용물을 지정한 위치에 출력
        TextOut(hdc, x - 10, i + 10, buf, lstrlen(buf));

        //strcat(buf, "|");
        lstrcat(per, L"|");
        TextOut(hdc, x - 10, i + 20, per, lstrlen(per));

        // 최상위 x 좌표 고정
        MoveToEx(hdc, x, 0, NULL);
        // i 값이 증가되면서 반복되므로, 선이 그려지는 효과
        LineTo(hdc, x, i);
        // delay : 설정하지 않으면 즉시 그려지므로 그려지는 효과가 보이지 않음

        // 사용된 문자열 버퍼를 초기화
        memset(buf, 0x00, lstrlen(buf));
        Sleep(30);
    }

    ReleaseDC(hWnd, hdc);

    // 스레드의 종료
    ExitThread(0);
    return 0;   // 반환. 0은 정상 종료. -9 반환 시 강제 종료
}

// 스레드의 제어를 위해 선언
HANDLE g_hdl[3000] = { NULL, };
// 배열 첨자용 변수
int g_index = 0;

// 메모장 프로세스 생성을 위한 구조체 선언
PROCESS_INFORMATION g_pi;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
/*
    // 메모장 프로세스 생성
    case WM_LBUTTONDOWN:
    {
        STARTUPINFO si = { 0, };
        WCHAR notepad[32] = L"notepad.exe";

        CreateProcess(NULL, notepad, NULL, NULL, false, 0, NULL, NULL, &si, &g_pi);
    }
        break;

    // 메모장의 Base Thread를 일시 중단
    case WM_RBUTTONDOWN:
    {
        SuspendThread(g_pi.hThread);
    }
        break;

    // 메모장의 Base Thread를 재개
    case WM_KEYDOWN:
    {
        ResumeThread(g_pi.hThread);
    }
        break;
*/
    // 스레드의 일시 중단
    case WM_RBUTTONDOWN:
    {
        int i = 0;
        for(i = 0 ; i < 3000; i++)
            SuspendThread(g_hdl[i]);
    }
        break;

    // 스레드의 재개
    case WM_KEYDOWN:
    {
        for(int i = 0; i < 3000; i++)
            ResumeThread(g_hdl[i]);
    }
        break;

    // 스레드의 생성
    case WM_LBUTTONDOWN:
    {
        // 스레드의 ID 값 확인 선언
        DWORD tid = 0;

        g_lParam = lParam;
        g_hdl[g_index++] = CreateThread(NULL, 0, drawLines, hWnd, 0, &tid);
        // 스레드 생성 여부를 확인
        if (NULL == g_hdl)
        {
            MessageBox(hWnd, L"스레드 생성 실패", L"앗", MB_OK);
            // 생성 실패 시, 진행할 필요가 없음
            break;
        }

        // 생성한 스레드가 일시 중단
        //SuspendThread(g_hdl);

        // 일시 중단 시간(3초) 설정
        //Sleep(3000);

        // 스레드의 실행 재개
        //ResumeThread(g_hdl);
    }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
