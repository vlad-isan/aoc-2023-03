//
// Created by Vlad Isan on 05/12/2023.
//

#include "puzzle.h"

int puzzle_sample_1(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-sample-1.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_1(file);
}

int puzzle_sample_2(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-sample-2.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_2(file);
}

int puzzle_1(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-1.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_1(file);
}

int puzzle_2(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-2.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_2(file);
}

int do_puzzle_1(std::ifstream &file) {
    EngineSchematic schematic;
    std::string line;

    while (std::getline(file, line)) {
        parse_engine_schematic_row(line, schematic);
    }

    // TODO: Iterate over all the symbols and for every symbol,
    // create a range of rows to search for parts and a position span to search for in those rows
    // eg: for row i, pos x (only one position per symbol as we only have one-char symbols), we search for parts in rows <i-1, i+1>
    // and search for parts whose positions intersect <x-1, x+1>


    return 0;
}

int do_puzzle_2(std::ifstream &file) {
    std::string line;

    while (std::getline(file, line)) {
        fmt::println("{}", line);
    }

    return 0;
}

void parse_engine_schematic_row(const std::string &line, EngineSchematic &schematic) {
    std::regex r("([0-9]+|[^0-9.]+)"); // Matches either sequence of digits or sequences of non-digits excluding the dot

    auto begin = std::sregex_iterator(line.begin(), line.end(), r);
    auto end = std::sregex_iterator();

    EngineRow row;

    for (std::sregex_iterator i = begin; i != end; ++i) {
        const std::smatch &match = *i;
        const std::string value = match.str();
        const bool is_part = std::ranges::all_of(value, [](char c) {
            return std::isdigit(c);
        });
        int start_pos = match.position();
        int end_pos = start_pos + value.size();

        auto pos = std::make_pair(start_pos, end_pos);
        row.parts.emplace_back<EnginePart>({
            .value = value, .type = is_part ? PartType::DIGIT : PartType::SYMBOL, .pos = pos
        });
    }

    schematic.rows.emplace_back(row);
}
