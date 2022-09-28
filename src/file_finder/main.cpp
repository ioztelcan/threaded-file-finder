#include <iostream>
#include <vector>

constexpr int max_args_allowed = 100;

int main(int argc, char * argv[])
{
    if (max_args_allowed < argc - 2) {
        std::cerr << "Too many substrings to search, please try with " << max_args_allowed << " arguments or less.\n";
        return EXIT_FAILURE;
    }

    if (argc < 3) {
        std::cerr << "You need to provide a directory and at least one one substring to search. \n";
        return EXIT_FAILURE;
    }

    const std::vector<std::string_view> args(argv + 2, argv + argc);
    for (const auto &arg: args) {
        std::cout << arg << "\n";
    }
}