// Ejercicio6.cpp : Caja registradora — “Sólo números”
// Estructura estilo cátedra (framework.h / Ejercicio6.h)

#define UNICODE
#define _UNICODE
#include "framework.h"
#include "Ejercicio6.h"

#include <string>
#include <cwctype>   // iswdigit


#define MAX_LOADSTRING 100
HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            


static std::wstring g_importe; 
static HFONT g_hFont = nullptr; // fuente única para rotular

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Cargar cadenas desde recursos (.rc)
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EJERCICIO6, szWindowClass, MAX_LOADSTRING);

    // Registrar clase de ventana
    MyRegisterClass(hInstance);

    // Crear y mostrar ventana
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    // Bucle de mensajes (despachador)
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EJERCICIO6));
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EJERCICIO6));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EJERCICIO6);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    HWND hWnd = CreateWindowW(
        szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 520, 200, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

// WndProc: acá va la lógica del ejercicio
//  - WM_CREATE: crear fuente
//  - WM_CHAR: filtrar entrada (0–9, coma/punto, Backspace)
//  - WM_PAINT: mostrar el importe
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // Fuente única para todo el texto (crear una vez)
        if (!g_hFont) {
            g_hFont = CreateFontW(
                -24, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        }
        return 0;

    case WM_CHAR:
    {
        // ------------- FILTRO DE TECLAS (según consigna) -------------
        wchar_t ch = (wchar_t)wParam;

        if (ch == L'\b') {
            // Backspace → borrar último caracter si existe
            if (!g_importe.empty()) g_importe.pop_back();
        }
        else if (iswdigit(ch)) {
            // Dígito 0..9 → aceptar
            g_importe.push_back(ch);
        }
        else if (ch == L',' || ch == L'.') {
            // Permitir UNA sola coma (o punto normalizado a coma)
            if (g_importe.find(L',') == std::wstring::npos) {
                g_importe.push_back(L','); // normalizo '.' → ','
            }
            // si ya hay una coma, se ignora
        }
        // Resto de teclas: RECHAZAR → no cambiamos g_importe

        InvalidateRect(hWnd, nullptr, TRUE); // repintar para ver el cambio
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);

        HFONT old = nullptr;
        if (g_hFont) old = (HFONT)SelectObject(hdc, g_hFont);

        RECT rc; GetClientRect(hWnd, &rc);
        RECT r = rc; r.left += 16; r.top += 32;

        // Rotulito en negrita suave
        const wchar_t* titulo = L"Caja registradora, Solo numeros";
        DrawTextW(hdc, titulo, -1, &r, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX);

        r.top += 40;
        // Línea principal: “Importe: 123,45”
        std::wstring linea = L"Importe: ";
        linea += (g_importe.empty() ? L"(vacio)" : g_importe);
        DrawTextW(hdc, linea.c_str(), -1, &r, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX);

        // Ayuda (opcional)
        r.top += 36;
        const wchar_t* help =
            L"Teclas validas: 0 al 9, una coma (o punto), Backspace borra.\n"
            L"Otras teclas se ignoran.";
        DrawTextW(hdc, help, -1, &r, DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);

        if (old) SelectObject(hdc, old);
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        if (g_hFont) { DeleteObject(g_hFont); g_hFont = nullptr; }
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
