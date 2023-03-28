
#include "pmlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tag_PmListData_Impl* PmListData;

void PmList_ResetMember(PmList self);

typedef struct tag_PmListData_Impl {
  PmListData prev;
  PmListData next;
  void* data;
} PmListData_Impl;

typedef struct tag_PmList_Impl {
  PmListData_Impl begin;
  PmListData_Impl end;
  size_t size;
  int num;
  PmObj obj;
} PmList_Impl;

const PmObj pm_list_obj = {sizeof(PmList_Impl), (PmObj_MoveFunc)PmList_Move, (PmObj_CopyFunc)PmList_Copy,
                           (PmObj_DeleteFunc)PmList_Delete};

PmList PmList_New(size_t size, PmObj obj) {
  PmList_Impl* self;
  self = malloc(sizeof(PmList_Impl));
  PmList_ResetMember(self);
  self->size = size;
  self->obj = obj;
  return ((PmList)self);
}

void PmList_Delete(PmList self) {
  PmList_Clear(self);
  free(self);
}
void PmList_ResetMember(PmList self) {
  self->begin.prev = NULL;
  self->begin.data = NULL;
  self->begin.next = &self->end;
  self->end.prev = &self->begin;
  self->end.data = NULL;
  self->end.next = NULL;
  self->num = 0;
}
PmList PmList_Move(PmList dst, PmList src) {
  PmList_Clear(dst);
  dst->begin.next = src->begin.next;
  dst->end.prev = src->end.prev;
  dst->size = src->size;
  dst->num = src->num;
  dst->obj = src->obj;

  PmList_ResetMember(src);

  return dst;
}
PmList PmList_Copy(PmList dst, PmList src) {
  PmList_Clear(dst);

  void* data;
  PmListIt it;
  for_PmList(src, &it, data) { PmList_PushBack(dst, data); }

  return dst;
}

int PmList_PushBack(PmList self, void* data) {
  PmListData tmp = malloc(sizeof(PmListData_Impl));
  void* new_data = malloc(self->size);
  PmListData prev = self->end.prev;

  PmObj_Copy(&(self->obj), new_data, data);

  tmp->data = new_data;

  prev->next = tmp;
  self->end.prev = tmp;
  tmp->prev = prev;
  tmp->next = &(self->end);

  self->num++;
  return (PMLIST_SUCCESS);
}
int PmList_PushFront(PmList self, void* data) {
  PmListData tmp = malloc(sizeof(PmListData_Impl));
  void* new_data = malloc(self->size);
  PmListData next = self->begin.next;

  PmObj_Copy(&(self->obj), new_data, data);

  tmp->data = new_data;

  next->prev = tmp;
  self->begin.next = tmp;
  tmp->next = next;
  tmp->prev = &(self->begin);

  self->num++;
  return (PMLIST_SUCCESS);
}

int PmList_PopFront(PmList self) {
  PmListData tmp = self->begin.next->next;

  PmObj_Del(&(self->obj), self->begin.next->data);
  free(self->begin.next);

  self->begin.next = tmp;
  tmp->prev = &self->begin;
  self->num--;
  return (PMLIST_SUCCESS);
}
int PmList_PopBack(PmList self) {
  PmListData tmp = self->end.prev->prev;

  PmObj_Del(&(self->obj), self->end.prev->data);
  free(self->end.prev);

  self->end.prev = tmp;
  tmp->next = &self->end;
  self->num--;
  return (PMLIST_SUCCESS);
}

int PmList_Clear(PmList self) {
  int i;
  int size = self->num;
  for (i = 0; i < size; i++) {
    PmList_PopFront(self);
  }

  PmList_ResetMember(self);
  return (PMLIST_SUCCESS);
}

int PmList_MoveMarge(PmList list1, PmList list2) {
  if (list1->size != list2->size) {
    return (PMLIST_ERROR);
  }

  list1->end.prev->next = list2->begin.next;
  list2->begin.next->prev = list1->end.prev;
  list1->end.prev = list2->end.prev;
  list1->num += list2->num;

  PmList_ResetMember(list2);

  return (PMLIST_SUCCESS);
}

int PmList_GetSize(PmList self) { return (self->num); }

int PmListIt_Erase_GotoNext(PmListIt* self) {
  if (self->current != &self->list->begin && self->current != &self->list->end) {
    PmListData_Impl* prev = self->current->prev;
    PmListData_Impl* next = self->current->next;
    prev->next = next;
    next->prev = prev;

    PmObj_Del(&(self->list->obj), self->current->data);
    free(self->current);

    self->current = next;
    self->list->num--;
    return (PMLIST_SUCCESS);
  }
  return (PMLIST_ERROR);
}
int PmListIt_Erase_GotoPrev(PmListIt* self) {
  if (self->current != &self->list->begin && self->current != &self->list->end) {
    PmListData prev = self->current->prev;
    PmListData next = self->current->next;
    prev->next = next;
    next->prev = prev;

    PmObj_Del(&(self->list->obj), self->current->data);
    free(self->current);

    self->current = prev;
    self->list->num--;
    return (PMLIST_SUCCESS);
  }
  return (PMLIST_ERROR);
}
PmListIt* PmListIt_New() {
  PmListIt* self = malloc(sizeof(PmListIt));
  self->list = NULL;
  self->current = NULL;
  return (self);
}

int PmListIt_Delete(PmListIt* it) {
  free(it);
  return 0;
}

int PmListIt_Init(PmListIt* self, PmList list) {
  self->list = list;
  self->current = &list->begin;
  return (PmListIt_Next(self));
}

void* PmListIt_Get(PmListIt* self) { return (self->current->data); }

int PmListIt_Next(PmListIt* self) {
  if (self->current->next != NULL) {
    self->current = self->current->next;
    return (PMLIST_SUCCESS);
  }
  return (PMLIST_ERROR);
}

int PmListIt_Prev(PmListIt* self) {
  if (self->current->prev != NULL) {
    self->current = self->current->prev;
    return (PMLIST_SUCCESS);
  }
  return (PMLIST_ERROR);
}

int PmListIt_IsEnd(PmListIt* self) { return (self->current == &self->list->end); }
int PmListIt_IsBegin(PmListIt* self) { return (self->current == &self->list->begin); }

int PmListIt_IsBack(PmListIt* self) { return (self->current->next == &self->list->end); }
int PmListIt_IsFront(PmListIt* self) { return (self->current->prev == &self->list->begin); }

int PmListIt_InsertPrev(PmListIt* self, void* data) {
  PmListData tmp = malloc(sizeof(PmListData_Impl));
  void* new_data = malloc(self->list->size);

  tmp->next = self->current;
  tmp->prev = self->current->prev;
  self->current->prev = tmp;
  tmp->prev->next = tmp;

  PmObj_Copy(&(self->list->obj), new_data, data);

  tmp->data = new_data;

  self->list->num++;
  return (PMLIST_SUCCESS);
}

int PmListIt_InsertNext(PmListIt* self, void* data) {
  PmListData tmp = malloc(sizeof(PmListData_Impl));
  void* new_data = malloc(self->list->size);

  tmp->next = self->current->next;
  tmp->prev = self->current;
  self->current->next = tmp;
  tmp->next->prev = tmp;

  PmObj_Copy(&(self->list->obj), new_data, data);

  tmp->data = new_data;

  self->list->num++;
  return (PMLIST_SUCCESS);
}

void* PmList_GetFront(PmList list) { return (void*)list->begin.next->data; }
void* PmList_GetBack(PmList list) { return (void*)list->end.prev->data; }