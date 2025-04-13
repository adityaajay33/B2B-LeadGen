#include <iostream>
#include "src/dbConnector.hpp"
#include "src/dbHandler.hpp"
#include "src/simCalculator.hpp"
#include "src/mathFunctions.hpp"
#include "src/jsonConverter.hpp"

#include <vector>
#include "unordered_map"
#include <map>
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>

#include <pybind11/embed.h>

#include "src/PromptExtractor.h"

int main(){

    pqxx::connection conn = DBConnector::createConnection();

    std::vector<Embedding> embeddings = DBHandler::getEmbeddings(conn, "description");
    std::cout << "Retrieved " << embeddings.size() << " embeddings \n";

    std::string prompt = "Food delivery company in San Francisco";
    std::vector<SimScore> simScoresByPrompt = SimCalculator::getSimilarCompaniesToPrompt(prompt, embeddings);
    
    for (int i = 0; i < 10 && i < simScoresByPrompt.size(); ++i) {
        Company company = DBHandler::getCompanyById(conn, simScoresByPrompt[i].id);
        std::cout << "- " << company.name << " (" << company.specificIndustry << ") (" << company.location << ")\n";
    }
    return 0;
}