#ifndef DB_HANDLER_HPP
#define DB_HANDLER_HPP

#include <vector>
#include "company.hpp"
#include <pqxx/pqxx>

class DBHandler {

    public:

        static std::vector<Company> getCompanies(pqxx::connection &conn);
};

#endif  // DB_HANDLER_HPP