#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pasien {
    int id;
    char nama[100];
    int usia;
    char keluhan[100];
    char has
    struct pasien* prev;
    struct pasien* next;
} Pasien;