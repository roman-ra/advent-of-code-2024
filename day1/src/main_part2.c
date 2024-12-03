#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h> // memset

typedef uint32_t location_id_t;

#define MAX_NUM_LOCATION_IDS (1000u)
#define MAX_LOCATION_ID      (99999u)

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    FILE *in = fopen(argv[1], "r");
    location_id_t *location_ids_left  = malloc(sizeof(location_id_t) * MAX_NUM_LOCATION_IDS);
    uint16_t      *frequencies_right  = malloc(sizeof(location_id_t) * (MAX_LOCATION_ID + 1u));
    memset(frequencies_right, 0, sizeof(location_id_t) * (MAX_LOCATION_ID + 1u));

    uint32_t num_location_ids = 0u;

    while ((!feof(in)) && (num_location_ids < MAX_NUM_LOCATION_IDS)) {
        uint32_t right_location_id = 0u;
        fscanf(in, "%u   %u", &location_ids_left[num_location_ids], &right_location_id);
        num_location_ids++;
        frequencies_right[right_location_id]++;
    }

    uint64_t similarity_score = 0u;

    for (uint32_t i = 0u; i < num_location_ids; i++) {
        similarity_score += (location_ids_left[i] * frequencies_right[location_ids_left[i]]);
    }

    printf("Similarity score = %llu", similarity_score);

    free(location_ids_left);
    free(frequencies_right);
    fclose(in);

    return 0;
}