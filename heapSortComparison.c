#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

long long comparison_count = 0;

void generate_data(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
    }
}

void copy_array(int* src, int* dest, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

int is_sorted(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

void max_heapify(int* arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        comparison_count++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }

    if (right < n) {
        comparison_count++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        max_heapify(arr, n, largest);
    }
}

void heap_sort(int* arr, int n) {
    comparison_count = 0;
    for (int i = n / 2 - 1; i >= 0; i--) {
        max_heapify(arr, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        max_heapify(arr, i, 0);
    }
}

void merge(int* arr, int left, int mid, int right, int* temp) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        comparison_count++;
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

void merge_sort_rec(int* arr, int left, int right, int* temp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_rec(arr, left, mid, temp);
        merge_sort_rec(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

void merge_sort(int* arr, int n) {
    comparison_count = 0;
    int* temp = (int*)malloc(n * sizeof(int));
    merge_sort_rec(arr, 0, n - 1, temp);
    free(temp);
}

int partition(int* arr, int low, int high, int pivot_type) {
    int pivot_idx = low;
    if (pivot_type == 1) {
        pivot_idx = low;
    }
    else if (pivot_type == 2) {
        pivot_idx = low + rand() % (high - low + 1);
    }
    else if (pivot_type == 3) {
        int mid = low + (high - low) / 2;
        comparison_count++;
        int a = arr[low], b = arr[mid], c = arr[high];
        comparison_count++;
        if ((a <= b && b <= c) || (c <= b && b <= a)) pivot_idx = mid;
        else {
            comparison_count++;
            if ((b <= a && a <= c) || (c <= a && a <= b)) pivot_idx = low;
            else pivot_idx = high;
        }
    }

    int pivot = arr[pivot_idx];
    int temp = arr[pivot_idx];
    arr[pivot_idx] = arr[high];
    arr[high] = temp;

    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        comparison_count++;
        if (arr[j] <= pivot) {
            i++;
            int t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }
    int t = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = t;
    return i + 1;
}

void quick_sort_rec(int* arr, int low, int high, int pivot_type) {
    if (low < high) {
        int pi = partition(arr, low, high, pivot_type);
        quick_sort_rec(arr, low, pi - 1, pivot_type);
        quick_sort_rec(arr, pi + 1, high, pivot_type);
    }
}

void quick_sort(int* arr, int n, int pivot_type) {
    comparison_count = 0;
    quick_sort_rec(arr, 0, n - 1, pivot_type);
}

int main() {
    srand(time(NULL));
    int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int num_sizes = 6;

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int* original = (int*)malloc(n * sizeof(int));
        int* work = (int*)malloc(n * sizeof(int));
        generate_data(original, n);

        printf("Data Size : %d\n\n", n);
        printf("%-15s %-14s %-11s %-15s\n", "Algorithm", "Comparisons", "Time(ms)", "Memory(Bytes)");
        printf("--------------------------------------------------------\n");

        copy_array(original, work, n);
        clock_t start = clock();
        heap_sort(work, n);
        clock_t end = clock();
        long long heap_comp = comparison_count;
        double heap_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        size_t heap_mem = sizeof(int) * 2 + sizeof(void*) * 4 + (size_t)(log2(n) * 32);

        copy_array(original, work, n);
        start = clock();
        merge_sort(work, n);
        end = clock();
        long long merge_comp = comparison_count;
        double merge_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        size_t merge_mem = (n * sizeof(int)) + (size_t)(log2(n) * 32);

        copy_array(original, work, n);
        start = clock();
        quick_sort(work, n, 1);
        end = clock();
        long long q1_comp = comparison_count;
        double q1_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        size_t q_mem = (size_t)(log2(n) * 32);

        copy_array(original, work, n);
        start = clock();
        quick_sort(work, n, 2);
        end = clock();
        long long q2_comp = comparison_count;
        double q2_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

        copy_array(original, work, n);
        start = clock();
        quick_sort(work, n, 3);
        end = clock();
        long long q3_comp = comparison_count;
        double q3_time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

        printf("%-15s %-14lld %-11.3f %-15zu\n", "Heap Sort", heap_comp, heap_time, heap_mem);
        printf("%-15s %-14lld %-11.3f %-15zu\n", "Merge Sort", merge_comp, merge_time, merge_mem);
        printf("%-15s %-14lld %-11.3f %-15zu\n", "Quick (First)", q1_comp, q1_time, q_mem);
        printf("%-15s %-14lld %-11.3f %-15zu\n", "Quick (Random)", q2_comp, q2_time, q_mem);
        printf("%-15s %-14lld %-11.3f %-15zu\n\n", "Quick (Median3)", q3_comp, q3_time, q_mem);

        free(original);
        free(work);
    }
    return 0;
}