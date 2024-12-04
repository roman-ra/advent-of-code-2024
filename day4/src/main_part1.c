#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH (256u + 1u)
#define MAX_COLUMNS     256u
#define MAX_ROWS        256u

#define TARGET_LEN 4u
const char target[TARGET_LEN + 1u] = "XMAS";

const int32_t dirs_x[8] = {  0,  1, 1, 1, 0, -1, -1, -1 };
const int32_t dirs_y[8] = { -1, -1, 0, 1, 1,  1,  0, -1 };

uint32_t check_at(const char *chars, uint32_t row, uint32_t column, uint32_t rows, uint32_t columns)
{
    uint32_t total_matches = 0u;
    for (uint32_t dir_id = 0u; dir_id < 8u; dir_id++) {
        int32_t dir_x = dirs_x[dir_id];
        int32_t dir_y = dirs_y[dir_id];

        uint32_t normal_matches = 0u;
        for (uint32_t i = 0u; i < TARGET_LEN; i++) {
            int32_t column_with_offset = column + i * dir_x;
            int32_t row_with_offset    = row    + i * dir_y;
            if (   (column_with_offset >= 0) && (column_with_offset < (int64_t)columns) 
                && (row_with_offset    >= 0) && (row_with_offset    < (int64_t)rows)) {
                uint32_t offset = (row_with_offset * columns) + column_with_offset;
                if (chars[offset] == target[i]) {
                    normal_matches++;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        if (normal_matches == TARGET_LEN) {
            total_matches++;
        }
    }
    
    return total_matches;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    FILE *in = fopen(argv[1], "r");
    
    char line[MAX_LINE_LENGTH];
    char *chars = malloc(MAX_COLUMNS * MAX_ROWS);
    uint32_t rows = 0u, columns = 0u;

    while (!feof(in)) {
        fgets(line, MAX_LINE_LENGTH, in);
        size_t line_len = strlen(line);
        if ((line_len > 0) && line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0';
            line_len--;
        }
        if (columns == 0u) {
            columns = line_len;
        }
        memcpy(&chars[rows * columns], line, columns);
        rows++;
    }
    
    uint32_t matches = 0u;

    for (uint32_t row = 0u; row < rows; row++) {
        for (uint32_t col = 0u; col < columns; col++) {
            matches += check_at(chars, row, col, rows, columns);
        }   
    }

    printf("Matches = %u", matches);

    free(chars);
    fclose(in);

    return 0;
}