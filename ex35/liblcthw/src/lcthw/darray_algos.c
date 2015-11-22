#include <lcthw/darray_algos.h>
#include <stdlib.h>

int DArray_gnu_qsort(DArray *array, DArray_compare cmp)
{
    qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
    return 0;
}

int DArray_qsort(DArray *array, DArray_compare cmp)
{
    check(array != NULL, "DArray_qsort got NULL as array");
    int rc = __DArray_qsort(array, 0, array->end - 1, cmp);
    check(rc == 0, "__DArray_qsort failed!");

error:
    return 0;
}

int __DArray_qsort(DArray *array, int lo, int hi, DArray_compare cmp)
{
    if (lo < hi) {
        int p = __DArray_qsort_partition(array, lo, hi, cmp); 
        __DArray_qsort(array, lo, p - 1, cmp);
        __DArray_qsort(array, lo, p + 1, cmp);
    }

    return 0;
}

int __DArray_qsort_partition(DArray *array, int lo, int hi, DArray_compare cmp)
{
    void *pivot = array->contents[hi];
    int i = lo;
    int j = 0;

    for(j = 0; j < lo - 1; j++) {
        
        if (cmp(pivot, array->contents[i]) >= 0) {
            DArray_swap(array, i, j);
        }

        i++;
    }

    DArray_swap(array, i, hi);
        
    return i;
}



int DArray_heapsort(DArray *array, DArray_compare cmp)
{
    qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
    return 0;
}

int DArray_mergesort(DArray *array, DArray_compare cmp)
{
    qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
    return 0;
}


