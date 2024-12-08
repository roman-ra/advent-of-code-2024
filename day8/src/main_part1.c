#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH (256u + 1u)
#define MAX_SIZE_X       256u
#define MAX_SIZE_Y       256u

#define CALC_OFFSET(x, y, size_x) ((y) * (size_x) + (x))

typedef struct {
    char antenna_freq;
    bool is_antinode;
} cell_t;

static inline bool coordinates_valid(int32_t x, int32_t y, uint32_t size_x, uint32_t size_y)
{
    return ((x >= 0) && (x < (int32_t)size_x) && (y >= 0) && (y < (int32_t)size_y));
}

static inline void identify_antinodes_for_antenna(cell_t *map, uint32_t x, uint32_t y, 
                                                  uint32_t size_x, uint32_t size_y)
{
    const char antenna_freq = map[CALC_OFFSET(x, y, size_x)].antenna_freq;
    /* Find antenna with the same frequency */
    for (uint32_t ay = 0u; ay < size_y; ay++) {
        for (uint32_t ax = 0u; ax < size_x; ax++) {
            if (((x == ax) && (y == ay)) || (map[CALC_OFFSET(ax, ay, size_x)].antenna_freq != antenna_freq)) {
                continue;
            }
            const int32_t diff_x = ax - x;
            const int32_t diff_y = ay - y;
            const int32_t antinode1_x = ax + diff_x;
            const int32_t antinode1_y = ay + diff_y;
            const int32_t antinode2_x = x - diff_x;
            const int32_t antinode2_y = y - diff_y;
            if (coordinates_valid(antinode1_x, antinode1_y, size_x, size_y)) {
                map[CALC_OFFSET(antinode1_x, antinode1_y, size_x)].is_antinode = true;
            }
            if (coordinates_valid(antinode2_x, antinode2_y, size_x, size_y)) {
                map[CALC_OFFSET(antinode2_x, antinode2_y, size_x)].is_antinode = true;
            }
        }
    }
}

static void identify_antinodes(cell_t *map, uint32_t size_x, uint32_t size_y)
{
    for (uint32_t y = 0u; y < size_y; y++) {
        for (uint32_t x = 0u; x < size_x; x++) {
            if (map[CALC_OFFSET(x, y, size_x)].antenna_freq != 0) {
                identify_antinodes_for_antenna(map, x, y, size_x, size_y);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    FILE *in = fopen(argv[1], "r");
    
    char line[MAX_LINE_LENGTH];
    cell_t *map = malloc(MAX_SIZE_X * MAX_SIZE_Y * sizeof(cell_t));
    uint32_t size_x, size_y;

    while (!feof(in)) {
        fgets(line, MAX_LINE_LENGTH, in);
        size_t line_len = strlen(line);
        if ((line_len > 0) && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
            line_len--;
        }
        if (size_x == 0u) {
            size_x = line_len;
        }
        for (uint32_t x = 0u; x < size_x; x++) {
            map[CALC_OFFSET(x, size_y, size_x)].antenna_freq = (line[x] != '.') ? line[x] : 0;
            map[CALC_OFFSET(x, size_y, size_x)].is_antinode = false;
        }
        size_y++;
    }
    
    identify_antinodes(map, size_x, size_y);
   
    uint32_t num_antinodes = 0u;
    for (uint32_t i = 0u; i < size_x * size_y; i++) {
        if (map[i].is_antinode) {
            num_antinodes++;
        }
    }
    
    printf("Number of antinodes = %u", num_antinodes);

    free(map);
    fclose(in);

    return 0;
}