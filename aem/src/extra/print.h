#ifndef PRINT_H
#define PRINT_H

#include <cstdio>
#include <stdlib.h>

#define PRINT(...) do { printf("====AEM> "); printf(__VA_ARGS__); printf("\n"); } while (0)

#endif
