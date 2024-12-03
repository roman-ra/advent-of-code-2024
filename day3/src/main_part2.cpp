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
    
    std::regex instruction_regex{"mul\\(([0-9]{1,3}),([0-9]{1,3})\\)|do\\(\\)|don't\\(\\)"};
    
    std::string line, merged;
    while (std::getline(in, line)) {
        merged += (line + "#");
    }

    uint64_t sum{0u};
    auto instructions_begin = std::sregex_iterator(merged.begin(), merged.end(), instruction_regex);
    auto instructions_end = std::sregex_iterator();
    bool doing = true;
    for (std::sregex_iterator i = instructions_begin; i != instructions_end; ++i)
    {
        std::smatch match = *i;
        switch (match[0].str()[0])
        {
        case 'm':
            /* mul */
            if (doing) {
                uint32_t arg1 = std::stoul(match[1].str()), arg2 = std::stoul(match[2].str());
                sum += (arg1 * arg2);
            }
            break;
        case 'd':
            if (match[0].str()[2] == 'n') {
                /* don't */
                doing = false;
            } else {
                doing = true;
            }
            break;
        default:
            break;
        }
    }

    printf("Sum = %llu", sum);

    in.close();

    return 0;
}