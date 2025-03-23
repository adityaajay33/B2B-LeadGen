#include "jsonConverter.hpp"
#include <iostream>

#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include "simScore.hpp" 

rapidjson::Document JSONConverter::convertSimScoresToJSON(const std::vector<SimScore>& simScores){

    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value nodes(rapidjson::kArrayType);

    rapidjson::Value node(rapidjson::kObjectType);
    node.AddMember("id", static_cast<int64_t>(simScores[0].id), allocator);
    node.AddMember("name", rapidjson::Value(simScores[0].name.c_str(), allocator), allocator);
    nodes.PushBack(node, allocator);

    for (size_t i = 1; i < simScores.size(); ++i) {
        rapidjson::Value node(rapidjson::kObjectType);
        node.AddMember("id", static_cast<int64_t>(simScores[i].id), allocator);
        node.AddMember("name", rapidjson::Value(simScores[i].name.c_str(), allocator), allocator);
        nodes.PushBack(node, allocator);
    }

    rapidjson::Value links(rapidjson::kArrayType);
    for (size_t i = 1; i < simScores.size() && i <= 5; ++i) { 
        double similarity = simScores[i].score;  

        rapidjson::Value link(rapidjson::kObjectType);
        link.AddMember("source", static_cast<int64_t>(simScores[0].id), allocator); 
        link.AddMember("target", static_cast<int64_t>(simScores[i].id), allocator);
        link.AddMember("value", similarity, allocator); 

        links.PushBack(link, allocator);
    }


    doc.AddMember("nodes", nodes, allocator);
    doc.AddMember("links", links, allocator);

    return doc;
}