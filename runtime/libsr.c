#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gc/gc.h>

void _8__core___5print_i(int32_t i)
{
    printf("%i\n", i);
}

void _8__core___5print_b(int8_t i)
{
    if (i)
        printf("true\n");
    else
        printf("false\n");
}

void _8__core___5print_c(uint32_t c)
{
    printf("%c\n", (unsigned char) c);
}

void _8__core___5print_d(double d)
{
    printf("%g\n", d);
}

void _8__core___5print_l(int64_t l)
{
    printf("%lli\n", l);
}

void _8__core___5print_ul(uint64_t u)
{
    printf("%llu\n", u);
}

void __4exit_i(int32_t code)
{
    exit(code);
}

void* _8__core___6Object_new(void)
{
    return GC_malloc(sizeof(void**));
}

void _8__core___6Object11constructor(void** obj)
{
  *obj = NULL;
}
