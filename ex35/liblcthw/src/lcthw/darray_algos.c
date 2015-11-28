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
    check(cmp != NULL, "DArray_qsort got NULL as DArray_compare function.");
    if (array->end == 1) return 0;
 
    int rc = __DArray_qsort(array, 0, array->end - 1,cmp);
    check(rc == 0, "__DArray_qsort failed!");

    return 0;
error:
    return 1;
}

int __DArray_qsort(DArray *array, int lo, int hi, DArray_compare cmp)
{
    if (lo < hi) {
        int p = __DArray_qsort_partition(array, lo, hi, cmp); 
        __DArray_qsort(array, lo, p - 1, cmp);
        __DArray_qsort(array, p + 1, hi, cmp);
    }

    return 0;
}

int __DArray_qsort_partition(DArray *array, int lo, int hi, DArray_compare cmp)
{
    void *pivot = &array->contents[hi];
    int i = lo;
    int j = 0;

    for(j = lo; j <= hi - 1; j++) {

        if (cmp(&array->contents[j], pivot) <= 0) {
            DArray_swap(array, i, j);
            i++;
        }       

    }

    DArray_swap(array, i, hi);

    return i;
}

int DArray_heapsort(DArray *array, DArray_compare cmp)
{
    check(array != NULL, "DArray_heapsort got NULL as DArray");
    check(cmp != NULL, "DArray_heapsort got NULL as Darray_compare function");
    if (array->end == 1) return 0;

    __DArray_heapsort_hepify(array, cmp);
    
    int end = array->end - 1;

    while (end > 0) {
        DArray_swap(array, 0, end);
        end--;
        __DArray_heapsort_siftdown(array, 0, end, cmp);
    }
    
    return 0;
error:
    return 1;
}

void __DArray_heapsort_hepify(DArray *array, DArray_compare cmp)
{

    int start = (int) floor(((double)array->end - 2) / 2);

    while (start >= 0) {
        __DArray_heapsort_siftdown(array, start, array->end - 1, cmp);
        start--;
    }

}

void __DArray_heapsort_siftdown(DArray *array, int start, int end, DArray_compare cmp)
{
    int root = start;
    int child = 0;
    int swap = 0;
    
    while((root * 2) + 1 <= end) {
        child = (root * 2) + 1;
        swap = root;
    
        if (cmp(&array->contents[swap], &array->contents[child]) < 0)
            swap = child;

        if (child + 1 <= end && cmp(&array->contents[swap], &array->contents[child+1]) < 0)
            swap = child + 1;
        
        if (swap == root) {
            return;
        } else {
            DArray_swap(array, root, swap);
            root = swap;
        }
    }
}

int DArray_mergesort(DArray *A, DArray_compare cmp)
{
    check(A != NULL, "DArray_mergesort got NULL as DAraay");
    check(cmp != NULL, "DArray_mergesort got NULL as DArray_compare");
    if (A->end == 1) return 0;

    // initialize array 
    DArray *B = DArray_create(A->element_size, A->max);
    DArray_copy(A, B);

    __DArray_bottom_up_mergesort(A, B, cmp);
    DArray_destroy(B);    
    return 0;
error:
    return 1;
}

void __DArray_bottom_up_mergesort(DArray *A, DArray *B, DArray_compare cmp)
{
    int width = 0;
    int i = 0;
    
    for (width = 1; width < A->end; width = 2 * width) {
        for(i = 0; i < A->end; i = i + 2 * width) {
            __DArray_bottom_up_mergesort_merge(A, i, min(i+width, A->end), min(i+2*width, A->end), B, cmp);
        }
        DArray_copy(B, A);
    }    
}

void __DArray_bottom_up_mergesort_merge(DArray *A, int iLeft, int iRight, int iEnd, DArray *B, DArray_compare cmp)
{
    int i0 = iLeft;
    int i1 = iRight;
    int j = 0;

    for(j = iLeft; j < iEnd; j++) {
        if(i0 < iRight && (i1 >= iEnd || cmp(&A->contents[i0], &A->contents[i1]) <= 0)) {
            B->contents[j] = A->contents[i0];
            i0++;

        } else {
            B->contents[j] = A->contents[i1];
            i1++;
        }
    } 
}
