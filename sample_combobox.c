#include <stdio.h>

#include "pmgui.h"

int clicked(PmEvent *event) {
  PmWidget *widgets = event->data;
  int select = PmWidget_GetComboboxSelect(widgets[1]);
  char *str = malloc(256);
  sprintf_s(str, 256, "index : %d, text : %s", select, PmWidget_GetComboboxSelectText(widgets[1]));
  PmWidget_SetText(widgets[0], str);
  free(str);

  return 0;
}

int main() {
  PmGui_Init();

  PmWidget window = PmWidget_Init(pmw_window);
  PmWidget_Show(window);

  PmWidget combobox = PmWidget_Init(pmw_combobox);
  PmWidget_SetParent(combobox, window);
  PmWidget_SetPos(combobox, 10, 10);
  PmWidget_AddCombobox(combobox, "aaaaa");
  PmWidget_AddCombobox(combobox, "bbbbb");
  PmWidget_AddCombobox(combobox, "ccccc");
  PmWidget_SetComboboxSelect(combobox, 0);
  PmWidget_Show(combobox);

  PmWidget label = PmWidget_Init(pmw_label);
  PmWidget_SetParent(label, window);
  PmWidget_SetText(label, "0");
  PmWidget_SetGeometry(label, 10, 210, 1000, 30);
  PmWidget_Show(label);

  PmWidget button = PmWidget_Init(pmw_button);
  PmWidget_SetParent(button, window);
  PmWidget_SetText(button, "OK");
  PmWidget_SetPos(button, 10, 310);
  PmWidget_Show(button);

  PmWidget widgets[] = {label, combobox};

  PmEvent_Bind(pme_clicked, clicked, button, widgets);

  return PmGui_ListenEvent();
}
