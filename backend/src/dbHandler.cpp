#include "dbHandler.hpp"
#include <iostream>


/*
HELPER FUNCTIONS - START
*/

bool columnExists(pqxx::connection &conn, const std::string &column) {

    try {
        pqxx::work txn(conn);
        std::string query = "SELECT column_name FROM information_schema.columns WHERE table_name = 'company_embeddings' AND column_name = '" + column + "';";
        pqxx::result res = txn.exec(query);

        return !res.empty();
    } catch (const std::exception &e) {
        std::cerr << "ERROR: Checking column existence failed: " << e.what() << "\n";
        return false;
    }
}

bool checkIdExists(pqxx::connection &conn, const std::string &table, long id){

    try{

        pqxx::work txn(conn);

        std::string query = "SELECT 1 FROM " + table + " WHERE id = " + std::to_string(id) + " LIMIT 1;";

        pqxx::result res = txn.exec(query);

        return !res.empty();

    }catch (const std::exception& e){

        std::cerr << "ERROR: Checking ID process did not execute correctly: " << e.what() << "\n";
        return false;
    }
}

bool checkCompanyNameExists(pqxx::connection &conn, const std::string &table, const std::string &company_name){

    try{

        pqxx::work txn(conn);

        std::string query = "SELECT 1 FROM " + table + " WHERE name = $1 LIMIT 1;";

        pqxx::result res = txn.exec(query, pqxx::params{company_name});

        return !res.empty();

    }catch (const std::exception& e){

        std::cerr << "ERROR: Checking company name existence did not execute correctly: " << e.what() << "\n";
        return false;
    }
}

std::vector<float> parseVector(const std::string &str) {
    std::vector<float> vec;

    std::string cleaned_str = str;
    if (!cleaned_str.empty() && cleaned_str.front() == '[' && cleaned_str.back() == ']') {
        cleaned_str = cleaned_str.substr(1, cleaned_str.size() - 2);
    }

    std::stringstream ss(cleaned_str);
    std::string token;

    while (std::getline(ss, token, ',')) { 
        try {
            vec.push_back(std::stof(token)); 
        } catch (const std::exception &e) {
            std::cerr << "Warning: Failed to parse value: " << token << " (Error: " << e.what() << ")\n";
        }
    }

    return vec;
}

/*

HELPER FUNCTIONS END
*/

std::vector<Company> DBHandler::getCompanies(pqxx::connection &conn){
    
    std::vector<Company> companies;

    try{

        pqxx::work txn(conn);

        pqxx::result res = txn.exec("SELECT id, name, location, description, batch, industry, specific_industry FROM companies;");

        for (const auto& row : res) {
            Company company = {
                row["id"].is_null() ? 0 : row["id"].as<long>(),
                row["name"].is_null() ? "Unknown" : row["name"].as<std::string>(),
                row["location"].is_null() ? "Unknown" : row["location"].as<std::string>(),
                row["description"].is_null() ? "No description" : row["description"].as<std::string>(),
                row["batch"].is_null() ? "N/A" : row["batch"].as<std::string>(),
                row["industry"].is_null() ? "Unknown industry" : row["industry"].as<std::string>(),
                row["specific_industry"].is_null() ? "Unknown specialization" : row["specific_industry"].as<std::string>()
            };

            companies.push_back(company);
        }


        txn.commit();
    } catch (const std::exception &e) {

        std::cerr << "ERROR: Fetching companies information from the database: " << e.what() << "\n";
    }

    return companies; //if successful: returns companies, if not: empty vector is returned
}

Company DBHandler::getCompanyById(pqxx::connection &conn, long id){

    Company company;

    if (!checkIdExists(conn, "companies", id)){

        std::cerr << "ERROR: ID '" << id << "' does not exist. Query aborted.\n";
        return company;
    }

    try{

        pqxx::work txn(conn);

        std::string query = "SELECT * FROM companies WHERE id=" + std::to_string(id) + ";";

        pqxx::result res = txn.exec(query);

        if (!res.empty()){
            pqxx::row row = res[0];
            company = {
                    row["id"].is_null() ? -1 : row["id"].as<long>(),
                    row["name"].is_null() ? "Unknown" : row["name"].as<std::string>(),
                    row["location"].is_null() ? "Unknown" : row["location"].as<std::string>(),
                    row["description"].is_null() ? "No description" : row["description"].as<std::string>(),
                    row["batch"].is_null() ? "N/A" : row["batch"].as<std::string>(),
                    row["industry"].is_null() ? "Unknown industry" : row["industry"].as<std::string>(),
                    row["specific_industry"].is_null() ? "Unknown specialization" : row["specific_industry"].as<std::string>()
            };
        }
            
    } catch (const std::exception &e){

        std::cerr << "ERROR: Fetching company information from the database: " << e.what() << "\n";
    }

    return company;
}

long DBHandler::getIdByCompanyName(pqxx::connection &conn, const std::string &company_name){

    long id = -1; // -1 is a default value returned when it is not found

    if (!checkCompanyNameExists(conn, "companies", company_name)){

        std::cerr << "ERROR: Company '" << company_name << "' does not exist. Query aborted.\n";
        return id;
    }

    try{

        pqxx::work txn(conn);

        std::string query = "SELECT id FROM companies WHERE name = $1;";

        pqxx::result res = txn.exec(query, pqxx::params{company_name});

        if (!res.empty()) {
            pqxx::row row = res[0];
            id = row["id"].is_null() ? -1 : row["id"].as<long>(); // Handle nulls
        }
            
    } catch (const std::exception &e){

        std::cerr << "ERROR: Fetching company information from the database: " << e.what() << "\n";
    }

    return id;
}

std::vector<Embedding> DBHandler::getEmbeddings(pqxx::connection &conn, const std::string &index){

    std::vector<Embedding> embeddings;

    if (!columnExists(conn, index)){
        std::cerr << "ERROR: Column '" << index << "' does not exist. Query aborted.\n";
        return embeddings;
    }

    try{
        pqxx::work txn(conn);

        std::string query = "SELECT id, name, " + index + " FROM company_embeddings;";
        pqxx::result res = txn.exec(query);
        
        for (const auto& row : res) {
            Embedding embedding = {
                row["id"].is_null() ? 0 : row["id"].as<long>(),
                row["name"].is_null() ? "Unknown" : row["name"].as<std::string>(),
                row[index].is_null() ? std::vector<float>{0.0} : parseVector(row[index].as<std::string>())
            };

            embeddings.push_back(embedding);
        }

        txn.commit();

    } catch (const std::exception &e) {

        std::cerr << "ERROR: Fetching text embeddings from the database: " << e.what() << "\n";
    }

    return embeddings;
}
