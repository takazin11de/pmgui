#include "pmgui.h"

int main() {
  PmGui_Init();

  PmWidget window = PmWidget_Init(pmw_window);
  PmWidget_SetText(window, "Window Title");
  PmWidget_SetGeometry(window, 0, 0, 900, 700);
  PmWidget_Show(window);

  PmWidget canvas = PmWidget_Init(pmw_canvas);
  PmWidget_SetParent(canvas, window);
  PmWidget_SetGeometry(canvas, 0, 0, 800, 600);
  PmCanvas_DrawLine(canvas, 10, 10, 110, 110);

  PmCanvas_SetStrokeSize(canvas, 5);

  int x, y;
  x = 10 + 100 + 10;
  y = 10;
  PmCanvas_DrawArc(canvas, x, y + 20, x + 100, y + 100, x + 50, y, x, y + 60);
  x = 10 + 100 + 10 + 100 + 10;
  y = 60;
  PmCanvas_Bezier(canvas, x, y, x + 66, y - 50, x + 33, y + 50, x + 100, y);

  PmCanvas_SetStrokeColor(canvas, 255, 0, 0);
  PmCanvas_SetStrokeSize(canvas, 1);
  PmCanvas_DrawLine(canvas, x + 66, y - 50, x, y);
  PmCanvas_DrawLine(canvas, x + 33, y + 50, x + 100, y);

  PmCanvas_SetFillColor(canvas, 255, 128, 64);
  PmCanvas_SetStrokeColor(canvas, 0, 128, 255);
  PmCanvas_SetStrokeSize(canvas, 5);

  x = 10;
  y = 110;
  PmCanvas_DrawRect(canvas, x, y, x + 100, y + 100);
  x = 10 + 100 + 10;
  y = 110;
  PmCanvas_DrawRectStroke(canvas, x, y, x + 100, y + 80);
  x = 10 + 100 + 10 + 100 + 10;
  y = 110;
  PmCanvas_DrawRectFill(canvas, x, y, x + 80, y + 100);

  PmCanvas_SetFillColor(canvas, 0, 128, 255);
  PmCanvas_SetStrokeColor(canvas, 255, 128, 64);
  PmCanvas_SetStrokeSize(canvas, 10);

  x = 10;
  y = 110 + 100 + 10;
  PmCanvas_EllipseRect(canvas, x, y, x + 100, y + 100);
  x = 10 + 100 + 10;
  y = 110 + 100 + 10;
  PmCanvas_DrawEllipseStroke(canvas, x, y, x + 100, y + 80);
  x = 10 + 100 + 10 + 100 + 10;
  y = 110 + 100 + 10;
  PmCanvas_DrawEllipseFill(canvas, x, y, x + 80, y + 100);

  PmCanvas_SetTextColor(canvas, 255, 0, 0);
  PmCanvas_SetStrokeColor(canvas, 255, 0, 0);
  PmCanvas_SetStrokeSize(canvas, 1);

  PmCanvas_SetFont(canvas, "ＭＳ ゴシック", 24, 0, 0, 0, 0);
  PmCanvas_DrawTextCalc(canvas, "abcdABCDあいうえお\nああああ");
  PmCanvas_DrawRectStroke(canvas, 10, 400, 10 + PmCanvas_GetTextX(canvas), 400 + PmCanvas_GetTextY(canvas));
  PmCanvas_DrawText(canvas, "abcdABCDあいうえお\nああああ", 10, 400);

  PmCanvas_SetTextColor(canvas, 0, 128, 255);
  PmCanvas_SetFont(canvas, "ＭＳ 明朝", 48, pmfw_bold, 1, 1, 1);
  PmCanvas_DrawText(canvas, "いろはにほへと", 400, 400);

  PmWidget_Show(canvas);

  return PmGui_ListenEvent();
}
