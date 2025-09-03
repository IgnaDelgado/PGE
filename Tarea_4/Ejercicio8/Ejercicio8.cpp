

#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#pragma execution_character_set("utf-8")

#include <windows.h>
#include <string>
#include <sstream>
#include <cwctype>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static std::wstring g_typed;        
static int          g_clicks = 0;   
static int          g_cx = 0, g_cy = 0; 

// Recursos GDI (presentación)
static HFONT  g_fontTitle = nullptr;
static HFONT  g_fontPanel = nullptr;
static HPEN   g_penPanel = nullptr;
static HBRUSH g_brPanel1 = nullptr;
static HBRUSH g_brPanel2 = nullptr;
static HBRUSH g_brPanel3 = nullptr;

// Helper: pinta un panel con fondo, borde y rótulo
void DrawPanelFrame(HDC hdc, const RECT& r, HBRUSH bg, LPCWSTR titulo) {
    FillRect(hdc, &r, bg);
    HPEN   oldPen = (HPEN)SelectObject(hdc, g_penPanel);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, r.left, r.top, r.right, r.bottom);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);

    HFONT old = (HFONT)SelectObject(hdc, g_fontPanel);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(25, 25, 25));
    RECT rt = r; rt.bottom = rt.top + 26; rt.left += 10;
    DrawTextW(hdc, titulo, -1, &rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
    SelectObject(hdc, old);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nCmdShow) {
    const wchar_t* kClass = L"PanelMostradorClase";

    WNDCLASSEXW wc{}; wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = kClass;
    RegisterClassExW(&wc);

    HWND hWnd = CreateWindowExW(
        0, kClass,
        L"Integración mini \u2013 Panel de mostrador",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 960, 600,
        nullptr, nullptr, hInst, nullptr);
    if (!hWnd) return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_CREATE: {
        // Crear recursos de presentación UNA sola vez
        g_fontTitle = CreateFontW(
            30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        g_fontPanel = CreateFontW(
            18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        g_penPanel = CreatePen(PS_SOLID, 1, RGB(185, 185, 185));
        g_brPanel1 = CreateSolidBrush(RGB(255, 245, 230)); // teclado
        g_brPanel2 = CreateSolidBrush(RGB(235, 248, 255)); // clics
        g_brPanel3 = CreateSolidBrush(RGB(237, 242, 237)); // tamaño
        return 0;
    }

    case WM_CHAR: {
        wchar_t ch = (wchar_t)w;
        if (ch == L'\b') { if (!g_typed.empty()) g_typed.pop_back(); }
        else if (iswprint(ch)) g_typed.push_back(ch);   // sólo imprimibles
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;
    }

    case WM_LBUTTONDOWN:
        ++g_clicks; InvalidateRect(hWnd, nullptr, TRUE); return 0;

    case WM_SIZE:
        g_cx = LOWORD(l); g_cy = HIWORD(l);
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps; HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc; GetClientRect(hWnd, &rc);
        SetBkMode(hdc, TRANSPARENT);

        // Cabecera
        HFONT old = (HFONT)SelectObject(hdc, g_fontTitle);
        SetTextColor(hdc, RGB(20, 60, 120));
        RECT hdr = rc; hdr.bottom = hdr.top + 60; hdr.left += 16; hdr.top += 8;
        DrawTextW(hdc, L"Panel de mostrador, Win32", -1, &hdr,
            DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
        SelectObject(hdc, old);
        SetTextColor(hdc, RGB(0, 0, 0));

        // Layout general (márgenes)
        const int M = 16;
        RECT area = rc; area.top += 72; area.left += M; area.right -= M; area.bottom -= M;

        // Panel 1: Teclado
        RECT p1 = area; p1.right = area.left + (area.right - area.left) / 2 - 8; p1.bottom = area.top + 190;
        DrawPanelFrame(hdc, p1, g_brPanel1, L"Nombre (teclado), WM_CHAR");
        HFONT oldP = (HFONT)SelectObject(hdc, g_fontPanel);
        RECT t1 = p1; t1.left += 12; t1.top += 40; t1.right -= 12;
        std::wstring linea1 = L"Texto ingresado: " + (g_typed.empty() ? L"(vac\u00EDo)" : g_typed);
        DrawTextW(hdc, linea1.c_str(), -1, &t1, DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);

        // Panel 2: Clics
        RECT p2 = p1; p2.left = p1.right + 16; p2.right = area.right;
        DrawPanelFrame(hdc, p2, g_brPanel2, L"Entradas (clicks), WM_LBUTTONDOWN");
        RECT t2 = p2; t2.left += 12; t2.top += 40;
        std::wstringstream s2; s2 << L"Clicks izquierdos: " << g_clicks;
        DrawTextW(hdc, s2.str().c_str(), -1, &t2, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);

        // Panel 3: Tamaño
        RECT p3 = area; p3.top = p1.bottom + 16; p3.bottom = area.bottom;
        DrawPanelFrame(hdc, p3, g_brPanel3, L"Tama\u00F1o (W\u00D7H),  WM_SIZE");
        RECT t3 = p3; t3.left += 12; t3.top += 40;
        std::wstringstream s3; s3 << L"\u00C1rea cliente: " << g_cx << L" \u00D7 " << g_cy << L" px";
        DrawTextW(hdc, s3.str().c_str(), -1, &t3, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);

        SelectObject(hdc, oldP);
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        if (g_fontTitle) { DeleteObject(g_fontTitle);  g_fontTitle = nullptr; }
        if (g_fontPanel) { DeleteObject(g_fontPanel);  g_fontPanel = nullptr; }
        if (g_penPanel) { DeleteObject(g_penPanel);   g_penPanel = nullptr; }
        if (g_brPanel1) { DeleteObject(g_brPanel1);   g_brPanel1 = nullptr; }
        if (g_brPanel2) { DeleteObject(g_brPanel2);   g_brPanel2 = nullptr; }
        if (g_brPanel3) { DeleteObject(g_brPanel3);   g_brPanel3 = nullptr; }
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hWnd, m, w, l);
}
