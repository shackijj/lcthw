#ifndef darray_algos_h
#define darray_algos_h

#include <lcthw/darray.h>
#include <math.h>

#define min(a, b) \
    ({__typeof__ (a) _a = (a); \
      __typeof__(b) _b = (b); \
      _a > _b ? _b : _a;})

typedef int (*DArray_compare)(const void *a, const void *b);

int DArray_gnu_qsort(DArray *array, DArray_compare cmp);

int DArray_qsort(DArray *array, DArray_compare cmp);

int __DArray_qsort(DArray *array, int lo, int hi, DArray_compare cmp);

int __DArray_qsort_partition(DArray *array, int lo, int hi, DArray_compare cmp);

int DArray_heapsort(DArray *array, DArray_compare cmp);

void __DArray_heapsort_hepify(DArray *array, DArray_compare cmp);

void __DArray_heapsort_siftdown(DArray *array, int start, int end, DArray_compare cmp);

int DArray_mergesort(DArray *array, DArray_compare cmp);

void __DArray_bottom_up_mergesort(DArray *A, DArray *B, DArray_compare cmp);

void __DArray_bottom_up_mergesort_merge(DArray *A, int iLeft, int iRight, int iEnd, DArray *B, DArray_compare cmp);

int DArray_sort_add(DArray *array, DArray_compare cmp, void *el);

int DArray_find(DArray *array, DArray_compare cmp, void *to_find);
#endif
