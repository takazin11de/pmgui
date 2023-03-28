
#include <stdio.h>

#include "pmgui.h"

// clang main.c pmcontainer.c pmconv.c pmgui.c pmlist.c pmtimer.c pmwidget.c pmmisc.c pmobj.c pmvector.c -mwindows

int clicked(PmEvent *event) {
  PmWidget_SetText(event->widget, PmWidget_GetText(event->data));
  return 0;
}

int ontimer(PmEvent *event) {
  PmWidget_StopTimer(event->widget);
  PmWidget_SetText(event->data, "aaaaa");
  return 0;
}

int main() {
  PmGui_Init();

  PmWidget window = PmWidget_Init(pmw_window);
  PmWidget_SetText(window, "Window Title");
  PmWidget_Show(window);

  PmWidget button = PmWidget_Init(pmw_button);

  PmWidget_SetText(button, "OK");
  PmWidget_SetParent(button, window);

  PmWidget input = PmWidget_Init(pmw_input);

  PmWidget_SetText(input, "input here");
  PmWidget_SetPos(input, 0, 100);
  PmWidget_SetParent(input, window);

  PmEvent_Bind(pme_clicked, clicked, button, input);

  PmWidget timer = PmWidget_Init(pmw_timer);
  PmWidget_SetTimer(timer, 1000);
  PmEvent_Bind(pme_timer, ontimer, timer, button);
  PmWidget_StartTimer(timer);

  return PmGui_ListenEvent();
}
