#include <iostream>
#include "src/dbConnector.hpp"
#include "src/dbHandler.hpp"

#include "vector"
#include "unordered_map"

int main(){

    pqxx::connection conn = DBConnector::createConnection();

    std::unordered_map<std::string, Company> companies = DBHandler::getCompanies(conn);
    std::cout << "Retrieved " << companies.size() << " companies \n";

    std::vector<Embedding> embeddings = DBHandler::getEmbeddings(conn, "locations");
    std::cout << "Retrieved " << embeddings.size() << " embeddings \n";

    

    return 0;
}