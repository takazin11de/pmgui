#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pmconv.h"
#include "pmgui_env.h"
#include "pmmisc.h"
#include "pmtimer.h"
#include "pmwidget.h"

PmGui pmgui;

const PmObj pm_event_obj = {sizeof(PmEvent), NULL, NULL, NULL};

void PmGui_Init()
{
  setvbuf(stdout, NULL, _IONBF, 0);

  pmgui.str_void = utf8to16(NULL, "");
  pmgui.str_label = utf8to16(NULL, "STATIC");
  pmgui.str_button = utf8to16(NULL, "BUTTON");
  pmgui.str_input = utf8to16(NULL, "EDIT");
  pmgui.str_listbox = utf8to16(NULL, "LISTBOX");
  pmgui.str_combobox = utf8to16(NULL, "COMBOBOX");
  pmgui.wnd_class_name = utf8to16(NULL, "PmGui WindowClassName");
  pmgui.event_list = PmList_New(sizeof(PmEvent), pm_event_obj);

  enable_visual_styles();

  pmgui.instance = GetModuleHandleW(NULL);
  pmgui.wndclass.style = CS_HREDRAW | CS_VREDRAW;
  pmgui.wndclass.lpfnWndProc = WndProc;
  pmgui.wndclass.cbClsExtra = 0;
  pmgui.wndclass.cbWndExtra = 0;
  pmgui.wndclass.hInstance = pmgui.instance;
  pmgui.wndclass.hIcon = NULL;
  pmgui.wndclass.hCursor =
      LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, GetSystemMetrics(SM_CXCURSOR), GetSystemMetrics(SM_CYCURSOR), LR_SHARED);
  pmgui.wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
  pmgui.wndclass.lpszMenuName = NULL;
  pmgui.wndclass.lpszClassName = pmgui.wnd_class_name;

  if (!RegisterClassW(&pmgui.wndclass))
  {
    exit(1);
  }
  pmgui.hidden_hwnd = CreateWindowW(pmgui.wnd_class_name, pmgui.str_void, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, pmgui.instance, NULL);
}

int PmGui_ListenEvent()
{
  MSG msg;
  while (GetMessageW(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
  return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  PmListIt it;
  PmEvent *tmp_event;
  PmEvent event;
  HWND child_hwnd;
  UINT_PTR timer_id;
  int f_list = 1;
  switch (msg)
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_COMMAND:
    child_hwnd = (HWND)lparam;
    // pme_onclick
    if (HIWORD(wparam) == BN_CLICKED)
    {
      for_PmList(pmgui.event_list, &it, tmp_event)
      {
        if (tmp_event->widget->hwnd == child_hwnd && tmp_event->type == pme_clicked)
        {
          event.type = tmp_event->type;
          event.func = tmp_event->func;
          event.widget = tmp_event->widget;
          event.data = tmp_event->data;
          return event.func(&event);
        }
      }
      return (DefWindowProcW(hwnd, msg, wparam, lparam));
    }
    else
    {
      return (DefWindowProcW(hwnd, msg, wparam, lparam));
    }
    break;
  case PM_WM_RADIO_GROUP_CLICKED:
    // pme_onclick
    for_PmList(pmgui.event_list, &it, tmp_event)
    {
      if (tmp_event->widget->type == pmw_radiobox_group && tmp_event->type == pme_clicked)
      {
        event.type = tmp_event->type;
        event.func = tmp_event->func;
        event.widget = tmp_event->widget;
        event.data = tmp_event->data;
        return event.func(&event);
      }
    }

    break;
  case WM_TIMER:
    timer_id = wparam;
    for_PmList(pmgui.event_list, &it, tmp_event)
    {
      if (tmp_event->widget->timer_id == timer_id && tmp_event->type == pme_timer)
      {
        event.type = tmp_event->type;
        event.func = tmp_event->func;
        event.widget = tmp_event->widget;
        event.data = tmp_event->data;
        return event.func(&event);
      }
    }
    return (DefWindowProcW(hwnd, msg, wparam, lparam));
    break;

  case WM_PAINT:
    for_PmList(pmgui.event_list, &it, tmp_event)
    {
      if (tmp_event->widget->hwnd == hwnd && tmp_event->type == pme_env_paint)
      {
        event.type = tmp_event->type;
        event.func = tmp_event->func;
        event.widget = tmp_event->widget;
        event.data = tmp_event->data;
        return event.func(&event);
      }
    }
    return (DefWindowProcW(hwnd, msg, wparam, lparam));
    break;

  default:
    return (DefWindowProcW(hwnd, msg, wparam, lparam));
  }
  return (0);
}

int PmEvent_Bind(int event_type, PmEvent_Callback function, PmWidget widgets, void *data)
{
  PmEvent event;
  event.type = event_type;
  event.func = function;
  event.widget = widgets;
  event.data = data;
  PmListIt it;
  PmEvent *tmp;
  int f_list = 1;
  for_PmList(pmgui.event_list, &it, tmp)
  {
    if (tmp->widget == widgets && tmp->type == event_type)
    {
      f_list = 0;
      break;
    }
  }
  if (f_list)
  {
    PmList_PushFront(pmgui.event_list, &event);
  }

  return 0;
}
