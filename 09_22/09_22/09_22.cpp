// 09_22.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "09_22.h"

#include <stdlib.h> // 랜덤 함수 사용
#include <time.h>   // 시간 값 획득 : 랜덤 시드 값 생성용

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

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0922, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0922));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0922));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0922);
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

// 제어할 사각형 선언
RECT g_rect;
// 상대방의 사각형 선언
RECT g_you;
// 그라운드
RECT g_ground;
// 아이템
RECT g_item;
// 시간 표시용 사각형
RECT g_timerect;

// 게임 시간
int g_gametime;

// 타이머 시간 값 제어 변수
int g_timer;

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

    case WM_TIMER:
       // MessageBox(hWnd, L"타이머가 도착했어요", L"심재호", MB_OK);
        if (1 == wParam)
        {
            // 타이머 값 감소
            if (100 <= g_timer)
                g_timer -= 50;
            // 타이머 재설정
            KillTimer(hWnd, 1);
            SetTimer(hWnd, 1, g_timer, NULL);

            if (g_rect.left < g_you.left)
            {
                g_you.left -= 10;
                g_you.right -= 10;
            }
            else
            {
                g_you.left += 10;
                g_you.right += 10;
            }
            if (g_rect.top < g_you.top)
            {
                g_you.top -= 10;
                g_you.bottom -= 10;
            }
            else
            {
                g_you.top += 10;
                g_you.bottom += 10;
            }
            /////////////// 상대의 좌표 계산 및 이동 완료

            // 겹침이 발생한 경우의 겸침 영역 좌표를 받아올 변수
            RECT isr;

            if (true == IntersectRect(&isr, &g_rect, &g_you))
            {
                // 타이머를 해제
                KillTimer(hWnd, 1);
                // 나는 현재 잡힌 상태
                MessageBox(hWnd, L"아이쿠", L"자펴쏘", MB_OK);
            }
        }
        else if (2 == wParam)
        {
            g_gametime--;
            // 시간에 따라 시간 표시 사각형의 크기 조절
            g_timerect.right = 10 + ( g_gametime * 10 );

            if (0 == g_gametime)
            {
                MessageBox(hWnd, L"게임 오버", L"go", MB_OK);
                // 타이머 해제
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 2);
            }
        }
        InvalidateRect(hWnd, NULL, true);
        break;
    // 프로그램이 실행될때, 단 한번 OS가 호출해주는 생성자와 같은 윈도우 메시지
    case WM_CREATE:
    {
        // 시드 값을 생성
        srand(time(NULL));
        // 타이머 기본 값 설정
        g_timer = 1000;

        // 게임 시간 설정
        g_gametime = 100;

        // 타이머 설정 - 상대방의 이동
        SetTimer(hWnd, 1, g_timer, NULL);
        // 시간을 재기 위한 타이머
        SetTimer(hWnd, 2, 1000, NULL);

        // 나의 사각형의 기본 좌표를 설정
        g_rect.left = 10;
        g_rect.top = 10;
        g_rect.right = 110;
        g_rect.bottom = 110;

        // 상대방 사각형의 기본 좌표를 설정
        g_you.left = 300;
        g_you.top = 300;
        g_you.right = 400;
        g_you.bottom = 400;

        // 그라운드 좌표
        g_ground.left = 10;
        g_ground.top = 10;
        g_ground.right = 500;
        g_ground.bottom = 500;

        // 아이템의 좌표 설정
        g_item.left = (rand() % 350) + 10;
        g_item.top = (rand() % 350) + 10;
        g_item.right = g_item.left + 100;
        g_item.bottom = g_item.top + 100;

        // 시간 표시 사각형
        g_timerect.left = 10;
        g_timerect.top = 520;
        g_timerect.right = 110;
        g_timerect.bottom = 560;

    }
        break;

    case WM_LBUTTONDOWN:
    {
        RECT is, pos;

        pos.left = LOWORD(lParam);
        pos.top = HIWORD(lParam);
        pos.right = pos.left + 1;
        pos.bottom = pos.top + 1;

        if (true == IntersectRect(&is, &g_rect, &pos))
        {
            //MessageBox(hWnd, L"잡혔다", L"Yes", MB_OK);
        }

    }
        break;

    case WM_LBUTTONUP:
    {
        int x, y;

        x = LOWORD(lParam);
        y = HIWORD(lParam);

        g_rect.left = x;
        g_rect.top = y;
        g_rect.right = g_rect.left + 100;
        g_rect.bottom = g_rect.top + 100;
    }
        break;

    // 키보드 입력
    case WM_KEYDOWN:
    {
        // 키 값에 따라 처리를 수행
        switch (wParam)
        {
        // 왼쪽 방향키 입력
        case VK_LEFT:
            g_rect.left -= 10;
            g_rect.right -= 10;
            break;
        // 오른쪽 방향키 입력
        case VK_RIGHT:
            g_rect.left += 10;
            g_rect.right += 10;
            break;
        // 위쪽 방향키 입력
        case VK_UP:
            g_rect.top -= 10;
            g_rect.bottom -= 10;
            break;
        // 아래쪽 방향키 입력
        case VK_DOWN:
            g_rect.top += 10;
            g_rect.bottom += 10;
            break;
        }

        // 그라운드 범위 확인 및 좌표 재설정
        if (g_rect.left < 10)
        {
            g_rect.left = 10;
            g_rect.right = 110;
        }
        if (g_rect.top < 10)
        {
            g_rect.top = 10;
            g_rect.bottom = 110;
        }
        if (g_rect.right > 500)
        {
            g_rect.right = 500;
            g_rect.left = 400;
        }
        if (g_rect.bottom > 500)
        {
            g_rect.bottom = 500;
            g_rect.top = 400;
        }

        // 아이템을 먹었는가? 확인
        RECT is;

        if (true == IntersectRect(&is, &g_rect, &g_item))
        {
            // 아이템을 획득한 상태
            // 점수 상승 + 시간 상승
            g_gametime += 3;
            // 아이템의 위치 이동
            g_item.left = (rand() % 350) + 10;
            g_item.top = (rand() % 350) + 10;
            g_item.right = g_item.left + 100;
            g_item.bottom = g_item.top + 100;
        }

        // 변경된 좌표 값이 적용된 그림을 그리도록 화면 무효화를 요청
        InvalidateRect(hWnd, NULL, true);
    }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            // 그라운드 사각형 그린다.
            Rectangle(hdc, g_ground.left, g_ground.top, g_ground.right, g_ground.bottom);

            // 나의 사각형 그린다.
            Rectangle(hdc, g_rect.left, g_rect.top, g_rect.right, g_rect.bottom);

            // 아이템을 그린다.
            Ellipse(hdc, g_item.left, g_item.top, g_item.right, g_item.bottom);

            // 상대방 사각형 그린다.
            Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);

            // 시간 정보 출력
            Rectangle(hdc, g_timerect.left, g_timerect.top, g_timerect.right, g_timerect.bottom);
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
