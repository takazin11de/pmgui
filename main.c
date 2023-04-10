#include "pmgui.h"

int clicked(PmEvent *event)
{
  PmWidget *widgets = event->data;

  if (PmWidget_GetCheckState(widgets[1]))
  {
    PmWidget_SetText(widgets[0], "1");
  }
  else if (PmWidget_GetCheckState(widgets[2]))
  {
    PmWidget_SetText(widgets[0], "2");
  }
  else if (PmWidget_GetCheckState(widgets[3]))
  {
    PmWidget_SetText(widgets[0], "3");
  }
  else
  {
    PmWidget_SetText(widgets[0], "-");
  }
  return 0;
}

int main()
{
  PmGui_Init();

  PmWidget window = PmWidget_Init(pmw_window);
  PmWidget_Show(window);

  PmWidget radiobox1 = PmWidget_Init(pmw_radiobox);
  PmWidget_SetParent(radiobox1, window);
  PmWidget_SetText(radiobox1, "text1");
  PmWidget_SetPos(radiobox1, 10, 10);
  PmWidget_Show(radiobox1);

  PmWidget radiobox2 = PmWidget_Init(pmw_radiobox);
  PmWidget_SetParent(radiobox2, window);
  PmWidget_SetText(radiobox2, "text2");
  PmWidget_SetPos(radiobox2, 10, 40);
  PmWidget_Show(radiobox2);

  PmWidget radiobox3 = PmWidget_Init(pmw_radiobox);
  PmWidget_SetParent(radiobox3, window);
  PmWidget_SetText(radiobox3, "text3");
  PmWidget_SetPos(radiobox3, 10, 70);
  PmWidget_Show(radiobox3);

  PmWidget radiobox_group = PmWidget_Init(pmw_radiobox_group);

  PmRadioboxGroup_Add(radiobox_group, radiobox1);
  PmRadioboxGroup_Add(radiobox_group, radiobox2);
  PmRadioboxGroup_Add(radiobox_group, radiobox3);

  PmWidget label = PmWidget_Init(pmw_label);
  PmWidget_SetParent(label, window);
  PmWidget_SetText(label, "0");
  PmWidget_SetPos(label, 10, 210);
  PmWidget_Show(label);

  PmWidget widgets[] = {label, radiobox1, radiobox2, radiobox3};

  PmEvent_Bind(pme_clicked, clicked, radiobox_group, widgets);

  return PmGui_ListenEvent();
}
