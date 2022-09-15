// 09_15.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "09_15.h"

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
    LoadStringW(hInstance, IDC_MY0915, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0915));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0915));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0915);
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

/// 전역 변수는 초기화하지 않아도 항상 [ 0 ] 으로 초기화 된다.
int g_x, g_y;
/// RECT 자료형 선언
RECT g_rect;        // left, top, right, bottom은 모두 0으로 초기화된 상태

// 마우스가 눌린 상태인지 확인을 위한 플래그 변수
bool g_press;       // true(눌린 상태), false(눌리지 않은 상태)

// 콜백 함수(*) --> 호출 시에만 데이터가 존재하고, 종료 시 모두 데이터 삭제
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 함수 안에서 선언된 모든 변수 값은 함수 종료 시 남아있지 않다!
    int x = 223, y = 878;

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

    // 오른쪽 마우스 버튼을 눌렀을 때 발생하는 메시지
    case WM_RBUTTONDOWN:
    {
        // 화면 무효화 영역을 지정하기 위한 자료형 선언
        RECT area;

        // 사각형 영역 설정
        area.left = 50;
        area.top = 50;
        area.right = 600;
        area.bottom = 600;

        // OS에게 WM_PAINT를 요청하는 API
        //InvalidateRect(hWnd, NULL, TRUE);
        InvalidateRect(hWnd, &area, TRUE);
    }
        break;

    case WM_LBUTTONUP:
    {
        // 왼쪽 버튼이 눌리지 않았음을 설정
        g_press = false;
    }
        break;

    case WM_LBUTTONDOWN:
    {
        // 왼쪽 버튼이 눌렸음을 설정
        g_press = true;
    }
        break;

    // 마우스가 이동했을 때 발생하는 메시지
    case WM_MOUSEMOVE:
        //break;

    // 왼쪽 마우스 버튼을 뗐을 때 발생하는 메시지
    //case WM_LBUTTONUP:
        //break;

    // 왼쪽 마우스 버튼을 눌렀을때 발생하는 메세지
    //case WM_LBUTTONDOWN:
    {
        if (false == g_press)
            break;

        HDC hdc;
        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
        g_rect.right = g_x;
        g_rect.bottom = g_y;

        x = g_x;
        y = g_y;

        hdc = GetDC(hWnd);

        Rectangle(hdc, 10, 10, g_x, g_y);
        Ellipse(hdc, 10, 10, g_x, g_y);
        MoveToEx(hdc, 0, 0, NULL);
        LineTo(hdc, x, y);

        MoveToEx(hdc, x, 10, NULL);
        LineTo(hdc, 10, y);

        ReleaseDC(hWnd, hdc);

        
    }
        break;

    // ***** 화면 무효화가 발생했을때, OS가 다시 그리라고 보내주는 윈도우 메세지
    // 무효화가 발생하는 조건
    // 1. 화면 밖으로 윈도우가 이동한 경우
    // 2. 창 크기 변경
    // 3. 창 최소화
    // 4. 창 최대화
    // 5. 프로그램이 시작 되자마자 반드시 한번 호출해준다.
    // 6. 프로그래머가 직접 무효화 처리 메시지를 발생시킨다
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            // RECT 자료형을 이용한 그리기
            Rectangle(hdc, 10, 10, g_rect.right, g_rect.bottom);
            Ellipse(hdc, 10, 10, g_rect.right, g_rect.bottom);

            MoveToEx(hdc, 0, 0, NULL);
            LineTo(hdc, g_rect.right, g_rect.bottom);

            /*
            // g_x, g_y를 이용한 그리기
            Rectangle(hdc, 10, 10, g_x, g_y);
            Ellipse(hdc, 10, 10, g_x, g_y);

            MoveToEx(hdc, 0, 0, NULL);
            LineTo(hdc, g_x, g_y);
            */            

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
