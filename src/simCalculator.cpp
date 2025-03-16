#include "simCalculator.hpp"
#include "mathFunctions.hpp"
#include <iostream>

//Find Companies similar to the company whose embedding is provided
std::vector<std::pair<std::string, double>> SimCalculator::getSimilarCompanies(const Embedding &target_company, const std::vector<Embedding> &embeddings){
    
    std::vector<std::pair<std::string, double>> similarityScores;

    for (const auto& embedding : embeddings){

        double similarity = MathFunctions::cosineSimilarity(target_company.embeddings, embedding.embeddings);
        similarityScores.push_back({embedding.name, similarity});
    }

    std::sort(similarityScores.begin(), similarityScores.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });

    return similarityScores;
}