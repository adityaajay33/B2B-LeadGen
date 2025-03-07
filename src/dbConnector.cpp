#include "dbConnector.hpp"
#include <cstdlib>
#include <iostream>

pqxx::connection DBConnector::createConnection(){

    try{
        
        std::string connectionString = std::getenv("DB_CONNECTION_STRING");

        return pqxx::connection(connectionString); //connects to database using connection string from .env

    } catch (const std::exception &e) {

        std::cerr << "ERROR: Database connection was unsuccessful: " << e.what() << "\n.";
        exit(1);
    }
}