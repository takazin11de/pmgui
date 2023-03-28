#ifndef PMCONTAINER_H
#define PMCONTAINER_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "pmobj.h"

#define PMCONTAINER_ERROR (1)
#define PMCONTAINER_SUCCESS (0)

typedef struct tag_PmContainer_Impl* PmContainer;

PmContainer PmContaine_New(size_t size, PmObj obj);
void PmContainer_Delete(PmContainer self);
int PmContainer_GetSize(PmContainer self);
int PmContainer_Clear(PmContainer self);
void* PmContainer_Get(PmContainer self);
int PmContainer_Set(PmContainer self, void* data);

PmContainer PmContainer_Move(PmContainer dst, PmContainer src);
PmContainer PmContainer_Copy(PmContainer dst, PmContainer src);
extern const PmObj pm_container_obj;

#ifdef __cplusplus
}
#endif
#endif
