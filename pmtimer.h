#ifndef PMTIMER_H
#define PMTIMER_H
#ifdef __cplusplus
extern "C" {
#endif

#define UNICODE

#include <windows.h>

#include "pmgui_env.h"

int PmWidget_SetTimer(PmWidget self, int timer);
int PmWidget_StartTimer(PmWidget self);
int PmWidget_StopTimer(PmWidget self);

#ifdef __cplusplus
}
#endif
#endif
