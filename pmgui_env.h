#ifndef PMGUI_ENV_H
#define PMGUI_ENV_H
#ifdef __cplusplus
extern "C"
{
#endif

#define UNICODE

#include <windows.h>

#include "pmgui.h"
#include "pmlist.h"
#include "pmobj.h"

  enum
  {
    pme_env_begin = 0x80000000,
    pme_env_create,
    pme_env_paint,
    pme_env_end
  };

  typedef struct tag_PmWidget_Impl
  {
    int type;
    HWND hwnd;
    char *text;
    unsigned short *utf16_text;
    int size_x;
    int size_y;
    int pos_x;
    int pos_y;

    HDC dc;
    HBITMAP bitmap;

    unsigned char pen_red;
    unsigned char pen_green;
    unsigned char pen_blue;

    unsigned int pen_size;

    HPEN pen;
    int f_pen;

    unsigned char brush_red;
    unsigned char brush_green;
    unsigned char brush_blue;

    HBRUSH brush;
    int f_brush;

    unsigned char text_red;
    unsigned char text_green;
    unsigned char text_blue;

    UINT_PTR timer_id;
    int timer;
    TIMERPROC timer_func;

    void *data;

    struct tag_PmWidget_Impl *parent;
    HFONT hfont;
    unsigned int font_size;
    unsigned int font_weight;
    int font_is_italic;
    int font_is_underline;
    int font_is_strikeout;
    char *font_name;
    unsigned short *font_name_utf16;
    int f_font;

    int text_size_x;
    int text_size_y;

    PmList radiobox_list;

  } PmWidget_Impl;

  extern const PmObj pm_widget_obj;

  typedef struct tag_PmGui
  {
    unsigned short *wnd_class_name;
    WNDCLASSW wndclass;
    HINSTANCE instance;
    HWND hidden_hwnd;

    unsigned short *str_label;
    unsigned short *str_button;
    unsigned short *str_input;
    unsigned short *str_listbox;
    unsigned short *str_combobox;
    unsigned short *str_void;
    PmList event_list;
  } PmGui;

  extern PmGui pmgui;

  LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
  ULONG_PTR enable_visual_styles();

  extern const PmObj pm_event_obj;

  typedef struct tag_PmSize
  {
    int x;
    int y;
  } PmSize;

  enum
  {
    PM_WM_RADIO_GROUP_CLICKED = WM_USER,
  };

#ifdef __cplusplus
}
#endif
#endif
