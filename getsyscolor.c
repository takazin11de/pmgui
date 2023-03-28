#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  HDC hdc;
  PAINTSTRUCT ps;
  int iCount, iWidth = 0;
  static const int syscolor[] = {COLOR_3DDKSHADOW, COLOR_3DFACE,        COLOR_3DHILIGHT,      COLOR_3DLIGHT,
                                 COLOR_3DSHADOW,   COLOR_ACTIVEBORDER,  COLOR_ACTIVECAPTION,  COLOR_APPWORKSPACE,
                                 COLOR_BACKGROUND, COLOR_BTNTEXT,       COLOR_CAPTIONTEXT,    COLOR_GRAYTEXT,
                                 COLOR_HIGHLIGHT,  COLOR_HIGHLIGHTTEXT, COLOR_INACTIVEBORDER, COLOR_INACTIVECAPTION,
                                 COLOR_INFOBK,     COLOR_INFOTEXT,      COLOR_MENU,           COLOR_MENUTEXT,
                                 COLOR_SCROLLBAR,  COLOR_WINDOW,        COLOR_WINDOWFRAME,    COLOR_WINDOWTEXT};

  switch (msg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      SelectObject(hdc, GetStockObject(NULL_PEN));
      for (iCount = 0; iCount < sizeof(syscolor) / sizeof(int); iCount++) {
        SelectObject(hdc, CreateSolidBrush(GetSysColor(syscolor[iCount])));
        Rectangle(hdc, iWidth, 0, iWidth + 20, 200);
        DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
        iWidth += 20;
      }
      EndPaint(hwnd, &ps);
      return 0;
  }
  return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
  HWND hwnd;
  MSG msg;
  WNDCLASS winc;

  winc.style = CS_HREDRAW | CS_VREDRAW;
  winc.lpfnWndProc = WndProc;
  winc.cbClsExtra = winc.cbWndExtra = 0;
  winc.hInstance = hInstance;
  winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  winc.hCursor = LoadCursor(NULL, IDC_ARROW);
  winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  winc.lpszMenuName = NULL;
  winc.lpszClassName = TEXT("KITTY");

  if (!RegisterClass(&winc)) return -1;

  hwnd = CreateWindow(TEXT("KITTY"), TEXT("Kitty on your lap"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT,
                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

  if (hwnd == NULL) return -1;

  while (GetMessage(&msg, NULL, 0, 0)) DispatchMessage(&msg);
  return msg.wParam;
}