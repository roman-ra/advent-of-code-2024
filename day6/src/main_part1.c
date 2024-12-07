#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH (256u + 1u)
#define MAX_SIZE_X       256u
#define MAX_SIZE_Y       256u
#define MAX_STEPS        99999u

#define CALC_OFFSET(x, y, size_x) ((y) * (size_x) + (x))

static inline bool is_cell_free(char *map, uint32_t x, uint32_t y, uint32_t size_x) {
    if (map[CALC_OFFSET(x, y, size_x)] == '#') {
        return false;
    }
    return true;
}

static inline void check_mark_visited(bool *map_visited, uint32_t x, uint32_t y, uint32_t size_x, uint32_t *unique_cell_ctr) {
    if (map_visited[CALC_OFFSET(x, y, size_x)]) {
        return;
    }
    map_visited[CALC_OFFSET(x, y, size_x)] = true;
    (*unique_cell_ctr)++;
}

bool guard_step(char *map, bool *map_visited, uint32_t *guard_x, uint32_t *guard_y, 
                uint32_t size_x, uint32_t size_y, uint32_t *unique_cell_ctr)
{
    const char guard_dir = map[CALC_OFFSET(*guard_x, *guard_y, size_x)];
    check_mark_visited(map_visited, *guard_x, *guard_y, size_x, unique_cell_ctr);
    switch (guard_dir) {
        case '^':
            if (*guard_y == 0u) {
                return false;
            } else if (is_cell_free(map, *guard_x, *guard_y - 1u, size_x)) {
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = '.';
                (*guard_y)--;
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = guard_dir;
            } else {
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = '>';
            }
            return true;
        case '>':
            if (*guard_x == size_x - 1) {
                return false;
            } else if (is_cell_free(map, *guard_x + 1u, *guard_y, size_x)) {
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = '.';
                (*guard_x)++;
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = guard_dir;
            } else {
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = 'V';
            }
            return true;
        case 'V':
            if (*guard_y == size_y - 1) {
                return false;
            } else if (is_cell_free(map, *guard_x, *guard_y + 1u, size_x)) {
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = '.';
                (*guard_y)++;
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = guard_dir;
            } else {
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = '<';
            }
            return true;
        case '<':
            if (*guard_x == 0u) {
                return false;
            } else if (is_cell_free(map, *guard_x - 1u, *guard_y, size_x)) {
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = '.';
                (*guard_x)--;
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = guard_dir;
            } else {
                map[CALC_OFFSET(*guard_x, *guard_y, size_x)] = '^';
            }
            return true;
        default:
            return false;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    FILE *in = fopen(argv[1], "r");
    
    char line[MAX_LINE_LENGTH];
    char *map         = malloc(MAX_SIZE_X * MAX_SIZE_Y);
    bool *map_visited = malloc(MAX_SIZE_X * MAX_SIZE_Y * sizeof(bool));
    memset(map_visited, 0, MAX_SIZE_X * MAX_SIZE_Y * sizeof(bool));
    uint32_t size_x = 0u, size_y = 0u;

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
        memcpy(&map[CALC_OFFSET(0, size_y, size_x)], line, size_x);
        size_y++;
    }
    
    uint32_t guard_x = 0u;
    uint32_t guard_y = 0u;
    for (uint32_t y = 0u; y < size_y; y++) {
        for (uint32_t x = 0u; x < size_x; x++) {
            if (map[CALC_OFFSET(x, y, size_x)] == '^') {
                guard_x = x;
                guard_y = y;
                break;
            }
        }   
    }

    uint32_t unique_cell_ctr = 1u;
    uint32_t iter_ctr = 0u;
    map_visited[CALC_OFFSET(guard_x, guard_y, size_x)] = true;
    while (iter_ctr < MAX_STEPS)
    {
        if(!guard_step(map, map_visited, &guard_x, &guard_y, size_x, size_y, &unique_cell_ctr))
        {
            break;
        }
        iter_ctr++;
    }
    
    printf("Number of visited cells = %u", unique_cell_ctr);

    free(map);
    fclose(in);

    return 0;
}