
#include "pmobj.h"

#include <stdlib.h>
#include <string.h>

const PmObj pm_obj_obj = {sizeof(PmObj), 0, 0, 0};

void PmObj_Init(PmObj *obj, size_t size, PmObj_MoveFunc move,
                PmObj_CopyFunc copy, PmObj_DeleteFunc del) {
  obj->size = size;
  obj->move = move;
  obj->copy = copy;
  obj->del = del;
}

void *PmObj_Move(PmObj *obj, void *dst, void *src) {
  if (obj->move) {
    return obj->move(dst, src);
  }
  void *_dst = memcpy(dst, src, obj->size);
  memset(src, 0, obj->size);
  return _dst;
}

void *PmObj_Copy(PmObj *obj, void *dst, void *src) {
  if (obj->copy) {
    return obj->copy(dst, src);
  }
  void *_dst = memcpy(dst, src, obj->size);
  return _dst;
}

size_t PmObj_GetSize(PmObj *obj) { return obj->size; }

void PmObj_Del(PmObj *obj, void *data) {
  if (obj->del) {
    obj->del(data);
    return;
  }
  free(data);
}

int PmObj_Cmp(PmObj *obj1, PmObj *obj2) {
  return (obj1->size == obj2->size && obj1->move == obj2->move &&
          obj1->copy == obj2->copy && obj1->del == obj2->del);
}