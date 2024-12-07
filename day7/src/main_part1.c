#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH (256u + 1u)
#define MAX_NUMBERS     128u

typedef enum {
    OP_ADD         = 0,
    OP_MUL         = 1,
    NUM_BINARY_OPS
} binary_op_t;

static inline void bignum_increment(binary_op_t *num)
{
    uint32_t digit_index = 0u;
    num[digit_index]++;
    while ((num[digit_index] == NUM_BINARY_OPS) && (digit_index < MAX_NUMBERS - 1u)) {
        num[digit_index + 0u] = OP_ADD;
        num[digit_index + 1u]++;
        digit_index++;
    }
}

static inline uint64_t calculate(uint64_t *numbers, binary_op_t *ops, uint32_t num_numbers)
{
    uint64_t accumulator = numbers[0u];
    for (uint32_t i = 1u; i < num_numbers; i++) {
        switch (ops[i - 1u])
        {
        case OP_ADD:
            accumulator += numbers[i];
            break;
        case OP_MUL:
            accumulator *= numbers[i];
            break;
        default:
            break;
        }
    }
    return accumulator;
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    FILE *in = fopen(argv[1], "r");
    
    char line[MAX_LINE_LENGTH];

    uint64_t total_calibration_result = 0u;

    while (!feof(in)) {
        fgets(line, MAX_LINE_LENGTH, in);
        uint64_t numbers[MAX_NUMBERS];
        uint32_t num_numbers = 0u; 
        uint64_t target;
        char *str_ptr; int offset;
        
        sscanf(line, "%llu%n", &target, &offset);
        str_ptr = &line[offset + 2u];
        
        char *token = strtok(str_ptr, " ");
        while ((num_numbers < MAX_NUMBERS) && (token != NULL)) {
            numbers[num_numbers++] = strtoull(token, NULL, 10);
            token = strtok(NULL, " ");
        }
    	
        binary_op_t ops[MAX_NUMBERS];
        memset(&ops[0u], 0, MAX_NUMBERS * sizeof(binary_op_t)); /* start with all set to OP_ADD */
        
        /* Effectively pow(a,b) for integers */
        uint32_t num_combinations = NUM_BINARY_OPS;
        for (uint32_t i = 0; i < num_numbers - 1; i++) { num_combinations *= NUM_BINARY_OPS; }
        
        /* Bruteforce */
        bool solution_found = 0u;
        for (uint32_t i = 0; i < num_combinations; i++) { 
            if (calculate(numbers, ops, num_numbers) == target) {
                solution_found = true;
                break;
            }
            bignum_increment(ops);
        }

        if (solution_found) {
            total_calibration_result += target;
        }
    }
   
    printf("Total calibration result = %llu", total_calibration_result);

    fclose(in);

    return 0;
}