

#include "framework.h"
#include "Ejercicio3.h"

#define MAX_LOADSTRING 100


HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

static int  g_cx = 0, g_cy = 0;                 
static bool g_isWide = false;                   
static HFONT g_hFontTitle = nullptr;            
const  int  kThreshold = 700;                   
const  int  kMargin = 24;                    

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
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

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EJERCICIO3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EJERCICIO3));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EJERCICIO3));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EJERCICIO3);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Almacenar identificador de instancia en una variable global

    // Podés ajustar tamaño inicial si querés probar rápido el umbral (ej. 800x300)
    HWND hWnd = CreateWindowW(
        szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, // ancho/alto por defecto del template
        nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PROPÓSITO: Procesa mensajes de la ventana principal.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        RECT rc; GetClientRect(hWnd, &rc);
        g_cx = rc.right - rc.left;
        g_cy = rc.bottom - rc.top;
        g_isWide = (g_cx >= kThreshold);

        // Fuente "noticia" (opcional)
        if (!g_hFontTitle) {
            g_hFontTitle = CreateFontW(
                -22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        }
        return 0;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
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
        return 0;
    }

    // Redimensionamiento: actualiza g_cx/g_cy y decide el modo responsive
    case WM_SIZE:
    {
        g_cx = LOWORD(lParam);
        g_cy = HIWORD(lParam);

        bool newIsWide = (g_cx >= kThreshold);
        if (newIsWide != g_isWide) {
            g_isWide = newIsWide; // cambió alineación
        }
        InvalidateRect(hWnd, nullptr, TRUE); // repintar (incluye W×H de depuración)
        return 0;
    }

    // Pintado: titular y W×H
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        SetBkMode(hdc, TRANSPARENT);

        HFONT old = nullptr;
        if (g_hFontTitle) old = (HFONT)SelectObject(hdc, g_hFontTitle);

        // Titular del portal
        const wchar_t* kHeadline = L"Portal de noticias Ultimas novedades";

        RECT rc; GetClientRect(hWnd, &rc);

        // Área del titular (parte superior)
        RECT rcTitle = rc;
        rcTitle.top = 24;
        rcTitle.bottom = rcTitle.top + 60;

        UINT fmt = DT_SINGLELINE | DT_VCENTER;
        if (g_isWide) {
            rcTitle.left += kMargin; // margen cuando es ancha
            fmt |= DT_LEFT;
        }
        else {
            fmt |= DT_CENTER;        // centrado cuando es chica
        }

        DrawTextW(hdc, kHeadline, -1, &rcTitle, fmt);

        // Depuración: W × H en esquina superior-izquierda
        wchar_t dbg[64];
        wsprintfW(dbg, L"%d x %d", g_cx, g_cy);
        TextOutW(hdc, 8, 8, dbg, lstrlenW(dbg));

        if (old) SelectObject(hdc, old);

        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        if (g_hFontTitle) { DeleteObject(g_hFontTitle); g_hFontTitle = nullptr; }
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

// Controlador de mensajes del cuadro Acerca de.
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
