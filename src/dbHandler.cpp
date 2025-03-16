#include "dbHandler.hpp"
#include <iostream>

std::unordered_map<std::string, Company> DBHandler::getCompanies(pqxx::connection &conn){
    
    std::unordered_map<std::string, Company> companies;

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

            companies.emplace(company.name, company);
        }


        txn.commit();
    } catch (const std::exception &e) {

        std::cerr << "ERROR: Fetching company information from the database: " << e.what() << "\n";
    }

    return companies; //if successful: returns companies, if not: empty vector is returned
}


// Helper function 
std::vector<float> parseVector(const std::string& str) {
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


// Helper function: Checks to see if a column exists in the db
bool columnExists(pqxx::connection &conn, const std::string& column) {

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

std::vector<Embedding> DBHandler::getEmbeddings(pqxx::connection &conn, const std::string &index){

    std::vector<Embedding> embeddings;

    if (!columnExists(conn, index)){
        std::cerr << "ERROR: Column '" << index << "' does not exist. Query aborted.\n";
        return embeddings;
    }

    try{
        pqxx::work txn(conn);

        std::string query = "SELECT id, company_name, " + index + " FROM company_embeddings;";
        pqxx::result res = txn.exec(query);
        
        for (const auto& row : res) {
            Embedding embedding = {
                row["id"].is_null() ? 0 : row["id"].as<long>(),
                row["company_name"].is_null() ? "Unknown" : row["company_name"].as<std::string>(),
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