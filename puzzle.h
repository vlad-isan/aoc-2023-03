//
// Created by Vlad Isan on 05/12/2023.
//

#ifndef PUZZLE_H
#define PUZZLE_H

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include "fmt/core.h"

enum class PartType {
    DIGIT,
    SYMBOL
};

struct EnginePart {
    std::string value;
    PartType type;
    std::pair<int, int> pos;
};

struct EngineRow {
    std::vector<EnginePart> parts;
};

struct EngineSchematic {
    std::vector<EngineRow> rows;
};

int puzzle_sample_1(const std::string &base_file_path);

int puzzle_sample_2(const std::string &base_file_path);

int puzzle_1(const std::string &base_file_path);

int puzzle_2(const std::string &base_file_path);

int do_puzzle_1(std::ifstream &file);

int do_puzzle_2(std::ifstream &file);

void parse_engine_schematic_row(const std::string &line, EngineSchematic &schematic);

#endif //PUZZLE_H
