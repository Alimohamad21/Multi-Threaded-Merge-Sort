#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct arguments arguments;
struct arguments {
    int *array;
    int left;
    int right;
}; // struct to hold argument of merge sort function

int *readArrayFromFile(char *fileName);  // file to read input array from file

int getInputArraySize(char *fileName);   // function to get the length of the input array

void merge(int *arr, int l, int m, int r);

void *mergeSort(void *args);  // threaded merge sort

void sort();  // function which asks for file name of array and prints out sorted array

int main() {
    sort();
}

void sort() {
    char fileName[20];
    printf("Please enter input file name:");
    scanf("%s", fileName);
    arguments args;
    args.array = readArrayFromFile(fileName);
    int n = getInputArraySize(fileName);
    printf("\nUNSORTED ARRAY: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", args.array[i]);
    }
    args.left = 0;
    args.right = n - 1;
    mergeSort(&args);
    printf("\nSORTED ARRAY: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", args.array[i]);
    }
    printf("\n");
}

int getInputArraySize(char *fileName) {
    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        printf("Error opening file or file doesn't exist\n");
        exit(0);
    }
    int n;
    fscanf(fp, "%d", &n);
    return n;
}

int *readArrayFromFile(char *fileName) {
    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        printf("Error opening file or file doesn't exist\n");
        exit(0);
    }
    int n;
    fscanf(fp, "%d", &n);
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d", &arr[i]);
    }
    return arr;
}


void merge(int *arr, int l, int m, int r) {
    int i, j, k;
    int nLeft = m - l + 1;
    int nRight = r - m;
    int leftArr[nLeft], rightArr[nRight];
    for (i = 0; i < nLeft; i++)
        leftArr[i] = arr[l + i];
    for (j = 0; j < nRight; j++)
        rightArr[j] = arr[m + 1 + j];
    i = 0;
    j = 0;
    k = l;
    while (i < nLeft && j < nRight) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    while (i < nLeft) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    while (j < nRight) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}


void *mergeSort(void *args) {
    pthread_t th[2];
    arguments *arrayArgs = args;
    arguments arrayArgs1;
    arrayArgs1.array = arrayArgs->array;
    arguments arrayArgs2;
    arrayArgs2.array = arrayArgs->array;  // had to store arguments in two separate variables one for each sub array because threads overwrote each other's arguments
    int l = arrayArgs->left;
    int r = arrayArgs->right;
    if (l < r) {
        int m = l + (r - l) / 2;
        arrayArgs1.left = l;
        arrayArgs1.right = m;
        arrayArgs2.left = m + 1;
        arrayArgs2.right = r;
        pthread_create(&th[0], NULL, &mergeSort, &arrayArgs1);
        pthread_create(&th[1], NULL, &mergeSort, &arrayArgs2);
        for (int i = 0; i < 2; i++)
            pthread_join(th[i], NULL);
        merge(arrayArgs->array, l, m, r);
    }
    return NULL;
}
