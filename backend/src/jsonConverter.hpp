#ifndef JSON_CONVERTER_HPP
#define JSON_CONVERTER_HPP

#include <vector>
#include "company.hpp"
#include "embedding.hpp"
#include <pqxx/pqxx>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include "simScore.hpp" 

class JSONConverter {

    public:
        static rapidjson::Document convertSimScoresToJSON(const std::vector<SimScore>& simScores);  
};

#endif  // JSON_CONVERTER_HPP