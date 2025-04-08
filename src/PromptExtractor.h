#pragma once
#include <string>
#include <unordered_map>

class PromptExtractor {
public:
    using CategoryMap = std::unordered_map<std::string, std::string>;
    CategoryMap extract(const std::string& prompt);
};
