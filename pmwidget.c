
#include <stdio.h>
#include <stdlib.h>

#include "pmconv.h"
#include "pmgui.h"
#include "pmgui_env.h"

void create_pen(PmWidget self);
void create_brush(PmWidget self);
void creat_font(PmWidget self);

void get_desktop_size(PmSize* size);

int on_window_paint(PmEvent* event) {
  HDC hdc;
  PAINTSTRUCT ps;
  RECT rect;
  PmWidget widget = event->widget;

  GetClientRect(widget->hwnd, &rect);

  hdc = BeginPaint(widget->hwnd, &ps);
  SelectObject(hdc, widget->brush);
  PatBlt(hdc, 0, 0, rect.right, rect.bottom, PATCOPY);

  EndPaint(widget->hwnd, &ps);
  return 0;
}

int on_canvas_paint(PmEvent* event) {
  HDC hdc;
  PAINTSTRUCT ps;
  RECT rect;
  PmWidget widget = event->widget;

  GetClientRect(widget->hwnd, &rect);

  hdc = BeginPaint(widget->hwnd, &ps);

  BitBlt(hdc, 0, 0, rect.right, rect.bottom, widget->dc, 0, 0, SRCCOPY);

  EndPaint(widget->hwnd, &ps);
  return 0;
}

PmWidget PmWidget_Init(int widgets_type) {
  HDC hdc;
  PmSize size;
  PmWidget self = malloc(sizeof(PmWidget_Impl));
  self->type = widgets_type;
  self->hwnd = NULL;
  self->text = malloc(1);
  self->text[0] = 0;
  self->size_x = 0;
  self->size_y = 0;
  self->pos_x = 0;
  self->pos_y = 0;
  self->data = NULL;
  self->utf16_text = utf8to16(NULL, self->text);
  self->timer_id = 0;

  self->dc = NULL;
  self->bitmap = NULL;

  self->pen_red = 0;
  self->pen_green = 0;
  self->pen_blue = 0;
  self->pen_size = 1;
  self->f_pen = 1;
  self->pen = NULL;

  self->brush_red = 255;
  self->brush_green = 255;
  self->brush_blue = 255;
  self->f_brush = 1;
  self->brush = NULL;

  self->hfont = NULL;
  self->font_size = 16;
  self->font_weight = 0;
  self->font_is_italic = 0;
  self->font_is_underline = 0;
  self->font_is_strikeout = 0;
  self->font_name = NULL;
  self->font_name_utf16 = NULL;
  self->f_font = 1;
  self->text_size_x = 0;
  self->text_size_y = 0;

  switch (widgets_type) {
    case pmw_window:
      self->size_x = 800;
      self->size_y = 600;
      self->brush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
      PmEvent_Bind(pme_env_paint, on_window_paint, self, NULL);
      self->hwnd = CreateWindowW(pmgui.wnd_class_name, self->utf16_text, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                 CW_USEDEFAULT, self->size_x, self->size_y, NULL, NULL, pmgui.instance, NULL);
      if (self->hwnd == NULL) {
      }

      break;

    case pmw_label:
      self->size_x = 100;
      self->size_y = 30;
      self->hwnd = CreateWindowW(pmgui.str_label, self->utf16_text, WS_CHILD | WS_VISIBLE, self->pos_x, self->pos_y,
                                 self->size_x, self->size_y, pmgui.hidden_hwnd, NULL, pmgui.instance, NULL);
      if (self->hwnd == NULL) {
      }
      self->hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SendMessage(self->hwnd, WM_SETFONT, (WPARAM)(self->hfont), MAKELPARAM(FALSE, 0));
      break;

    case pmw_button:
      self->size_x = 100;
      self->size_y = 30;
      self->hwnd =
          CreateWindowW(pmgui.str_button, self->utf16_text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, self->pos_x,
                        self->pos_y, self->size_x, self->size_y, pmgui.hidden_hwnd, NULL, pmgui.instance, NULL);
      if (self->hwnd == NULL) {
      }
      self->hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SendMessage(self->hwnd, WM_SETFONT, (WPARAM)(self->hfont), MAKELPARAM(FALSE, 0));
      break;

    case pmw_checkbox:
      self->size_x = 100;
      self->size_y = 30;
      self->hwnd =
          CreateWindowW(pmgui.str_button, self->utf16_text, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, self->pos_x,
                        self->pos_y, self->size_x, self->size_y, pmgui.hidden_hwnd, NULL, pmgui.instance, NULL);
      if (self->hwnd == NULL) {
      }
      self->hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SendMessage(self->hwnd, WM_SETFONT, (WPARAM)(self->hfont), MAKELPARAM(FALSE, 0));
      break;

    case pmw_input:
      self->size_x = 100;
      self->size_y = 30;
      self->hwnd =
          CreateWindowW(pmgui.str_input, self->utf16_text, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, self->pos_x,
                        self->pos_y, self->size_x, self->size_y, pmgui.hidden_hwnd, NULL, pmgui.instance, NULL);
      if (self->hwnd == NULL) {
      }
      self->hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SendMessage(self->hwnd, WM_SETFONT, (WPARAM)(self->hfont), MAKELPARAM(FALSE, 0));
      break;

    case pmw_listbox:
      self->size_x = 100;
      self->size_y = 100;
      self->hwnd =
          CreateWindowW(pmgui.str_listbox, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY, self->pos_x,
                        self->pos_y, self->size_x, self->size_y, pmgui.hidden_hwnd, NULL, pmgui.instance, NULL);
      if (self->hwnd == NULL) {
      }
      self->hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SendMessage(self->hwnd, WM_SETFONT, (WPARAM)(self->hfont), MAKELPARAM(FALSE, 0));
      break;

    case pmw_combobox:
      self->size_x = 100;
      self->size_y = 100;
      self->hwnd =
          CreateWindowW(pmgui.str_combobox, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN, self->pos_x,
                        self->pos_y, self->size_x, self->size_y, pmgui.hidden_hwnd, NULL, pmgui.instance, NULL);
      if (self->hwnd == NULL) {
      }
      self->hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SendMessage(self->hwnd, WM_SETFONT, (WPARAM)(self->hfont), MAKELPARAM(FALSE, 0));
      break;

    case pmw_canvas:
      self->size_x = 100;
      self->size_y = 100;

      get_desktop_size(&size);

      PmEvent_Bind(pme_env_paint, on_canvas_paint, self, NULL);

      self->hwnd =
          CreateWindowW(pmgui.wnd_class_name, self->utf16_text, WS_CHILD | WS_VISIBLE, self->pos_x, self->pos_y,
                        self->size_x, self->size_y, pmgui.hidden_hwnd, NULL, pmgui.instance, NULL);
      if (self->hwnd == NULL) {
      }

      hdc = GetDC(self->hwnd);
      if (hdc == NULL) {
        printf("err1\n");
      }
      self->dc = CreateCompatibleDC(hdc);

      if (self->dc == NULL) {
        printf("err2\n");
      }
      self->bitmap = CreateCompatibleBitmap(hdc, size.x, size.y);
      if (self->bitmap == NULL) {
        printf("err3\n");
      }

      ReleaseDC(self->hwnd, hdc);

      SelectObject(self->dc, self->bitmap);
      SelectObject(self->dc, GetStockObject(NULL_PEN));
      PatBlt(self->dc, 0, 0, size.x, size.y, WHITENESS);

      printf("destop size:%d,%d", size.x, size.y);

      break;
    case pmw_timer:
      break;

    default:
      self->type = pmw_end;
  }
  return self;
}

int PmWidget_SetParent(PmWidget self, PmWidget parent) {
  switch (self->type) {
    case pmw_label:
    case pmw_window:
    case pmw_button:
    case pmw_checkbox:
    case pmw_listbox:
    case pmw_combobox:
    case pmw_input:
    case pmw_canvas:
      self->parent = parent;
      HWND hwnd = SetParent(self->hwnd, parent->hwnd);
      UpdateWindow(self->hwnd);
      break;

    default:;
  }
  return 0;
}

int PmWidget_SetPos(PmWidget self, int pos_x, int pos_y) {
  switch (self->type) {
    case pmw_label:
    case pmw_window:
    case pmw_button:
    case pmw_checkbox:
    case pmw_listbox:
    case pmw_combobox:
    case pmw_input:
    case pmw_canvas:
      self->pos_x = pos_x;
      self->pos_y = pos_y;
      SetWindowPos(self->hwnd, NULL, self->pos_x, self->pos_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
      break;

    default:;
  }
  return 0;
}

int PmWidget_SetSize(PmWidget self, int size_x, int size_y) {
  switch (self->type) {
    case pmw_label:
    case pmw_window:
    case pmw_button:
    case pmw_checkbox:
    case pmw_listbox:
    case pmw_combobox:
    case pmw_input:
    case pmw_canvas:
      self->size_x = size_x;
      self->size_y = size_y;
      SetWindowPos(self->hwnd, NULL, 0, 0, self->size_x, self->size_y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
      break;

    default:;
  }
  return 0;
}

int PmWidget_SetGeometry(PmWidget self, int pos_x, int pos_y, int size_x, int size_y) {
  switch (self->type) {
    case pmw_label:
    case pmw_window:
    case pmw_button:
    case pmw_checkbox:
    case pmw_listbox:
    case pmw_combobox:
    case pmw_input:
    case pmw_canvas:
      self->pos_x = pos_x;
      self->pos_y = pos_y;
      self->size_x = size_x;
      self->size_y = size_y;
      SetWindowPos(self->hwnd, NULL, self->pos_x, self->pos_y, self->size_x, self->size_y,
                   SWP_NOZORDER | SWP_NOACTIVATE);
      break;

    default:;
  }
  return 0;
}

int PmWidget_SetText(PmWidget self, char* text) {
  char* _text;
  switch (self->type) {
    case pmw_label:
    case pmw_window:
    case pmw_button:
    case pmw_checkbox:
    case pmw_listbox:
    case pmw_combobox:
    case pmw_input:
    case pmw_canvas:
      _text = malloc(strlen(text));
      self->text = strcpy(_text, text);
      self->utf16_text = utf8to16(self->utf16_text, self->text);
      SetWindowTextW(self->hwnd, self->utf16_text);
      break;

    default:;
  }

  return 0;
}

int PmWidget_Show(PmWidget self) {
  switch (self->type) {
    case pmw_label:
    case pmw_window:
    case pmw_button:
    case pmw_checkbox:
    case pmw_listbox:
    case pmw_combobox:
    case pmw_input:
    case pmw_canvas:
      ShowWindow(self->hwnd, SW_SHOW);
      UpdateWindow(self->hwnd);
      break;

    default:;
  }
  return 0;
}

int PmWidget_Hide(PmWidget self) {
  switch (self->type) {
    case pmw_label:
    case pmw_window:
    case pmw_button:
    case pmw_checkbox:
    case pmw_listbox:
    case pmw_combobox:
    case pmw_input:
    case pmw_canvas:
      ShowWindow(self->hwnd, SW_HIDE);
      break;

    default:;
  }
  return 0;
}

int PmWidget_Disable(PmWidget self) {
  switch (self->type) {
    case pmw_label:
    case pmw_window:
    case pmw_button:
    case pmw_checkbox:
    case pmw_input:
    case pmw_listbox:
    case pmw_combobox:
    case pmw_canvas:
      EnableWindow(self->hwnd, 0);
      break;

    default:;
  }
  return 0;
}

int PmWidget_Enable(PmWidget self) {
  switch (self->type) {
    case pmw_label:
    case pmw_window:
    case pmw_button:
    case pmw_checkbox:
    case pmw_listbox:
    case pmw_combobox:
    case pmw_input:
    case pmw_canvas:
      EnableWindow(self->hwnd, 1);
      break;

    default:;
  }
  return 0;
}

char* PmWidget_GetText(PmWidget self) {
  int len;
  switch (self->type) {
    case pmw_combobox:
    case pmw_input:
      len = GetWindowTextLengthW(self->hwnd) + 1;
      free(self->utf16_text);
      self->utf16_text = malloc(sizeof(short) * len);
      GetWindowTextW(self->hwnd, self->utf16_text, len);
      self->text = utf16to8(self->text, self->utf16_text);
      return self->text;

      break;

    default:;
  }
  return 0;
}

int PmWidget_GetCheckState(PmWidget self) {
  switch (self->type) {
    case pmw_checkbox:
      if (BST_CHECKED == SendMessageW(self->hwnd, BM_GETCHECK, 0, 0)) {
        return 1;
      } else {
        return 0;
      }
      break;
    default:;
  }
  return 0;
}
int PmWidget_SetCheck(PmWidget self, int state) {
  switch (self->type) {
    case pmw_checkbox:
      if (state) {
        SendMessageW(self->hwnd, BM_SETCHECK, BST_CHECKED, 0);
      } else {
        SendMessageW(self->hwnd, BM_SETCHECK, BST_UNCHECKED, 0);
      }
      break;
    default:;
  }
  return 0;
}

int PmWidget_GetListboxSelect(PmWidget self) {
  int ret;
  switch (self->type) {
    case pmw_listbox:
      ret = SendMessageW(self->hwnd, LB_GETCURSEL, 0, 0);
      if (LB_ERR == ret) {
        return -1;
      } else {
        return ret;
      }
      break;
    default:;
  }
  return 0;
}

int PmWidget_AddListbox(PmWidget self, char* text) {
  unsigned short* _text;
  switch (self->type) {
    case pmw_listbox:
      _text = utf8to16(NULL, text);
      SendMessageW(self->hwnd, LB_ADDSTRING, 0, (LPARAM)_text);
      free(_text);
      break;
    default:;
  }
  return 0;
}

int PmWidget_GetComboboxSelect(PmWidget self) {
  int ret;
  switch (self->type) {
    case pmw_combobox:
      ret = SendMessageW(self->hwnd, CB_GETCURSEL, 0, 0);
      if (LB_ERR == ret) {
        return -1;
      } else {
        return ret;
      }
      break;
    default:;
  }
  return 0;
}

int PmWidget_SetComboboxSelect(PmWidget self, int index) {
  int ret;
  switch (self->type) {
    case pmw_combobox:
      ret = SendMessageW(self->hwnd, CB_SETCURSEL, index, 0);
      break;
    default:;
  }
  return 0;
}

char* PmWidget_GetComboboxSelectText(PmWidget self) {
  int len, index;
  switch (self->type) {
    case pmw_combobox:
      index = PmWidget_GetComboboxSelect(self);

      if (index == -1) {
        PmWidget_GetText(self);
      } else {
        len = SendMessageW(self->hwnd, CB_GETLBTEXTLEN, index, 0);
        free(self->utf16_text);
        self->utf16_text = malloc(sizeof(short) * len);
        SendMessageW(self->hwnd, CB_GETLBTEXT, index, (LPARAM)self->utf16_text);
        self->text = utf16to8(self->text, self->utf16_text);
      }

      return self->text;
      break;
    default:;
  }
  return 0;
}

int PmWidget_AddCombobox(PmWidget self, char* text) {
  unsigned short* _text;
  switch (self->type) {
    case pmw_combobox:
      _text = utf8to16(NULL, text);
      SendMessageW(self->hwnd, CB_ADDSTRING, 0, (LPARAM)_text);
      free(_text);
      break;
    default:;
  }
  return 0;
}

int PmCanvas_SetStrokeColor(PmWidget self, int red, int green, int blue) {
  if (self->pen_red != red || self->pen_green != green || self->pen_blue != blue) {
    self->pen_red = red;
    self->pen_green = green;
    self->pen_blue = blue;
    self->f_pen = 1;
  }
  return 0;
}
int PmCanvas_SetStrokeSize(PmWidget self, int size) {
  if (self->pen_size != size) {
    self->pen_size = size;
    self->f_pen = 1;
  }
  return 0;
}
int PmCanvas_SetFillColor(PmWidget self, int red, int green, int blue) {
  if (self->brush_red != red || self->brush_green != green || self->brush_blue != blue) {
    self->brush_red = red;
    self->brush_green = green;
    self->brush_blue = blue;
    self->f_brush = 1;
  }
  return 0;
}

int PmCanvas_SetTextColor(PmWidget self, int red, int green, int blue) {
  if (self->text_red != red || self->text_green != green || self->text_blue != blue) {
    self->text_red = red;
    self->text_green = green;
    self->text_blue = blue;
    SetTextColor(self->dc, RGB(self->text_red, self->text_green, self->text_blue));
  }
  return 0;
}

int PmCanvas_SetFont(PmWidget self, char* font_name, int size, int weight, int italic, int underline, int strikeout) {
  if (weight <= 0) {
    weight = pmfw_nomal;
  }

  unsigned int tmp_weight = (unsigned int)(weight / 50);
  switch (tmp_weight) {
    case 0:
      weight = 100;
      break;
    case 1:
      weight = 100;
      break;
    case 2:  // 100
      weight = 100;
      break;
    case 3:
      weight = 200;
      break;
    case 4:
      weight = 200;
      break;
    case 5:
      weight = 300;
      break;
    case 6:
      weight = 300;
      break;
    case 7:
      weight = 400;
      break;
    case 8:
      weight = 400;
      break;
    case 9:
      weight = 500;
      break;
    case 10:  // 500
      weight = 500;
      break;
    case 11:
      weight = 600;
      break;
    case 12:
      weight = 600;
      break;
    case 13:
      weight = 700;
      break;
    case 14:
      weight = 700;
      break;
    case 15:
      weight = 800;
      break;
    case 16:
      weight = 800;
      break;
    case 17:
      weight = 900;
      break;
    case 18:  // 900
      weight = 900;
      break;
    default:
      weight = 900;
  }
  if (!self->font_name || strcmp(self->font_name, font_name) != 0 || self->font_size != size ||
      self->font_weight != weight || self->font_is_italic != italic || self->font_is_underline != underline ||
      self->font_is_strikeout != strikeout) {
    if (!self->font_name) {
      free(self->font_name);
    }
    self->font_name = malloc(strlen(font_name));
    strcpy(self->font_name, font_name);
    self->font_name_utf16 = utf8to16(self->font_name_utf16, self->font_name);
    self->font_size = size;
    self->font_weight = weight;
    self->font_is_italic = italic;
    self->font_is_underline = underline;
    self->font_is_strikeout = strikeout;

    self->f_font = 1;
  }
  return 0;
}

int PmCanvas_DrawLine(PmWidget self, int x1, int y1, int x2, int y2) {
  create_pen(self);
  MoveToEx(self->dc, x1, y1, NULL);
  LineTo(self->dc, x2, y2);

  return 0;
}
int PmCanvas_DrawArc(PmWidget self, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
  create_pen(self);
  Arc(self->dc, x1, y1, x2, y2, x3, y3, x4, y4);

  return 0;
}

int PmCanvas_Bezier(PmWidget self, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
  create_pen(self);
  // MoveToEx(self->dc, x1, y1, NULL);
  POINT point[] = {{x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}};
  PolyBezier(self->dc, point, 4);

  return 0;
}

int PmCanvas_DrawRect(PmWidget self, int x1, int y1, int x2, int y2) {
  RECT rect;
  rect.left = x1;
  rect.top = y1;
  rect.right = x2;
  rect.bottom = y2;
  create_brush(self);
  create_pen(self);
  Rectangle(self->dc, x1, y1, x2, y2);

  return 0;
}
int PmCanvas_DrawRectStroke(PmWidget self, int x1, int y1, int x2, int y2) {
  RECT rect;
  rect.left = x1;
  rect.top = y1;
  rect.right = x2;
  rect.bottom = y2;
  SelectObject(self->dc, GetStockObject(NULL_BRUSH));
  create_pen(self);
  Rectangle(self->dc, x1, y1, x2, y2);
  SelectObject(self->dc, self->brush);

  return 0;
}

int PmCanvas_DrawRectFill(PmWidget self, int x1, int y1, int x2, int y2) {
  RECT rect;
  rect.left = x1;
  rect.top = y1;
  rect.right = x2;
  rect.bottom = y2;
  create_brush(self);
  SelectObject(self->dc, GetStockObject(NULL_PEN));
  Rectangle(self->dc, x1, y1, x2, y2);
  SelectObject(self->dc, self->pen);

  return 0;
}

int PmCanvas_EllipseRect(PmWidget self, int x1, int y1, int x2, int y2) {
  RECT rect;
  rect.left = x1;
  rect.top = y1;
  rect.right = x2;
  rect.bottom = y2;
  create_brush(self);
  create_pen(self);
  Ellipse(self->dc, x1, y1, x2, y2);

  return 0;
}
int PmCanvas_DrawEllipseStroke(PmWidget self, int x1, int y1, int x2, int y2) {
  RECT rect;
  rect.left = x1;
  rect.top = y1;
  rect.right = x2;
  rect.bottom = y2;
  SelectObject(self->dc, GetStockObject(NULL_BRUSH));
  create_pen(self);
  Ellipse(self->dc, x1, y1, x2, y2);
  SelectObject(self->dc, self->brush);

  return 0;
}

int PmCanvas_DrawEllipseFill(PmWidget self, int x1, int y1, int x2, int y2) {
  RECT rect;
  rect.left = x1;
  rect.top = y1;
  rect.right = x2;
  rect.bottom = y2;
  create_brush(self);
  SelectObject(self->dc, GetStockObject(NULL_PEN));
  Ellipse(self->dc, x1, y1, x2, y2);
  SelectObject(self->dc, self->pen);

  return 0;
}

int PmCanvas_DrawText(PmWidget self, char* text, int x, int y) {
  unsigned short* text_utf16 = utf8to16(NULL, text);
  RECT rect = {x, y, INT_MAX, INT_MAX};
  SetBkMode(self->dc, TRANSPARENT);
  creat_font(self);
  DrawTextW(self->dc, text_utf16, lstrlenW(text_utf16), &rect, DT_LEFT);
  free(text_utf16);
  return 0;
}

int PmCanvas_DrawTextCalc(PmWidget self, char* text) {
  unsigned short* text_utf16 = utf8to16(NULL, text);
  RECT rect = {0, 0, 0, 0};
  creat_font(self);
  DrawText(self->dc, text_utf16, lstrlenW(text_utf16), &rect, DT_CALCRECT);
  self->text_size_x = rect.right;
  self->text_size_y = rect.bottom;
  free(text_utf16);
  return 0;
}
int PmCanvas_GetTextX(PmWidget self) { return self->text_size_x; }
int PmCanvas_GetTextY(PmWidget self) { return self->text_size_y; }

int PmCanvas_Update(PmWidget self) {
  RedrawWindow(self->hwnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
  return 0;
}

void create_pen(PmWidget self) {
  if (self->f_pen) {
    self->f_pen = 0;
    if (!self->pen) {
      DeleteObject(self->pen);
    }
    self->pen = CreatePen(PS_SOLID, self->pen_size, RGB(self->pen_red, self->pen_green, self->pen_blue));
    SelectObject(self->dc, self->pen);
  }
}
void create_brush(PmWidget self) {
  if (self->f_brush) {
    self->f_brush = 0;
    if (!self->brush) {
      DeleteObject(self->brush);
    }
    self->brush = CreateSolidBrush(RGB(self->brush_red, self->brush_green, self->brush_blue));
    SelectObject(self->dc, self->brush);
  }
}
void creat_font(PmWidget self) {
  if (self->f_font) {
    self->f_font = 0;
    if (!self->hfont) {
      DeleteObject(self->hfont);
    }
    self->hfont = CreateFontW(self->font_size, 0, 0, 0, self->font_weight, self->font_is_italic,
                              self->font_is_underline, self->font_is_strikeout, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, self->font_name_utf16);
    if (!self->hfont) {
      self->hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    } else {
    }
    SelectObject(self->dc, self->hfont);
  }
}

void get_desktop_size(PmSize* size) {
  size->x = GetSystemMetrics(SM_CXVIRTUALSCREEN);
  size->y = GetSystemMetrics(SM_CYVIRTUALSCREEN);
}