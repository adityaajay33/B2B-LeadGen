#include "simCalculator.hpp"
#include "mathFunctions.hpp"
#include <iostream>

#include "simScore.hpp"

//Find Companies similar to the company whose embedding is provided
std::vector<SimScore> SimCalculator::getSimilarCompaniesToCompany(const Embedding &target_company, const std::vector<Embedding> &embeddings){
    
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

std::vector<SimScore> SimCalculator::getSimilarCompaniesToPrompt(const std::string& prompt, const std::vector<Embedding>& embeddings){

    std::string command = "python3 ../src/extractEmbeddings.py \"" + prompt + "\"";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to run Python script\n";
        return {};
    }

    char buffer[32768];
    std::string output;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);

    std::vector<float> embedding;
    std::stringstream ss(output);
    std::string token;
    while (std::getline(ss, token, ',')) {
        embedding.push_back(std::stof(token));
    }

    std::vector<SimScore> similarityScores;

    for (const auto& currEmbedding : embeddings){

        double similarity = MathFunctions::cosineSimilarity(embedding, currEmbedding.embeddings);
        similarityScores.push_back(SimScore{currEmbedding.id, currEmbedding.name, similarity});
    }

    std::sort(similarityScores.begin(), similarityScores.end(), [](const auto &a, const auto &b) {
        return a.score > b.score;
    });

    return similarityScores;
}