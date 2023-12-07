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

    return get_engine_parts_sum(schematic);
}

int do_puzzle_2(std::ifstream &file) {
    EngineSchematic schematic;
    std::string line;

    while (std::getline(file, line)) {
        parse_engine_schematic_row(line, schematic);
    }

    return get_engine_gear_ratios_sum(schematic);
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

        size_t start_pos = match.position() - (is_part ? 0 : 1);
        size_t end_pos = start_pos + value.size() + (is_part ? -1 : 1);

        auto pos = std::make_pair(start_pos, end_pos);

        row.parts.emplace_back<EnginePart>({
            .value = value, .type = is_part ? PartType::DIGIT : PartType::SYMBOL, .pos = pos
        });
    }

    schematic.rows.emplace_back(row);
}

uint32_t get_engine_parts_sum(const EngineSchematic &schematic) {
    uint32_t sum = 0;
    size_t row_index = 0;

    for(const auto &row: schematic.rows) {
        for(const auto &part: row.parts | std::views::filter([&](const auto &part_el) {
            return part_el.type == PartType::SYMBOL;
        })) {
            auto row_range = std::make_pair(row_index > 0 ? row_index - 1 : row_index, row_index + 1);
            sum += get_adjacent_parts_sum(schematic, row_range, part.pos);
        }

        row_index++;
    }

    return sum;
}

uint32_t get_adjacent_parts_sum(const EngineSchematic &schematic, const std::pair<size_t, size_t> &row_range, const std::pair<size_t, size_t> &pos_range) {
    uint32_t sum = 0;
    auto subrange = schematic.rows | std::views::drop(row_range.first) | std::views::take((row_range.second - row_range.first) + 1);

    for(const auto &row: subrange) {
        for(const auto &part: row.parts | std::views::filter([&](const auto &part_el) {
            return part_el.type == PartType::DIGIT && (part_el.pos.first <= pos_range.second && pos_range.first <= part_el.pos.second);
        })) {
            sum += std::stoi(part.value);
        }
    }

    return sum;
}

uint32_t get_engine_gear_ratios_sum(const EngineSchematic &schematic) {
    uint32_t sum = 0;
    size_t row_index = 0;

    for(const auto &row: schematic.rows) {
        for(const auto &part: row.parts | std::views::filter([&](const auto &part_el) {
            return part_el.type == PartType::SYMBOL && part_el.value == GEAR_SYMBOL;
        })) {
            auto row_range = std::make_pair(row_index > 0 ? row_index - 1 : row_index, row_index + 1);
            sum += get_gear_ratio(schematic, row_range, part.pos);
        }

        row_index++;
    }

    return sum;
}

uint32_t get_gear_ratio(const EngineSchematic &schematic, const std::pair<size_t, size_t> &row_range, const std::pair<size_t, size_t> &pos_range) {
    uint32_t found = 0;
    uint32_t ratio = 1;

    auto subrange = schematic.rows | std::views::drop(row_range.first) | std::views::take((row_range.second - row_range.first) + 1);

    for(const auto &row: subrange) {
        for(const auto &part: row.parts | std::views::filter([&](const auto &part_el) {
            return part_el.type == PartType::DIGIT && (part_el.pos.first <= pos_range.second && pos_range.first <= part_el.pos.second);
        })) {
            found++;
            ratio *= std::stoi(part.value);
        }
    }

    return found == 2 ? ratio : 0;
}
