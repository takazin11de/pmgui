
#include "pmvector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int PmVector_Alloc(PmVector self, size_t size);

// #define DEBUG

#ifdef DEBUG
#define D(x, a) (printf("DEBUG [ " #x " ] ( " a " ) \n", x), fflush(stdout))
#else
#define D(x, a)
#endif

typedef struct tag_PmVector_Impl {
  void* data;
  size_t
      data_size;  // size_of()はアライメント済みでパディングが含まれるサイズを返す
  size_t size;
  size_t alloc_size;
} PmVector_Impl;

PmVector PmVector_New(size_t size) {
  PmVector_Impl* self;
  self = malloc(sizeof(PmVector_Impl));
  self->data = NULL;
  self->data_size = size;
  self->size = 0;
  self->alloc_size = 0;
  return ((PmVector)self);
}

int PmVector_Delete(PmVector self) {
  free(self->data);
  free(self);
  return (PMVECTOR_SUCCESS);
}

int PmVector_PushBack(PmVector self, void* data) {
  PmVector_Impl* tmp;
  int i;

  PmVector_Alloc(self, self->size + 1);
  memcpy(self->data + self->data_size * (self->size - 1), data,
         self->data_size);

  return (PMVECTOR_SUCCESS);
}

int PmVector_PopBack(PmVector self) {
  if (self->size == 0) {
    return (PMVECTOR_ERROR);
  }
  self->size--;

  memset(self->data_size * self->size + self->data, 0, self->data_size);
  return (PMVECTOR_SUCCESS);
}

int PmVector_Clear(PmVector self) {
  for (; self->size;) {
    PmVector_PopBack(self);
  }
  free(self->data);
  self->data = NULL;
  self->size = 0;
  self->alloc_size = 0;
  return (PMVECTOR_SUCCESS);
}

int PmVector_MoveMarge(PmVector self, PmVector vector_source) {
  int i;
  PmVector_Alloc(self, self->size + vector_source->size);

  memcpy(self->data + self->data_size * (self->size - 1), vector_source->data,
         vector_source->data_size * vector_source->size);

  return (PMVECTOR_SUCCESS);
}

int PmVector_GetSize(PmVector self) { return (self->size); }
int PmVector_Alloc(PmVector self, size_t size) {
  PmVector_Impl* tmp;
  if (self->size > size) {
    return (PMVECTOR_ERROR);
  }
  self->size = size;
  for (; self->alloc_size < self->data_size * self->size;
       self->alloc_size *= 2) {
    if (self->alloc_size == 0) {
      self->alloc_size = 1;
    }
  }
  D(self->alloc_size, "%lld");
  tmp = malloc(self->alloc_size);
  memset(tmp, 0, self->alloc_size);
  if (self->data != NULL) {
    memcpy(tmp, self->data, self->data_size * self->size);
  }

  free(self->data);
  self->data = tmp;
  return (PMVECTOR_SUCCESS);
}

void* PmVector_Ptr(PmVector self) { return (self->data); }