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
    
    std::regex mul_instruction_regex{"mul\\(([0-9]{1,3}),([0-9]{1,3})\\)"};
    
    std::string line, merged;
    while (std::getline(in, line)) {
        merged += (line + "#");
    }
    line += "do()"; // guaranteed do() at the end

    size_t dont_pos = merged.find("don't()");
    while (dont_pos != std::string::npos) {
        size_t do_pos = merged.find("do()", dont_pos);
        merged.erase(dont_pos, do_pos - dont_pos + 1);
        dont_pos = merged.find("don't()");
    }

    uint64_t sum{0u};
    auto instructions_begin = std::sregex_iterator(merged.begin(), merged.end(), mul_instruction_regex);
    auto instructions_end = std::sregex_iterator();
    for (std::sregex_iterator i = instructions_begin; i != instructions_end; ++i)
    {
        std::smatch match = *i;
        uint32_t arg1 = std::stoul(match[1].str()), arg2 = std::stoul(match[2].str());
        sum += (arg1 * arg2);
    }

    printf("Sum = %llu", sum);

    in.close();

    return 0;
}