#ifndef DB_HANDLER_HPP
#define DB_HANDLER_HPP

#include <vector>
#include "company.hpp"
#include "embedding.hpp"
#include <pqxx/pqxx>

class DBHandler {

    public:

        static std::unordered_map<std::string, Company> getCompanies(pqxx::connection &conn);

        static std::vector<Embedding> getEmbeddings(pqxx::connection &conn, const std::string &index);
};

#endif  // DB_HANDLER_HPP