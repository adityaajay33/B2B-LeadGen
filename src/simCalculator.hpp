#ifndef SIM_CALCULATOR_HPP
#define SIM_CALCULATOR_HPP

#include <vector>
#include "embedding.hpp"
#include <pqxx/pqxx>

class SimCalculator {

    public:

        static std::vector<std::string> getSimilarCompanies(pqxx::connection &conn, const std::vector<Embedding> embeddings);
};

#endif  // SIM_CALCULATOR_HPP