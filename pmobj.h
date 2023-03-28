#ifndef PMOBJ_H
#define PMOBJ_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef void *(*PmObj_MoveFunc)(void *dst, void *src);
typedef void *(*PmObj_CopyFunc)(void *dst, void *src);
typedef void (*PmObj_DeleteFunc)(void *data);

typedef struct tag_PmObj {
  size_t size;
  PmObj_MoveFunc move;
  PmObj_CopyFunc copy;
  PmObj_DeleteFunc del;
} PmObj;

extern const PmObj pm_obj_obj;

void PmObj_Init(PmObj *obj, size_t size, PmObj_MoveFunc move,
                PmObj_CopyFunc copy, PmObj_DeleteFunc del);
void *PmObj_Move(PmObj *obj, void *dst, void *src);
void *PmObj_Copy(PmObj *obj, void *dst, void *src);
void PmObj_Del(PmObj *obj, void *data);
int PmObj_Cmp(PmObj *obj1, PmObj *obj2);
size_t PmObj_GetSize(PmObj *obj);

#ifdef __cplusplus
}
#endif
#endif