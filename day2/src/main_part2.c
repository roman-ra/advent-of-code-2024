#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef uint32_t level_t;
typedef int64_t  level_diff_t;

typedef enum {
    DIR_INCREASING,
    DIR_DECREASING,
    DIR_UNKNOWN
} direction_t;

#define MAX_LINE_LENGTH (1024u + 1u)
#define MIN_LEVEL_DIFF  1u
#define MAX_LEVEL_DIFF  3u
#define MAX_NUM_LEVELS  512u

static inline bool abs_level_diff_ok(level_diff_t abs_diff)
{
    return (abs_diff >= MIN_LEVEL_DIFF) && (abs_diff <= MAX_LEVEL_DIFF);
}

static bool is_report_safe(level_t *levels, uint32_t num_levels, uint32_t skip_index)
{
    level_t     prev_level, curr_level;
    direction_t direction = DIR_UNKNOWN;
    bool        safe      = true;
    uint32_t    start_i;
    if (skip_index == 0u) {
        prev_level = levels[1u];
        start_i = 2u;
    } else if (skip_index == 1u) {
        prev_level = levels[0u];
        start_i = 2u;
    } else {
        prev_level = levels[0u];
        start_i = 1u;
    }
    for (uint32_t i = start_i; i < num_levels; i++) {
        if (i == skip_index) continue;
        curr_level = levels[i];
        level_diff_t diff     = (int64_t)curr_level - (int64_t)prev_level;
        level_diff_t abs_diff = llabs(diff);
        switch (direction) {
            case DIR_UNKNOWN:
                if (abs_level_diff_ok(abs_diff)) {
                    if (diff > 0) {
                        direction = DIR_INCREASING;
                    } else {
                        direction = DIR_DECREASING;
                    }
                } else {
                    safe = false;
                }
                break;
            case DIR_INCREASING:
                if (abs_level_diff_ok(abs_diff) && (diff > 0)) {
                    /* OK */
                } else {
                    safe = false;
                }
                break;
            case DIR_DECREASING:
                if (abs_level_diff_ok(abs_diff) && (diff < 0)) {
                    /* OK */
                } else {
                    safe = false;
                }
                break;
        }
        prev_level = curr_level;
    }
    return safe;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    FILE *in = fopen(argv[1], "r");
    
    char line[MAX_LINE_LENGTH];
    uint32_t num_safe_reports = 0u;

    while (!feof(in)) {
        fgets(line, MAX_LINE_LENGTH, in);

        char       *next_num  = &line[0];
        level_t     levels[MAX_NUM_LEVELS];
        uint32_t    num_levels = 0u;
        
        while (next_num && (num_levels < MAX_NUM_LEVELS)) 
        {
            sscanf(next_num, "%u", &levels[num_levels++]);
            next_num = strchr(next_num + 1, ' ');
        }

        bool safe = false;
        for (uint32_t i = 0; i <= num_levels; i++) {
            if (is_report_safe(&levels[0u], num_levels, i)) {
                safe = true;
                break;
            }
        }
        if (safe) {
            num_safe_reports++;
        }
    }

    printf("Number of safe reports = %u", num_safe_reports);

    fclose(in);

    return 0;
}