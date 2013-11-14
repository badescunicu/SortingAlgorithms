#include <stdio.h>
#include <stdlib.h>

#define GAPS_SIZE 8

/* Swap 2 values */
void swap(int *a, int *b) {
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

/* Print an array */
void print(int *v, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void bubble_sort(int *to_sort, int size) {
    int i, swapped = 1;

    while (swapped) {
        swapped = 0;
        for (i = 0; i < size - 1; i++) {
            if (to_sort[i] > to_sort[i + 1]) {
                swapped = 1;
                swap(&to_sort[i], &to_sort[i + 1]);
            }
        }
    }
}

/* Insertion sort, as shown in "Introduction to Algorithms" */
void insertion_sort(int *to_sort, int size) {
    int i, j;
    for (j = 1; j < size; j++) {
        for (i = j - 1; i >= 0; i--) {
            if (to_sort[i + 1] < to_sort[i]) {
                swap(&to_sort[i], &to_sort[i + 1]);
            }
        }
    }
}

/* Shellsort using Ciura's gaps */
void shellsort(int *to_sort, int size) {
    int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};
    int gap_iterator, i, j;
    /* Cycle through gaps */
    for (gap_iterator = 0; gap_iterator < GAPS_SIZE; gap_iterator++) {
        /* Perform an Insertion Sort for each gap */
        for (i = gaps[gap_iterator]; i < size; i++) {
            for (j = i; j >= gaps[gap_iterator]; j--) {
                if (to_sort[j] < to_sort[j - gaps[gap_iterator]]) {
                    swap(&to_sort[j], &to_sort[j - gaps[gap_iterator]]);
                }
            }
        }
    }
}

int main() {
    int *v = malloc(10 * sizeof(int));
    int i, j;
    for (i = 0, j = 10; i < 10; i++)
        v[i] = j--;
    v[6] = -4;
    v[8] = -1;
    print(v, 10);
    shellsort(v, 10);
    print(v, 10);

    return 0;
}

