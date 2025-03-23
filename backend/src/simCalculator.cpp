#include "simCalculator.hpp"
#include "mathFunctions.hpp"
#include <iostream>

#include "simScore.hpp"

//Find Companies similar to the company whose embedding is provided
std::vector<SimScore> SimCalculator::getSimilarCompanies(const Embedding &target_company, const std::vector<Embedding> &embeddings){
    
    std::vector<SimScore> similarityScores;

    for (const auto& embedding : embeddings){

        double similarity = MathFunctions::cosineSimilarity(target_company.embeddings, embedding.embeddings);
        similarityScores.push_back(SimScore{embedding.id, embedding.name, similarity});
    }

    std::sort(similarityScores.begin(), similarityScores.end(), [](const auto &a, const auto &b) {
        return a.score > b.score;
    });

    return similarityScores;
}