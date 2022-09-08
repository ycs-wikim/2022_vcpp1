// 09_08.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

// 미리 만들어진 헤더 파일 : 별도로 필요하지 않으면 손대지 말것
#include "framework.h"
#include "09_08.h"

// 상수
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

// APIENTRY : 시작 함수임을 알리는 기능
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,         // OS가 생성한 내 정보
                     _In_opt_ HINSTANCE hPrevInstance,  // 동일한 다른 프로그램 인스턴스
                     _In_ LPWSTR    lpCmdLine,          // argc, argv와 동일한 정보
                     _In_ int       nCmdShow)           // 화면 표시 여부
{
    // 매크로 : 포함된 변수가 사용되지 않으면 메모리에서 해제하는 매크로
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0908, szWindowClass, MAX_LOADSTRING);

    // 클래스 등록 함수
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0908));

    // OS가 잘 정리해서 전달해준, 내가 처리해야하는 정보체
    MSG msg;

    // 기본 메시지 루프입니다: 무한 대기
    // GetMessage( ) : 메세지 큐에서 메시지를 가져오는 역할
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            // MSG 구조체를 처리할 수 있는 상태로 번역
            TranslateMessage(&msg);
            // 메세지를 가져와서 실행
            DispatchMessage(&msg);
        }
    }

    // 프로그램 종료
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    // 윈도우 클래스 ==> 내 프로그램의 동작 설정을 입력
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    // 콜백 함수 등록! 매우 중요!
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0908));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0908);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // 설정된 상태로 윈도우를 OS에 등록
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

   // 우리가 보는 백색 바탕을 갖는 윈도우를 생성
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   // 생성 확인
   if (!hWnd)
   {
      return FALSE;
   }
   // 화면에 표시
   ShowWindow(hWnd, nCmdShow);
   // 변경 사항이 있으면 다시 그려라.
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM ==> Window Message
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

int gx, gy;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // OS가 전달한 메시지의 종류를 확인하고, 필요한 처리를 수행
    switch (message)
    {
    // 메뉴 항목, 버튼 등의 입력을 처리하는 메시지
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
                // default window(OS) proc : 
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

        // 마우스 처리 윈도우 메시지 추가
    case WM_LBUTTONDOWN:
    {
        int x, y;   // x, y 좌표계

        gx = LOWORD(lParam);     // x좌표 획득
        gy = HIWORD(lParam);     // y좌표 획득

        HDC hdc = GetDC(hWnd);

        MoveToEx(hdc, 0, 0, NULL);
        LineTo(hdc, x, y);

        ReleaseDC(hWnd, hdc);

    }
        break;


    // 화면을 그릴때 호출
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            // HDC : Handle Device Context
            // 지금부터 화면에 그리기를 하고 싶어요 only in WM_PAINT
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            // 첫번째 API
            TextOut(hdc, 10, 10, L"여자친구/남자친구 이름", 10);

            // 선그리기 : 시작점 설정, 끝점 설정
            MoveToEx(hdc, 0, 0, NULL);  // 시작 점 설정
            LineTo(hdc, 100, 100);

            // 그리기 끝!
            EndPaint(hWnd, &ps);
        }
        break;
    // 프로그램 종료 요청 시 처리할 내용
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    // 처리할 WM가 아닌 경우.
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
