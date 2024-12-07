#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH (256u + 1u)
#define MAX_PAGE_NUM    255u
#define MAX_PRINT_PAGES 256u

typedef struct {
    uint32_t num_rules;
    uint32_t rules[MAX_PAGE_NUM]; /* page numbers before which the current page number must be printed */
} ruleset_t;

static ruleset_t *rulesets = NULL;

static inline bool ordering_exists(uint32_t lpage, uint32_t rpage) {
    for (uint32_t i = 0u; i < rulesets[lpage].num_rules; i++) {
        if (rulesets[lpage].rules[i] == rpage) {
            return true;
        }
    }
    return false;
}

static int comp(const void *a, const void *b) {
    uint32_t lpage = *(uint32_t*)a;
    uint32_t rpage = *(uint32_t*)b;

    if (ordering_exists(lpage, rpage)) {
        return -1;
    }
    return 1;
} 

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    FILE *in = fopen(argv[1], "r");
    
    char line[MAX_LINE_LENGTH];
    rulesets = malloc(sizeof(ruleset_t) * (MAX_PAGE_NUM + 1u));
    memset(&rulesets[0u], 0, sizeof(ruleset_t) * (MAX_PAGE_NUM + 1u));
    bool first_section = true;

    while (first_section) {
        fgets(line, MAX_LINE_LENGTH, in);
        if (strlen(line) < 2) {
            first_section = false;
        } else {
            uint32_t num1 = 0u, num2 = 0u;
            sscanf(line, "%u|%u", &num1, &num2);
            rulesets[num1].rules[rulesets[num1].num_rules++] = num2;
        }
    }

    uint64_t sum_corrected_updates = 0u;
    while (!feof(in)) {
        fgets(line, MAX_LINE_LENGTH, in);
        uint32_t printed_pages[MAX_PRINT_PAGES];
        uint32_t num_printed_pages = 0u; 
        
        bool update_is_correct = true;

        char *token = strtok(line, ",");
        while ((num_printed_pages < MAX_PRINT_PAGES) && (token != NULL)) {
            printed_pages[num_printed_pages++] = strtoul(token, NULL, 10);
            /* Check if the are no ordering violations after adding the latest page */
            for (uint32_t i = 0u; i < num_printed_pages - 1u; i++) {
                if (ordering_exists(printed_pages[num_printed_pages - 1u], printed_pages[i])) {
                    update_is_correct = false;
                    break;
                }
            }
            token = strtok(NULL, ",");
        }
        if (!update_is_correct) {
            qsort((void*)&printed_pages[0u], num_printed_pages, sizeof(printed_pages[0u]), comp);
            sum_corrected_updates += printed_pages[num_printed_pages / 2];
        }
    }
   
    printf("Sum of middle page number of corrected updates = %llu", sum_corrected_updates);

    free(rulesets);
    fclose(in);

    return 0;
}