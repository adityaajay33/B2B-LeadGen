#include <iostream>
#include "src/dbConnector.hpp"
#include "src/dbHandler.hpp"

int main(){

    pqxx::connection conn = DBConnector::createConnection();

    std::vector<Company> companies = DBHandler::getCompanies(conn);

    std::cout << "Retrieved " << companies.size() << " companies \n";
    for (const auto &company : companies){
        std::cout << "- " << company.name << " (" << company.location << ")\n";
    }

    return 0;
}