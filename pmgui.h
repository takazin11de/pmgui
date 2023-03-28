#ifndef PMGUI_H
#define PMGUI_H
#ifdef __cplusplus
extern "C" {
#endif

#define UNICODE

#include <stddef.h>

#define PMGUI_ERROR (1)
#define PMGUI_SUCCESS (0)

#define dbg                                   \
  printf("DBG:%s, %d\n", __FILE__, __LINE__); \
  fflush(stdout);
#define pr_int(a)      \
  printf("%d\n", (a)); \
  fflush(stdout);

typedef struct tag_PmWidget_Impl* PmWidget;
typedef struct tag_PmEvent* Ptr_PmEvent;

typedef int (*PmEvent_Callback)(Ptr_PmEvent event);

typedef struct tag_PmEvent {
  int type;
  PmWidget widget;
  PmEvent_Callback func;
  void* data;
} PmEvent;

void PmGui_Init();
int PmGui_ListenEvent();

int PmEvent_Bind(int event_type, PmEvent_Callback function, PmWidget widgets, void* data);

enum { pme_begin, pme_clicked, pme_timer, pme_end };

enum {
  pmw_begin,
  pmw_window,
  pmw_label,
  pmw_button,
  pmw_checkbox,
  pmw_input,
  pmw_listbox,
  pmw_combobox,
  pmw_timer,
  pmw_canvas,
  pmw_end
};
enum {
  pmfw_thin = 100,
  pmfw_extra_light = 200,
  pmfw_ultra_light = 200,
  pmfw_light = 300,
  pmfw_nomal = 400,
  pmfw_regular = 400,
  pmfw_medium = 500,
  pmfw_semi_bold = 600,
  pmfw_demi_bold = 600,
  pmfw_bold = 700,
  pmfw_extra_bold = 800,
  pmfw_ultra_bold = 800,
  pmfw_black = 900,
  pmfw_heavy = 900
};

#define PmEvent_ANY (0)

#include "pmconv.h"
#include "pmwidget.h"

#ifdef __cplusplus
}
#endif
#endif
