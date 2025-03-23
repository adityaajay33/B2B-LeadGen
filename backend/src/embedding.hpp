#ifndef EMBEDDING_HPP
#define EMBEDDING_HPP

#include <string>
#include <vector>

struct Embedding {

    long id;
    std::string name;
    std::vector<float> embeddings;

    Embedding() = default;

    Embedding(const long id, const std::string& name, std::vector<float> embeddings)
    : id(id), name(name), embeddings(embeddings) {}

};

#endif //EMBEDDING_HPP