#include <fstream>
#include <iostream>
#include <cstdint>
#include <regex>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    std::ifstream in{argv[1]};
    
    uint64_t sum{0u};
    std::regex mul_instruction_regex{"mul\\(([0-9]{1,3}),([0-9]{1,3})\\)"};
    
    std::string line;
    while (std::getline(in, line)) {
        auto instructions_begin = std::sregex_iterator(line.begin(), line.end(), mul_instruction_regex);
        auto instructions_end = std::sregex_iterator();
        for (std::sregex_iterator i = instructions_begin; i != instructions_end; ++i)
        {
            std::smatch match = *i;
            uint32_t arg1 = std::stoul(match[1].str()), arg2 = std::stoul(match[2].str());
            sum += (arg1 * arg2);
        }
    }

    printf("Sum = %llu", sum);

    in.close();

    return 0;
}