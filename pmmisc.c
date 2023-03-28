#include "pmmisc.h"

#include <windows.h>

#include "pmconv.h"

ULONG_PTR enable_visual_styles() {
  unsigned short dir[MAX_PATH];
  ULONG_PTR cookie = FALSE;
  ACTCTXW actctx = {
      sizeof(ACTCTX),
      ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_SET_PROCESS_DEFAULT | ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID,
      utf8to16(NULL, "shell32.dll"),
      0,
      0,
      dir,
      (LPCTSTR)124};
  UINT cch = GetSystemDirectoryW(dir, sizeof(dir) / sizeof(*dir));
  if (cch >= sizeof(dir) / sizeof(*dir)) {
    return FALSE;
  }
  dir[cch] = 0;
  ActivateActCtx(CreateActCtxW(&actctx), &cookie);
  return cookie;
}