#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


typedef void (*rd)();

//static rd random_orig;

extern "C" long int random()
{
    printf("random\n");
    return 0;
}

extern "C" int rand()
{
    printf("rand\n");
    return 0;
}

extern "C" int rand_r(unsigned int * seed)
{
    printf("rand_r\n");
    return 0;
}

extern "C" int random_r(struct random_data *buf, int32_t *result)
{
    printf("random_r\n");
    (*result) = 0;
    return 0;
}
