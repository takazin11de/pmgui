#include <windows.h>

#include "pmgui_env.h"

void timer_func(HWND hwnd, UINT msg, UINT_PTR timer_id, DWORD time) { WndProc(hwnd, msg, timer_id, 0); }

int PmWidget_SetTimer(PmWidget self, int timer) {
  self->timer = timer;
  return 0;
}
int PmWidget_StartTimer(PmWidget self) {
  UINT_PTR ret = SetTimer(NULL, 0, self->timer, timer_func);
  if (!ret) {
    exit(1);
  }
  self->timer_id = ret;
  return 0;
}
int PmWidget_StopTimer(PmWidget self) {
  BOOL ret = KillTimer(NULL, self->timer_id);
  if (!ret) {
    exit(1);
  }
  return 0;
}