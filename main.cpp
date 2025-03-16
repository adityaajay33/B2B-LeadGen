#include <iostream>
#include "src/dbConnector.hpp"
#include "src/dbHandler.hpp"
#include "src/simCalculator.hpp"
#include "src/mathFunctions.hpp"

#include "vector"
#include "unordered_map"

int main(){

    pqxx::connection conn = DBConnector::createConnection();

    std::vector<Company> companies = DBHandler::getCompanies(conn);
    std::cout << "Retrieved " << companies.size() << " companies \n";

    std::vector<Embedding> embeddings = DBHandler::getEmbeddings(conn, "specific_industry_locations");
    std::cout << "Retrieved " << embeddings.size() << " embeddings \n";
    
    Embedding airbnb = embeddings[0];
    std::vector<std::pair<long, double>> simScores = SimCalculator::getSimilarCompanies(airbnb, embeddings);

    for (int i = 0; i < 5 && i < simScores.size(); ++i) {
        std::cout << simScores[i].first << " \n";
    }

    Company company = DBHandler::getCompanyById(conn, 1);
    std::cout << "- " << company.name << " (" << company.industry << ") (" << company.location << ")\n";

    return 0;
}