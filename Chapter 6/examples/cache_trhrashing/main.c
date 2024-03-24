#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

# define L1_SIZE 32768
# define CACHELINE_SIZE 64
# define VEC_SIZE 1024 * 1024
# ifndef GAP
    # define GAP 1024 * CACHELINE_SIZE
# endif

void rdtsc(uint32_t* high, uint32_t* low)
{
    __asm__ __volatile__(
        "rdtsc;\n"
        "movl %%edx, %0;\n"
        "movl %%eax, %1;\n"
        : "=r" (*high), "=r" (*low) 
        // : "=r" (*low), "=r" (*high) 
        :
        : "%rax", "%rdx"
    );
}

uint64_t run(int *array1, int *array2, long long (func)(int* arr1, int* arr2))
{
    for (long long int i = 0; i < VEC_SIZE; i++)
    {
        array1[i] = i;
        array2[i] = VEC_SIZE - i;
    }
    uint32_t tsc_high = 0;
    uint32_t tsc_low = 0;
    rdtsc(&tsc_high, &tsc_low);

    func(array1, array2);
    
    uint32_t ntsc_high = 0;
    uint32_t ntsc_low = 0;
    rdtsc(&ntsc_high, &ntsc_low);

    return (((uint64_t) ntsc_high << 32) + ntsc_low) - (((uint64_t) tsc_high << 32) + tsc_low);
}

long long func(int* array1, int* array2)
{
    long long sum = 0;
    for (long long int i = 0; i < VEC_SIZE; i++)
    {
        sum += array1[i] * array2[i];
    }
    return sum;
}

int main(void)
{
    int *buffer = malloc(VEC_SIZE * 10 * sizeof(int));
    int *array1 = buffer;
    int *array2 = (void *) (array1 + VEC_SIZE) + GAP;
    assert((void *)(array2 + VEC_SIZE) < (void *)(buffer + VEC_SIZE * 3));

# ifdef DEBUG
    printf("arr1 %p\n", array1);
    printf("arr2 %p\n", array2);
    printf("gap %lx\n", (void *)array2 - (void *)array1);
# endif
    printf("%d, %d, ", VEC_SIZE, GAP);

    long long int runs = 0;
    for (int i =0; i < 100; i++)
        runs += run(array1, array2, func);

    printf("%lf, %lf\n", 
        (double) runs / 100,
        ((double) runs / 100) / 2.5 * 1e9);
    free(buffer);
    return 0;
}