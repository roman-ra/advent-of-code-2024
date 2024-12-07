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

static inline bool check_visited(char *map_dirs, char direction,
                                 uint32_t x, uint32_t y, uint32_t size_x) {
    if (map_dirs[CALC_OFFSET(x, y, size_x)] == direction) {
        return true;
    } else if (map_dirs[CALC_OFFSET(x, y, size_x)] == 0) {
        map_dirs[CALC_OFFSET(x, y, size_x)] = direction;
    }
    return false;
}

bool guard_step(char *map, char *map_dir, uint32_t *guard_x, uint32_t *guard_y, 
                uint32_t size_x, uint32_t size_y, bool *loop_found)
{
    const char guard_dir = map[CALC_OFFSET(*guard_x, *guard_y, size_x)];

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
            break;
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
            break;
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
            break;
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
            break;
        default:
            return false;
    }

    if (check_visited(map_dir, map[CALC_OFFSET(*guard_x, *guard_y, size_x)], *guard_x, *guard_y, size_x)) {
        *loop_found = true;
    }

    return true;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    FILE *in = fopen(argv[1], "r");
    
    char line[MAX_LINE_LENGTH];
    char *map         = malloc(MAX_SIZE_X * MAX_SIZE_Y);
    char *map_backup  = malloc(MAX_SIZE_X * MAX_SIZE_Y);
    char *map_dir     = malloc(MAX_SIZE_X * MAX_SIZE_Y);
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
        memcpy(&map_backup[CALC_OFFSET(0, size_y, size_x)], line, size_x);
        size_y++;
    }
    
    uint32_t guard_x = 0u;
    uint32_t guard_y = 0u;
    for (uint32_t y = 0u; y < size_y; y++) {
        for (uint32_t x = 0u; x < size_x; x++) {
            if (map_backup[CALC_OFFSET(x, y, size_x)] == '^') {
                guard_x = x;
                guard_y = y;
                break;
            }
        }   
    }

    uint32_t num_loop_options = 0u;

    for (uint32_t y = 0u; y < size_y; y++) {
        for (uint32_t x = 0u; x < size_x; x++) {
            if ((map_backup[CALC_OFFSET(x, y, size_x)] == '#') || ((x == guard_x) && (y == guard_y))) {
                continue;
            }
            bool loop_found = false;
            uint32_t iter_ctr = 0u;
            uint32_t tmp_guard_x = guard_x;
            uint32_t tmp_guard_y = guard_y;

            memset(map_dir, 0, MAX_SIZE_X * MAX_SIZE_Y);
            map_dir[CALC_OFFSET(guard_x, guard_y, size_x)] = '^';
            memcpy(map, map_backup, size_x * size_y);
            map[CALC_OFFSET(x, y, size_x)] = '#';

            while (iter_ctr < MAX_STEPS)
            {
                if(!guard_step(map, map_dir, &tmp_guard_x, &tmp_guard_y, size_x, size_y, &loop_found))
                {
                    break;
                }
                if (loop_found) {
                    num_loop_options++;
                    break;
                }
                iter_ctr++;
            }
        }
    }
    
    printf("Number of option to create a loop = %u", num_loop_options);

    free(map);
    free(map_backup);
    free(map_dir);
    fclose(in);

    return 0;
}