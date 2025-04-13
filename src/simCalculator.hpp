#ifndef SIM_CALCULATOR_HPP
#define SIM_CALCULATOR_HPP

#include <vector>
#include "embedding.hpp"
#include "simScore.hpp"
#include <pqxx/pqxx>

class SimCalculator {

    public:

        static std::vector<SimScore> getSimilarCompaniesToCompany(const Embedding &target_company, const std::vector<Embedding> &embeddings);

        static std::vector<SimScore> getSimilarCompaniesToPrompt(const std::string& prompt, const std::vector<Embedding>& embeddings);
};

#endif  // SIM_CALCULATOR_HPP