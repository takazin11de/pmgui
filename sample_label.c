#include "pmgui.h"

int main() {
  PmGui_Init();

  PmWidget window = PmWidget_Init(pmw_window);
  PmWidget_Show(window);

  PmWidget label = PmWidget_Init(pmw_label);
  PmWidget_SetParent(label, window);
  PmWidget_SetText(label, "Label Text");
  PmWidget_Show(label);

  return PmGui_ListenEvent();
}
