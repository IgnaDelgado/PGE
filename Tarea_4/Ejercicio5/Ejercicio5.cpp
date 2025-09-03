

#define UNICODE
#define _UNICODE
#include "framework.h"
#include "Ejercicio5.h"
#include <string>


static int   g_piso = 0;              
static HFONT g_hFontBtns = nullptr;   


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
static void         CalcButtonRects(HWND hWnd, RECT out[3], RECT* areaTexto);


#define MAX_LOADSTRING 100
HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EJERCICIO5, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EJERCICIO5));
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
    wcex.lpfnWndProc = WndProc;                               // nuestro callback
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EJERCICIO5));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);            // fondo blanco de la ventana
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EJERCICIO5);      // usa el menú del .rc si existe
    wcex.lpszClassName = szWindowClass;                         // nombre de clase a usar en CreateWindowW
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}



BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    HWND hWnd = CreateWindowW(
        szWindowClass,                  // clase registrada arriba
        szTitle,                        // título (recurso)
        WS_OVERLAPPEDWINDOW,            // estilo clásico
        CW_USEDEFAULT, 0, 860, 360,     // tamaño inicial cómodo para ver 3 botones
        nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}



static void CalcButtonRects(HWND hWnd, RECT out[3], RECT* areaTexto)
{
    RECT rc; GetClientRect(hWnd, &rc);

    const int margen = 16;   // margen general
    const int sep = 8;    // separación horizontal entre botones

    int W = rc.right - rc.left;
    int H = rc.bottom - rc.top;

    // Faja superior para los 3 botones
    int top = margen;
    int altoBoton = max(60, min(120, H / 3)); // alto razonable (crece un poco con la ventana)

    int anchoDisponible = W - 2 * margen - 2 * sep;
    int anchoBoton = anchoDisponible / 3;

    for (int i = 0; i < 3; ++i) {
        int left = margen + i * (anchoBoton + sep);
        out[i].left = left;
        out[i].top = top;
        out[i].right = left + anchoBoton;
        out[i].bottom = top + altoBoton;
    }

    if (areaTexto) {
        *areaTexto = rc;
        areaTexto->top = top + altoBoton + margen; // debajo de los botones
        areaTexto->left += margen;
        areaTexto->right -= margen;
    }
}


// Procedimiento de ventana: maneja los mensajes del sistema
//  - WM_CREATE: crear recursos (fuente)
//  - WM_LBUTTONDOWN: detectar clic y actualizar g_piso
//  - WM_SIZE: invalidar para repintar layout
//  - WM_PAINT: TODO el dibujo (nunca fuera de acá)
//  - WM_DESTROY: liberar recursos y terminar app
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // Crear la fuente UNA sola vez (evita pérdidas y es más eficiente).
        if (!g_hFontBtns) {
            g_hFontBtns = CreateFontW(
                -24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        }
        return 0;

    case WM_CHAR:
        // Soporte opcional: si el usuario presiona '1', '2' o '3', cambia el piso.
        if (wParam >= L'1' && wParam <= L'3') {
            g_piso = (int)(wParam - L'0');
            InvalidateRect(hWnd, nullptr, TRUE);   // pedir repintado
        }
        return 0;

    case WM_LBUTTONDOWN:
    {
        // 1) Obtener el punto del clic
		POINT p = { LOWORD(lParam), HIWORD(lParam) };

        // 2) Calcular rectángulos actuales
        RECT rBtns[3]; RECT dummy{};
        CalcButtonRects(hWnd, rBtns, &dummy);

        // 3) ¿En qué rectángulo cayó el clic?
        for (int i = 0; i < 3; ++i) {
            if (PtInRect(&rBtns[i], p)) {
                g_piso = i + 1;                      // registrar “último piso solicitado”
                InvalidateRect(hWnd, nullptr, TRUE);  // repintar (feedback visual)
                break;
            }
        }
        return 0;
    }

    case WM_SIZE:
        // Cuando cambia tamaño, simplemente invalidamos para que WM_PAINT re-dibuje
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;

    case WM_PAINT:
    {
        // TODO: todo el dibujo va acá (pizarra: nunca dibujar fuera de WM_PAINT)
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);

        // Calcular layout actual
        RECT rBtns[3]; RECT rTexto{};
        CalcButtonRects(hWnd, rBtns, &rTexto);

        // Seleccionar la fuente del ejercicio
        HFONT old = nullptr;
        if (g_hFontBtns) old = (HFONT)SelectObject(hdc, g_hFontBtns);

        // Dibujar los 3 “botones” (1..3)
        for (int i = 0; i < 3; ++i) {
            // Fondo distinto si es el último piso solicitado
            HBRUSH brSel = CreateSolidBrush(RGB(200, 230, 255));      
            HBRUSH br = (g_piso == (i + 1))
                ? brSel
                : (HBRUSH)GetStockObject(LTGRAY_BRUSH);

            FillRect(hdc, &rBtns[i], br);
            if (br == brSel) DeleteObject(brSel); // sólo libero si lo creé yo

            // Borde negro
            FrameRect(hdc, &rBtns[i], (HBRUSH)GetStockObject(BLACK_BRUSH));

            // Número centrado dentro del rectángulo
            wchar_t num[2] = { wchar_t(L'1' + i), 0 };
            RECT r = rBtns[i];
            DrawTextW(hdc, num, 1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }

        // Texto inferior: “Último piso: X”
        wchar_t linea[64];
        if (g_piso == 0)
            lstrcpyW(linea, L"Ultimo piso: 0");
        else
            wsprintfW(linea, L"Ultimo piso: %d", g_piso);

        DrawTextW(hdc, linea, -1, &rTexto, DT_LEFT | DT_TOP | DT_NOPREFIX);

        // Restaurar fuente y cerrar ciclo de pintura
        if (old) SelectObject(hdc, old);
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        // Liberar recursos creados con Create*
        if (g_hFontBtns) { DeleteObject(g_hFontBtns); g_hFontBtns = nullptr; }
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
