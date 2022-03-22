#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define ARRAY_SIZE(a)sizeof(a)/sizeof(a[0])

#define TIME_TEST(testCode, time) { \
    clock_t start_time = clock () ; \
    testCode \
        clock_t end_time = clock () ;\
    clock_t sort_time = end_time - start_time ; \
 time = ( double ) sort_time / CLOCKS_PER_SEC ; \
 }

// функция сортировки
typedef struct SortFunc {
    void (*sort )(int *a, size_t n); // указатель на функцию
    // сортировки
    char name[64];                   // имя сортировки,
    // используемое при выводе
} SortFunc;

// функция генерации
typedef struct GeneratingFunc {
    void (*generate )(int *a, size_t n);  // указатель на функцию
    // генерации последоват.
    char name[64];                            // имя генератора,
    // используемое при выводе
} GeneratingFunc;

// функция сортировки
typedef struct nCompSort {
    long long (*nComp)(int *a, size_t n); // указатель на функцию
    // сортировки
    char name[64];                   // имя сортировки,
    // используемое при выводе
} nCompSort;

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

bool isOrdered(const int *a, size_t size) {
    for (int i = 1; i < size - 1; i++) {
        if (a[i - 1] > a[i])
            return false;
    }
    return true;
}

void inputArray(int *a, const int n) {
    for (size_t i = 0; i < n; i++)
        scanf("%d", &a[i]);
}

void outputArray(const int *a, const int n) {
    for (size_t i = 0; i < n; i++)
        printf("%d\n", a[i]);
}

void generateRandomArray(int *a, size_t size) {
    srand(time(0));
    for (int i = 0; i < size; i++) {
        a[i] = 100000 - rand() % 100000;
    }
}

int cmpReverse(const void *a, const void *b) {
    return *(const int *) b - *(const int *) a;
}

int cmp(const void *a, const void *b) {
    return *(const int *) a - *(const int *) b;
}

void generateOrderedArray(int *a, size_t size) {
    srand(time(0));
    for (int i = 0; i < size; i++) {
        a[i] = 100000 - rand() % 100000;
    }
    qsort(a, size, sizeof(int), cmp);
}

void generateOrderedBackwards(int *a, size_t size) {
    srand(time(0));
    for (int i = 0; i < size; i++) {
        a[i] = 100000 - rand() % 100000;
    }
    qsort(a, size, sizeof(int), cmpReverse);
}

double getTime() {
    clock_t start_time = clock();
    // фрагмент кода
    // время которого измеряется
    clock_t end_time = clock();\
    clock_t sort_time = end_time - start_time;
    return (double) sort_time / CLOCKS_PER_SEC;
}

void checkTime(void (*sortFunc )(int *, size_t),
               void (*generateFunc )(int *, size_t),
               size_t size, char *experimentName) {
    static size_t runCounter = 1;

    // генерация последовательности
    static int innerBuffer[100000];
    generateFunc(innerBuffer, size);
    printf("Run #%zu| ", runCounter++);
    printf(" Name : %s\n", experimentName);
// замер времени
    double time;
    TIME_TEST ({ sortFunc(innerBuffer, size); }, time);
    // результаты замера
    printf(" Status : ");
    if (isOrdered(innerBuffer, size)) {
        printf("OK! Time : %.3f s.\n", time);

        // запись в файл
        char filename[256];
        sprintf(filename, "./data/%s.csv", experimentName);
        FILE *f = fopen(filename, "a");
        if (f == NULL) {
            printf(" FileOpenError %s", filename);
            exit(1);
        }
        fprintf(f, "%zu; %.3f\n", size, time);
        fclose(f);
    } else {
        printf(" Wrong !\n");

        // вывод массива, который не смог быть отсортирован
        outputArray(innerBuffer, size);

        exit(1);
    }
}

void selectionSort(int *a, const int n) {
    for (int i = 0; i < n; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[minIndex])
                minIndex = j;
        }
        swap(&a[i], &a[minIndex]);
    }
}

long long getNCmpsOfSelectionSort(int *array, const size_t size) {
    long long countComp = 0;
    for (int i = 0; i < size - 1 && ++countComp; i++) {
        int minPos = i;
        for (int j = i + 1; j < size && ++countComp; j++)
            if (array[j] < array[minPos] && ++countComp)
                minPos = j;
        swap(&array[i], &array[minPos]);
    }
    return countComp;
}

void shellSort(int *a, int length) {
    unsigned step = length / 2, compare = 0, swap = 0;
    while (step > 0) {
        for (int i = 0; i < length; i++) {
            int j = i;
            int temp = a[j];
            compare++;
            while (j >= step && a[j - step] > temp) {
                compare++;
                a[j] = a[j - step];
                j = j - step;
                swap++;
            }
            a[j] = temp;
        }
        step /= 2;
    }
}

long long getNCmpsOfShell(int *array, const size_t size) {
    long long countComp = 0;
    for (size_t step = size / 2; step > 0 && ++countComp; step /= 2)
        for (size_t i = step; i < size && ++countComp; i++) {
            int tmp = array[i];
            size_t j;
            for (j = i; j >= step && ++countComp; j -= step) {
                if (tmp < array[j - step] && ++countComp)
                    array[j] = array[j - step];
                else
                    break;
            }
            array[j] = tmp;
        }
    return countComp;
}

void bubbleSort(int *a, size_t size) {
    for (int i = 0; i < size - 1; i++)
        for (int j = size - 1; j > i; j--)
            if (a[j - 1] > a[j])
                swap(&a[j - 1], &a[j]);
}

long long getNCmpsOfBubble(int *array, const size_t size) {
    long long countComp = 0;
    for (size_t i = 0; i < size - 1 && ++countComp; i++)
        for (size_t j = size - 1; j > i && ++countComp; j--)
            if (array[j - 1] > array[j] && ++countComp)
                swap(&array[j - 1], &array[j]);

    return countComp;
}

void insertionSort(int *a, size_t size) {
    for (size_t i = 1; i < size; i++) {
        int t = a[i];
        int j = i;
        while (j > 0 && a[j - 1] > t) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = t;
    }
}

long long getNCmpsOfIntersections(int *array, const size_t size) {
    long long countComp = 0;
    for (size_t i = 1; i < size && ++countComp; i++) {
        int t = array[i];
        size_t j = i;
        while (j > 0 && ++countComp && array[j - 1] > t && ++countComp) {
            array[j] = array[j - 1];
            j--;
        }
        array[j] = t;
    }
    return countComp;
}

void combSort(int *a, const size_t size) {
    size_t step = size;
    int swapped = 1;
    while (step > 1 || swapped) {
        if (step > 1)
            step /= 1.24733;
        swapped = 0;
        for (size_t i = 0, j = i + step; j < size; ++i, ++j)
            if (a[i] > a[j]) {
                swap(&a[i], &a[j]);
                swapped = 1;
            }
    }
}

long long getNCmpsOfComb(int *array, const size_t size) {
    size_t step = size;
    int swapped = 1;
    long long countComp = 0;
    while (step > 1 && ++countComp || swapped && ++countComp) {
        if (step > 1 && ++countComp)
            step /= 1.24733;
        swapped = 0;
        for (size_t i = 0, j = i + step; j < size && ++countComp; ++i, ++j)
            if (array[i] > array[j] && ++countComp) {
                swap(&array[i], &array[j]);
                swapped = 1;
            }
    }
    return countComp;
}

int digit(int n, int k, int N, int M) {
    return (n >> (N * k) & (M - 1));
}

void _radixSort(int *l, int *r, int N) {
    int k = (32 + N - 1) / N;
    int M = 1 << N;
    int sz = r - l;
    int *b = (int *) malloc(sizeof(int) * sz);
    int *c = (int *) malloc(sizeof(int) * M);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < M; j++)
            c[j] = 0;

        for (int *j = l; j < r; j++)
            c[digit(*j, i, N, M)]++;

        for (int j = 1; j < M; j++)
            c[j] += c[j - 1];

        for (int *j = r - 1; j >= l; j--)
            b[--c[digit(*j, i, N, M)]] = *j;

        int cur = 0;
        for (int *j = l; j < r; j++)
            *j = b[cur++];
    }
    free(b);
    free(c);
}

void radixSort(int *a, size_t n) {
    _radixSort(a, a + n, 8);
}

void getMinMax(const int *a, size_t size, int *min, int *max) {
    *min = a[0];
    *max = a[0];
    for (int i = 1; i < size; i++) {
        if (a[i] < *min)
            *min = a[i];
        else if (a[i] > *max)
            *max = a[i];
    }
}

void checkNComp(long long (*nComp )(int *a, size_t n),
                void (*generateFunc)(int *, size_t),
                size_t size, char *experimentName, char *name) {
    static size_t runCounter = 1;

    // генерация последовательности
    static int innerBuffer[100000];
    generateFunc(innerBuffer, size);
    printf("Run #%zu| ", runCounter++);
    printf("Name: %s\n", experimentName);

    // замер времени
    long long nComps = nComp(innerBuffer, size);

    // результаты замера
    printf("Status: ");
    if (isOrdered(innerBuffer, size)) {
        printf("OK! Comps: %lld\n", nComps);

        // запись в файл
        char filename[256];
        sprintf(filename, "./data/comps/%s.csv", experimentName);
        FILE *f = fopen(filename, "a");
        if (f == NULL) {
            printf("FileOpenError %s", filename);
            exit(1);
        }
        fprintf(f, "%zu; %lld\n", size, nComps);
        fclose(f);
    } else {
        printf("Wrong!\n");

        // вывод массива, который не смог быть отсортирован
        outputArray(innerBuffer, size);

        exit(1);
    }
}

void timeExperiment() {
    // описание функций сортировки
    SortFunc sorts[] = {
            {selectionSort, "selectionSort"},
            {insertionSort, "insertionSort"},
            {shellSort,     "Shellsort"},
            {combSort,      "combSort"},
            {bubbleSort,    "bubbleSort"},
            {radixSort,     "radixSort"}
    };
    const unsigned FUNCS_N = ARRAY_SIZE (sorts);

    nCompSort nComps[] = {
            {getNCmpsOfSelectionSort, "getNCmpsOfSelectionSort"},
            {getNCmpsOfIntersections, "getNCmpsOfIntersections"},
            {getNCmpsOfBubble,        "getNCmpsOfBubble"},
            {getNCmpsOfComb,          "combSortN"},
            {getNCmpsOfShell,         "getNCmpsOfShell"},
    };

    const unsigned COMPS_N = ARRAY_SIZE(nComps);

    GeneratingFunc generatingFuncs[] = {
            // генерируется случайный массив
            {generateRandomArray,      " random "},
            // генерируется массив 0, 1, 2, ..., n - 1
            {generateOrderedArray,     " ordered "},
            // генерируется массив n - 1, n - 2, ..., 0
            {generateOrderedBackwards, " orderedBackwards "}
    };
    const unsigned CASES_N = ARRAY_SIZE
                             (generatingFuncs);

    // запись статистики в файл
    for (size_t size = 10000; size <= 100000; size += 10000) {
        printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
        printf(" Size : %d\n", size);
        for (int i = 0; i < FUNCS_N; i++) {
            for (int j = 0; j < CASES_N; j++) {
                // генерация имени файла
                static char filename[128];
                sprintf(filename, "%s_%s_time ",
                        sorts[i].name, generatingFuncs[j].name);
                checkTime(sorts[i].sort,
                          generatingFuncs[j].generate,
                          size, filename);
            }
        }
        printf("\n");
    }


// запись статистики в файл
    for (size_t size = 10000; size <= 100000; size += 10000) {
        printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
        printf("Size : %zu\n", size);
        for (int i = 0; i < COMPS_N; i++) {
            for (int j = 0; j < CASES_N; j++) {
                // генерация имени файла
                static char filename[128];
                sprintf(filename, "%s_%s_comps",
                        nComps[i].name, generatingFuncs[j].name);
                checkNComp(nComps[i]
                                   .nComp,
                           generatingFuncs[j].generate,
                           size, filename, nComps[i].name);
            }
        }
        printf("\n");
    }
}


int main() {
    timeExperiment();
    return 0;
}
