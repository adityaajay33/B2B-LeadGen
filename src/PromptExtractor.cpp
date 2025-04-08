#include "PromptExtractor.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>

#include <rapidjson/document.h>

PromptExtractor::CategoryMap PromptExtractor::extract(const std::string& prompt) {
    std::string command = "python3 ../src/extract.py \"" + prompt + "\" > temp_output.json";

    std::system(command.c_str());

    std::ifstream infile("temp_output.json"); 
    if (!infile.is_open()) {
        std::cerr << "Failed to open temp_output.json\n";
        return {};
    }

    std::stringstream buffer;
    buffer << infile.rdbuf();
    std::string jsonStr = buffer.str();

    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());

    CategoryMap result;
    for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it) {
        std::string key = it->name.GetString();
        if (it->value.HasMember("match")) {
            std::string match = it->value["match"].GetString();
            result[key] = match;
        }
    }

    return result;
}
