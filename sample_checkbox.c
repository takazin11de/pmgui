#include "pmgui.h"

int clicked(PmEvent *event) {
  PmWidget *widgets = event->data;
  int state = PmWidget_GetCheckState(event->widget);
  if (state) {
    PmWidget_SetText(widgets[0], "1");
    PmWidget_SetCheck(widgets[1], 1);
  } else {
    PmWidget_SetText(widgets[0], "0");
    PmWidget_SetCheck(widgets[1], 0);
  }
  return 0;
}

int main() {
  PmGui_Init();

  PmWidget window = PmWidget_Init(pmw_window);
  PmWidget_Show(window);

  PmWidget checkbox1 = PmWidget_Init(pmw_checkbox);
  PmWidget_SetParent(checkbox1, window);
  PmWidget_SetText(checkbox1, "text");
  PmWidget_SetPos(checkbox1, 10, 10);
  PmWidget_Show(checkbox1);

  PmWidget checkbox2 = PmWidget_Init(pmw_checkbox);
  PmWidget_SetParent(checkbox2, window);
  PmWidget_SetText(checkbox2, "text");
  PmWidget_SetPos(checkbox2, 10, 110);
  PmWidget_Disable(checkbox2);
  PmWidget_Show(checkbox2);

  PmWidget label = PmWidget_Init(pmw_label);
  PmWidget_SetParent(label, window);
  PmWidget_SetText(label, "0");
  PmWidget_SetPos(label, 10, 210);
  PmWidget_Show(label);

  PmWidget widgets[] = {label, checkbox2};

  PmEvent_Bind(pme_clicked, clicked, checkbox1, widgets);

  return PmGui_ListenEvent();
}
