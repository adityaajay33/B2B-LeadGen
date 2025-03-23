#include <iostream>
#include "src/dbConnector.hpp"
#include "src/dbHandler.hpp"
#include "src/simCalculator.hpp"
#include "src/mathFunctions.hpp"
#include "src/jsonConverter.hpp"

#include <vector>
#include "unordered_map"
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>

int main(){

    pqxx::connection conn = DBConnector::createConnection();

    std::vector<Company> companies = DBHandler::getCompanies(conn);
    std::cout << "Retrieved " << companies.size() << " companies \n";

    std::vector<Embedding> embeddings = DBHandler::getEmbeddings(conn, "specific_industry_locations");
    std::cout << "Retrieved " << embeddings.size() << " embeddings \n";

    long id = DBHandler::getIdByCompanyName(conn, "The Ticket Fairy");
    std::cout << "Retrieved id = " << id << ".\n";
    
    Embedding airbnb = embeddings[0];
    std::vector<SimScore> simScores = SimCalculator::getSimilarCompanies(airbnb, embeddings);

    for (int i = 0; i < 10 && i < simScores.size(); ++i) {
        Company company = DBHandler::getCompanyById(conn, simScores[i].id);
        std::cout << "- " << company.name << " (" << company.specificIndustry << ") (" << company.location << ")\n";
    }
    
    std::vector<SimScore> topkSimScores(simScores.begin(), simScores.begin() + std::min(5, static_cast<int>(simScores.size())));

    rapidjson::Document jsonData = JSONConverter::convertSimScoresToJSON(topkSimScores);

    std::ofstream outFile("../graph/graph_data.json");

    if (outFile.is_open()) {
        rapidjson::OStreamWrapper osw(outFile);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
        jsonData.Accept(writer);
        outFile.close();
        std::cout << "JSON data written to graph/graph_data.json" << std::endl;
    } else {
        std::cerr << "Failed to open the file for writing." << std::endl;
    }

    return 0;
}