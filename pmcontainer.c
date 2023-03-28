#include "pmcontainer.h"

#include <stdlib.h>
#include <string.h>

#include "pmobj.h"

typedef struct tag_PmContainer_Impl {
  PmObj obj;
  void* data;
} PmContainer_Impl;

const PmObj pm_container_obj = {sizeof(PmContainer_Impl), (PmObj_MoveFunc)PmContainer_Move,
                                (PmObj_CopyFunc)PmContainer_Copy, (PmObj_DeleteFunc)PmContainer_Delete};

PmContainer PmContainer_New(size_t size, PmObj obj) {
  PmContainer_Impl* self;
  self = malloc(sizeof(PmContainer_Impl));
  self->obj = obj;
  self->data = NULL;
  return (PmContainer)self;
}

PmContainer PmContainer_Move(PmContainer dst, PmContainer src) {
  if (!PmObj_Cmp(&dst->obj, &src->obj)) {
    exit(1);
    return NULL;
  }
  PmObj_Move(&src->obj, &dst->data, &src->data);
  return dst;
}
PmContainer PmContainer_Copy(PmContainer dst, PmContainer src) {
  if (!PmObj_Cmp(&dst->obj, &src->obj)) {
    exit(1);
    return NULL;
  }
  PmObj_Copy(&src->obj, &dst->data, &src->data);
  return dst;
}

void PmContainer_Delete(PmContainer self) {
  PmObj_Del(&self->obj, self->data);
  free(self);
}

int PmContainer_GetSize(PmContainer self) { return PmObj_GetSize(&self->obj); }

void* PmContainer_Get(PmContainer self) { return self->data; }

int PmContainer_Set(PmContainer self, void* data) {
  PmObj_Copy(&self->obj, &self->data, data);
  return PMCONTAINER_SUCCESS;
}
