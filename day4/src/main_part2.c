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

uint32_t check_at(const char *chars, uint32_t row, uint32_t column, uint32_t columns)
{
    uint32_t offset_neg_x_neg_y = ((row - 1) * columns) + (column - 1);
    uint32_t offset_pos_x_pos_y = ((row + 1) * columns) + (column + 1);
    uint32_t offset_neg_x_pos_y = ((row + 1) * columns) + (column - 1);
    uint32_t offset_pos_x_neg_y = ((row - 1) * columns) + (column + 1);
    if (     ((chars[offset_neg_x_neg_y] == 'M' && chars[offset_pos_x_pos_y] == 'S')
           || (chars[offset_neg_x_neg_y] == 'S' && chars[offset_pos_x_pos_y] == 'M'))
        &&   ((chars[offset_neg_x_pos_y] == 'M' && chars[offset_pos_x_neg_y] == 'S')
           || (chars[offset_neg_x_pos_y] == 'S' && chars[offset_pos_x_neg_y] == 'M'))) {
        return 1u;
    }
    return 0u;
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

    for (uint32_t row = 1u; row < rows - 1u; row++) {
        for (uint32_t col = 1u; col < columns - 1u; col++) {
            uint32_t offset = (row * columns) + col;
            if (chars[offset] == 'A') {
                matches += check_at(chars, row, col, columns);
            }
        }   
    }

    printf("Matches = %u", matches);

    free(chars);
    fclose(in);

    return 0;
}