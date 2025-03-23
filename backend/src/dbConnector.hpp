#ifndef DB_CONNECTOR_HPP
#define DB_CONNECTOR_HPP

#include <pqxx/pqxx>
#include <string>

class DBConnector {

    public:

        static pqxx::connection createConnection();
};

#endif