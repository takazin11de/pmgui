#ifndef PMLIST_H
#define PMLIST_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "pmobj.h"

#define PMLIST_ERROR (1)
#define PMLIST_SUCCESS (0)

typedef struct tag_PmList_Impl* PmList;

typedef struct tag_PmListIt {
  struct tag_PmList_Impl* list;
  struct tag_PmListData_Impl* current;
} PmListIt;

PmList PmList_New(size_t size, PmObj obj);
void PmList_Delete(PmList self);

int PmList_GetSize(PmList self);

int PmList_PushFront(PmList self, void* data);
int PmList_PushBack(PmList self, void* data);

int PmList_PopFront(PmList self);
int PmList_PopBack(PmList self);

int PmList_Clear(PmList self);

int PmList_MoveMarge(PmList self, PmList list);

void* PmList_GetFront(PmList list);
void* PmList_GetBack(PmList list);

PmListIt* PmListIt_New();
int PmListIt_Delete(PmListIt* self);

int PmListIt_Init(PmListIt* self, PmList list);

void* PmListIt_Get(PmListIt* self);

int PmListIt_Next(PmListIt* self);
int PmListIt_Prev(PmListIt* self);

int PmListIt_IsBegin(PmListIt* self);
int PmListIt_IsEnd(PmListIt* self);

int PmListIt_IsFront(PmListIt* self);
int PmListIt_IsBack(PmListIt* self);

int PmListIt_Erase_GotoNext(PmListIt* self);
int PmListIt_Erase_GotoPrev(PmListIt* self);

int PmListIt_InsertPrev(PmListIt* self, void* data);
int PmListIt_InsertNext(PmListIt* self, void* data);

PmList PmList_Move(PmList dst, PmList src);
PmList PmList_Copy(PmList dst, PmList src);
extern const PmObj pm_list_obj;

#define for_PmList(list, ptr_of_it, data)                                  \
  for (PmListIt_Init((ptr_of_it), (list));                                 \
       ((data) = PmListIt_Get((ptr_of_it)), !PmListIt_IsEnd((ptr_of_it))); \
       PmListIt_Next((ptr_of_it)))
// typedef struct {
//   int value;
// } TYPE;
// PmList list = PmList_New(sizeof(TYPE));
// PmListIt it;
// TYPE *data;
//
// for_PmList(list, &it, data){
//   printf("%d", data->value);
// }

#ifdef __cplusplus
}
#endif
#endif
