/* C Dynamic Array (CDA) Usage examples:
 * Authored by usesc
 * MIT Licensure
 * 
 * test.c
 */

/* if you want to compile the dynamic array
 * only once over all uses then leave this
 * include alone, if you want every instance
 * to have its own dynamic array implementation,
 * then simply paste this:
 * #define CDA_IMPL
 * before the header file. Using this define
 * lets you compile the code without manually
 * generating the object file and linking it.
 */

#define CDA_IMPL
#include "cda.h"

#include <stdio.h>

int cnt = 1;
void printarr(int *arr, int size) {
        printf("\nARRAY PRINT #%d | SIZE: %d\n", cnt, size);
        for (int i = 0; i < size; i++) {
                printf("offset: %010d value: %d\n", i, arr[i]);
        }
        cnt++;
}

int main() {
        /* makes a dynamic array with 
         * a starting size of 8 elements
         * and assigns those 8 elements
         * to their offsets: 
         */

        int n = 8;
        int *arr = cdainit(int, n);

        /* the buffer cannot grow like this. vv 
         * however, if you want the buffer
         * to grow in this scenario, you would
         * use cdapush(arr, &i, 1);
         * IN FACT, if you want the buffer to
         * grow or shrink as you add or remove
         * elements, then you must use one of
         * the library's functions. 
         */

        for (int i = 0; i < n; i++) 
                cdaset(arr, i, i);
                /* near-equivalent to:
                 * arr[i] = i;
                 */
        printarr(arr, cdasize(arr));
        /* arr = {0, 1, 2, 3, 4, 5, 6, 7} */

        int arr2[] = {100, 200, 300};
        cdapush(arr, arr2, 3);
        printarr(arr, cdasize(arr));
        /* arr = {0, 1, 2, 3, 4, 5, 6, 7, 100, 200, 300} */

        int arr3[] = {400, 500, 600};
        cdains(arr, arr3, 3, 0);
        printarr(arr, cdasize(arr));
        /* arr = {400, 500, 600, 0, 1, 2, 3, 4, 5, 6, 7, 100, 200, 300} */

        /* lets remove the last 5 elements, null is the 
         * output pointer, we dont need that right now.
         */ 
        cdapop(arr, NULL, 5);
        printarr(arr, cdasize(arr));
        /* arr = {400, 500, 600, 0, 1, 2, 3, 4, 5} */

        /* lets remove the element at index 2 (value: 600) */
        cdatake(arr, NULL, 1, 2);
        printarr(arr, cdasize(arr));
        /* arr = {400, 500, 0, 1, 2, 3, 4, 5} */

        cdafree(arr);

        return 0;
}