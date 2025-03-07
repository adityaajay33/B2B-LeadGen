#include "dbHandler.hpp"
#include <iostream>

std::vector<Company> DBHandler::getCompanies(pqxx::connection &conn){

    std::vector<Company> companies;

    try{

        pqxx::work txn(conn);

        pqxx::result res = txn.exec("SELECT id, name, location, description, batch, industry, specific_industry FROM companies;");

        for (const auto& row : res) {
            companies.emplace_back(
                row["id"].as<int>(), 
                row["name"].is_null() ? "Unknown" : row["name"].as<std::string>(), 
                row["location"].is_null() ? "Unknown" : row["location"].as<std::string>(), 
                row["description"].is_null() ? "No description" : row["description"].as<std::string>(), 
                row["batch"].is_null() ? "N/A" : row["batch"].as<std::string>(), 
                row["industry"].is_null() ? "Unknown industry" : row["industry"].as<std::string>(), 
                row["specific_industry"].is_null() ? "Unknown specialization" : row["specific_industry"].as<std::string>()
            );
        }


        txn.commit();
    } catch (const std::exception &e) {

        std::cerr << "ERROR: Fetching company information from the database: " << e.what() << "\n";
    }

    return companies; //if successful: returns companies, if not: empty vector is returned
}