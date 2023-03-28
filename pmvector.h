#ifndef PMVECTOR_H
#define PMVECTOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define PMVECTOR_ERROR (1)
#define PMVECTOR_SUCCESS (0)

typedef struct tag_PmVector_Impl* PmVector;

PmVector PmVector_New(size_t size);
int PmVector_Delete(PmVector self);

int PmVector_GetSize(PmVector self);

// int PmVector_PushFront(PmVector self, void* data);
int PmVector_PushBack(PmVector self, void* data);

// int PmVector_PopFront(PmVector self);
int PmVector_PopBack(PmVector self);

int PmVector_Clear(PmVector self);

int PmVector_MoveMarge(PmVector self, PmVector vector);
void* PmVector_Ptr(PmVector self);

// mVectorIt PmVectorIt_New();
// int PmVectorIt_Delete(PmVectorIt self);

// int PmVectorIt_Init(PmVectorIt self, PmVector vector);

// void* PmVectorIt_Get(PmVectorIt self);

// int PmVectorIt_Next(PmVectorIt self);
// int PmVectorIt_Prev(PmVectorIt self);

// int PmVectorIt_IsBegin(PmVectorIt self);
// int PmVectorIt_IsEnd(PmVectorIt self);

// int PmVectorIt_IsFront(PmVectorIt self);
// int PmVectorIt_IsBack(PmVectorIt self);

// int PmVectorIt_Erase_GotoNext(PmVectorIt self);
// int PmVectorIt_Erase_GotoPrev(PmVectorIt self);

// int PmVectorIt_InsertPrev(PmVectorIt self, void* data);
// int PmVectorIt_InsertNext(PmVectorIt self, void* data);

#define for_PmVector(vector, index, data)               \
  for ((index) = 0; (data) = PmVector_GetPtr((vector)), \
      (index) < PmVector_GetSize((vector), (index));    \
       (index)++)

// typedef struct {
//   int value;
// } TYPE;
// TYPE *data;

// PmVector vector = PmVector_New(sizeof(TYPE));
// int i;

// for(i=0; data = PmVector_Ptr(vector),i<PmVector_Size(vector,i); i++){
//   printf("%d", data[i]->value);
// }

// or

// for_PmVector(vector, i, data){
//   printf("%d", data[i]->value);
// }

// TYPE aaa;
// aaa.value=100;
// PmVector_PushBack(vector, &aaa);

#ifdef __cplusplus
}
#endif
#endif
