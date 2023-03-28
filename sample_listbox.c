#include <stdio.h>

#include "pmgui.h"

int clicked(PmEvent *event) {
  PmWidget *widgets = event->data;
  printf("aaa\n");
  int select = PmWidget_GetListboxSelect(widgets[1]);
  printf("%d\n", select);

  char *str = malloc(64);
  PmWidget_SetText(widgets[0], itoa(select, str, 10));

  free(str);

  return 0;
}

int main() {
  PmGui_Init();

  PmWidget window = PmWidget_Init(pmw_window);
  PmWidget_Show(window);

  PmWidget listbox = PmWidget_Init(pmw_listbox);
  PmWidget_SetParent(listbox, window);
  PmWidget_SetText(listbox, "text");
  PmWidget_SetPos(listbox, 10, 10);
  PmWidget_AddListbox(listbox, "aaaaa");
  PmWidget_AddListbox(listbox, "bbbbb");
  PmWidget_AddListbox(listbox, "ccccc");
  PmWidget_Show(listbox);

  PmWidget label = PmWidget_Init(pmw_label);
  PmWidget_SetParent(label, window);
  PmWidget_SetText(label, "0");
  PmWidget_SetPos(label, 10, 210);
  PmWidget_Show(label);

  PmWidget button = PmWidget_Init(pmw_button);
  PmWidget_SetParent(button, window);
  PmWidget_SetText(button, "OK");
  PmWidget_SetPos(button, 10, 310);
  PmWidget_Show(button);

  PmWidget widgets[] = {label, listbox};

  PmEvent_Bind(pme_clicked, clicked, button, widgets);

  return PmGui_ListenEvent();
}
