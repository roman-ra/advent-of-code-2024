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

static inline bool abs_level_diff_ok(level_diff_t abs_diff)
{
    return (abs_diff >= MIN_LEVEL_DIFF) && (abs_diff <= MAX_LEVEL_DIFF);
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

        level_t     prev_level, curr_level;
        direction_t direction = DIR_UNKNOWN;
        bool        safe      = true;
        char       *next_num  = &line[0];
        
        sscanf(next_num, "%u", &prev_level);
        next_num = strchr(next_num + 1, ' ');
        while (safe && next_num) {
            sscanf(next_num, "%u", &curr_level);
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
            next_num = strchr(next_num + 1, ' ');
        }
        if (safe) {
            num_safe_reports++;
        }
    }

    printf("Number of safe reports = %u", num_safe_reports);

    fclose(in);

    return 0;
}