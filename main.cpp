#include <iostream>
#include "src/dbConnector.hpp"
#include "src/dbHandler.hpp"

#include "vector"
#include "unordered_map"

int main(){

    pqxx::connection conn = DBConnector::createConnection();

    std::unordered_map<std::string, Company> companies = DBHandler::getCompanies(conn);

    std::cout << "Retrieved " << companies.size() << " companies \n";
    for (const auto &pair : companies){
        const Company &company = pair.second;
        std::cout << "- " << "[" << company.id << "] " << company.name << " (" << company.location << ")\n";
    }

    std::vector<Embedding> embeddings = DBHandler::getEmbeddings(conn, "locations");
    std::cout << "Retrieved " << embeddings.size() << " embeddings \n";

    return 0;
}