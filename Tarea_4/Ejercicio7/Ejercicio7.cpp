

#define UNICODE
#define _UNICODE
#include "framework.h"
#include "Ejercicio7.h"
#include <windowsx.h> // Agregado para las macros GET_X_LPARAM y GET_Y_LPARAM
#include <deque>
#include <string>
#include <sstream>
#include <cwctype>   // iswprint


#define MAX_LOADSTRING 100
HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            


static std::deque<std::wstring> g_log;    
static const size_t kMaxItems = 10;       
static HFONT g_hFontMono = nullptr;       


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// Agrega una línea al log (circular) y fuerza repintado
static void PushEvent(HWND hWnd, const std::wstring& s) {
    g_log.push_front(s);
    if (g_log.size() > kMaxItems) g_log.pop_back();
    InvalidateRect(hWnd, nullptr, TRUE); // actualizar pantalla
}

// Formatea eventos comunes
static std::wstring FormatChar(wchar_t ch) {
    // Mostrar backspace, tab, enter, etc. con “escapes”; el resto literal
    switch (ch) {
    case L'\b': return L"WM_CHAR '\\b' (Backspace)";
    case L'\t': return L"WM_CHAR '\\t' (Tab)";
    case L'\r': return L"WM_CHAR '\\r' (Enter)";
    case L'\n': return L"WM_CHAR '\\n'";
    default: {
        std::wstringstream ss;
        if (iswprint(ch))
            ss << L"WM_CHAR '" << ch << L"'";
        else
            ss << L"WM_CHAR (0x" << std::hex << (unsigned)ch << L")";
        return ss.str();
    }
    }
}

static std::wstring FormatClick(LPARAM lParam) {
    int x = GET_X_LPARAM(lParam);
    int y = GET_Y_LPARAM(lParam);
    std::wstringstream ss;
    ss << L"WM_LBUTTONDOWN  (x=" << x << L", y=" << y << L")";
    return ss.str();
}

static std::wstring FormatSize(LPARAM lParam) {
    int w = LOWORD(lParam);
    int h = HIWORD(lParam);
    std::wstringstream ss;
    ss << L"WM_SIZE  " << w << L"\u00D7" << h; // ×
    return ss.str();
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EJERCICIO7, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EJERCICIO7));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EJERCICIO7));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EJERCICIO7);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    HWND hWnd = CreateWindowW(
        szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 700, 420, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

// WndProc: registra eventos y los dibuja
//  - WM_CREATE: crear fuente y log inicial
//  - WM_CHAR / WM_LBUTTONDOWN / WM_SIZE: registrar cada evento
//  - WM_PAINT: dibujar la tabla (más reciente arriba)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // Fuente monoespaciada (queda prolija en “tabla”)
        if (!g_hFontMono) {
            g_hFontMono = CreateFontW(
                -20, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");
        }
        PushEvent(hWnd, L"(inicio) listo para registrar eventos...");
        return 0;

    case WM_CHAR:
        PushEvent(hWnd, FormatChar((wchar_t)wParam));
        return 0;

    case WM_LBUTTONDOWN:
        PushEvent(hWnd, FormatClick(lParam));
        return 0;

    case WM_SIZE:
        PushEvent(hWnd, FormatSize(lParam));
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);

        RECT rc; GetClientRect(hWnd, &rc);
        RECT r = rc; r.left += 16; r.top += 16; r.right -= 16; r.bottom -= 16;

        // Título
        HFONT old = nullptr; if (g_hFontMono) old = (HFONT)SelectObject(hdc, g_hFontMono);
        DrawTextW(hdc, L"Ultimos eventos (mas reciente arriba):", -1, &r,
            DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX);

        // Lista: cada item una línea
        r.top += 32;
        for (size_t i = 0; i < g_log.size(); ++i) {
            std::wstring const& line = g_log[i];
            DrawTextW(hdc, line.c_str(), -1, &r,
                DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX);
            r.top += 26; // avance de línea
        }

        if (old) SelectObject(hdc, old);
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        if (g_hFontMono) { DeleteObject(g_hFontMono); g_hFontMono = nullptr; }
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
