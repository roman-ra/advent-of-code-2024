#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint32_t location_id_t;

#define MAX_NUM_LOCATION_IDS (1000u)

int comp(const void* a, const void* b)
{
    location_id_t arg1 = *(const location_id_t*)a;
    location_id_t arg2 = *(const location_id_t*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    FILE *in = fopen(argv[1], "r");
    location_id_t *location_ids_left  = malloc(sizeof(location_id_t) * MAX_NUM_LOCATION_IDS);
    location_id_t *location_ids_right = malloc(sizeof(location_id_t) * MAX_NUM_LOCATION_IDS);
    uint32_t num_location_ids = 0u;

    while ((!feof(in)) && (num_location_ids < MAX_NUM_LOCATION_IDS)) {
        fscanf(in, "%u   %u", &location_ids_left[num_location_ids], &location_ids_right[num_location_ids]);
        num_location_ids++;
    }

    qsort(location_ids_left,  num_location_ids, sizeof(location_id_t), comp);
    qsort(location_ids_right, num_location_ids, sizeof(location_id_t), comp);

    uint64_t total_diff = 0u;

    for (uint32_t i = 0u; i < num_location_ids; i++) {
        if (location_ids_left[i] < location_ids_right[i]) {
            total_diff += (location_ids_right[i] - location_ids_left[i]);
        } else {
            total_diff += (location_ids_left[i] - location_ids_right[i]);
        }
    }

    printf("Total diff = %llu", total_diff);

    return 0;
}