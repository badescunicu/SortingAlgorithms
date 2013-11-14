#include <stdio.h>
#include <stdlib.h>

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

int main() {
    int *v = malloc(10 * sizeof(int));
    int i, j;
    for (i = 0, j = 10; i < 10; i++)
        v[i] = j--;
    print(v, 10);
    bubble_sort(v, 10);
    print(v, 10);

    return 0;
}

