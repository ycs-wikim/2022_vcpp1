// 11_10.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "11_10.h"
#include <stdlib.h>
#include <time.h>

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

    // 프로그램 다중 실행 방지 기능

    HANDLE mux;

    // OS에 등록되어 있는 이름 가운데 asdf가 있으면 핸들이 반환
    mux = OpenMutex(MUTEX_ALL_ACCESS, false, L"asdf");

    // 확인 : 이미 등록되어 있는 이름이 존재하는지
    if (NULL == mux)
    {
        // OS 이름 영역에 등록된 asdf 가 존재하지 않는 상태
        // 이름을 등록
        CreateMutex(NULL, true, L"asdf");
    }
    else
    {
        // OS 이름 영역에 등록된 asdf가 존재!하는 상태
        // 이미 동일한 이름을 갖는 프로그램이 동작 중인 상태
        //MessageBox(NULL, L"이미 실행 중인가벼...", L"저런...", MB_OK);
        return 0;
    }

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1110, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1110));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1110));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1110);
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

//LPARAM g_lParam;
HWND g_hWnd;        // 스레드의 인수로 사용하지 않는 경우

// 공유 자원
int g_x;

// 임계영역 자료형
CRITICAL_SECTION g_cs;

// 세마포어 자료형
HANDLE g_Sem;

// 뮤텍스 자료형
HANDLE g_Mux;

// 스레드 생성 : 스레드 함수는 원형이 고정
DWORD WINAPI drawLines(LPVOID param)
{
    int x, y, i;
    HDC hdc = GetDC(g_hWnd);
    HPEN osp, myp;

    // 클릭된 위치 획득
    x = LOWORD((LPARAM)param);
    y = HIWORD((LPARAM)param);

    // 2-2. 세마포어 대기 : 전체 자원의 개수 3개 --> 진입 가능한 객체는 3개
    //      특징 : 작업 자체를 허용 여부
    WaitForSingleObject(g_Sem, INFINITE);
    for (i = 0; i < y; i++)
    {
        myp = CreatePen(PS_SOLID, 3, RGB(rand() % 256, rand() % 256, rand() % 256));
        osp = (HPEN)SelectObject(hdc, myp);

        MoveToEx(hdc, x, 0, NULL);
        // 임계 영역 시작
        // 1-2. 크리티컬 섹션 대기
        //EnterCriticalSection(&g_cs);

        // 3-2. 뮤텍스 대기
        //WaitForSingleObject(g_Mux, INFINITE);

        g_x = x;
        Sleep(30);          // 문맥 교환 효과
        LineTo(hdc, g_x, i);

        // 1-3. 크리티컬 섹션 해제
        //LeaveCriticalSection(&g_cs);
        
        // 3-3. 뮤텍스 해제
        //ReleaseMutex(g_Mux);

        // 임계 영역 종료
        SelectObject(hdc, osp);
        DeleteObject(myp);
    }
    // 2-3. 세마포어 해제
    ReleaseSemaphore(g_Sem, 1, NULL);

    ReleaseDC(g_hWnd, hdc);

    ExitThread(0);  // 자원 정리 후 종료 요청
    return 0;   // 스레드 함수의 반환 값
}



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

    case WM_CREATE:
    {
        srand(time(NULL));
        g_hWnd = hWnd;
        // 1-1. 크리티컬 섹션 생성
        InitializeCriticalSection(&g_cs);

        // 2-1. 세마포어 객체 생성
        // 첫번째 인수 : 보안 속성 - 프로세스의 보안 속성을 상속 받음
        // 두번째 인수 : 초기 개수 값
        // 세번째 인수 : 최대 개수 값
        // 네번째 인수 : 이름 - 프로세스 내부에서 유효한 ID
        g_Sem = CreateSemaphore(NULL, 3, 3, NULL);

        // 3-1. 뮤텍스 생성
        // 1번째 인수 : 보안 속성
        // 2번째 인수 : 소유자가 있는지 여부
        // 3번째 인수 : 이름 - OS에서 유효한 ID
        g_Mux = CreateMutex(NULL, false, NULL);
    }
        break;

    case WM_LBUTTONDOWN:
    {
        CreateThread(NULL, 0, drawLines, (LPVOID)lParam, 0, NULL);
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
    {
        // 1-4. 크리티컬 섹션 제거
        DeleteCriticalSection(&g_cs);
        // 2-4. 세마포어 핸들을 닫음
        CloseHandle(g_Sem);

        // 3-4. 뮤텍스의 핸들을 닫음
        CloseHandle(g_Mux);
        PostQuitMessage(0);
    }
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
