#ifndef PMWIDGET_H
#define PMWIDGET_H
#ifdef __cplusplus
extern "C" {
#endif

#define UNICODE
#include "pmgui.h"
#include "pmgui_env.h"

PmWidget PmWidget_Init(int widgets_type);
int PmWidget_SetParent(PmWidget self, PmWidget parent);
int PmWidget_SetPos(PmWidget self, int pos_x, int pos_y);
int PmWidget_SetSize(PmWidget self, int size_x, int size_y);
int PmWidget_SetGeometry(PmWidget self, int pos_x, int pos_y, int size_x, int size_y);
int PmWidget_SetText(PmWidget self, char* text);
int PmWidget_Disable(PmWidget self);
int PmWidget_Enable(PmWidget self);
int PmWidget_Show(PmWidget self);
int PmWidget_Hide(PmWidget self);
char* PmWidget_GetText(PmWidget self);

int PmWidget_GetCheckState(PmWidget self);
int PmWidget_SetCheck(PmWidget self, int state);

int PmWidget_GetListboxSelect(PmWidget self);
int PmWidget_AddListbox(PmWidget self, char* text);

int PmWidget_GetComboboxSelect(PmWidget self);
char* PmWidget_GetComboboxSelectText(PmWidget self);
int PmWidget_AddCombobox(PmWidget self, char* text);
int PmWidget_SetComboboxSelect(PmWidget self, int index);

int PmCanvas_SetStrokeColor(PmWidget self, int red, int green, int blue);
int PmCanvas_SetStrokeSize(PmWidget self, int size);
int PmCanvas_SetFillColor(PmWidget self, int red, int green, int blue);
int PmCanvas_SetTextColor(PmWidget self, int red, int green, int blue);
int PmCanvas_SetFont(PmWidget self, char* font_name, int size, int weight, int italic, int underline, int strikeout);

int PmCanvas_DrawLine(PmWidget self, int x1, int y1, int x2, int y2);
int PmCanvas_DrawArc(PmWidget self, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
int PmCanvas_Bezier(PmWidget self, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

int PmCanvas_DrawRect(PmWidget self, int x1, int y1, int x2, int y2);
int PmCanvas_DrawRectStroke(PmWidget self, int x1, int y1, int x2, int y2);
int PmCanvas_DrawRectFill(PmWidget self, int x1, int y1, int x2, int y2);

int PmCanvas_EllipseRect(PmWidget self, int x1, int y1, int x2, int y2);
int PmCanvas_DrawEllipseStroke(PmWidget self, int x1, int y1, int x2, int y2);
int PmCanvas_DrawEllipseFill(PmWidget self, int x1, int y1, int x2, int y2);

int PmCanvas_DrawText(PmWidget self, char* text, int x1, int y1);
int PmCanvas_DrawTextCalc(PmWidget self, char* text);
int PmCanvas_GetTextX(PmWidget self);
int PmCanvas_GetTextY(PmWidget self);

int PmCanvas_Update(PmWidget self);

#ifdef __cplusplus
}
#endif
#endif
