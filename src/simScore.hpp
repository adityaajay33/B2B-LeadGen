#ifndef SIM_SCORE_HPP
#define SIM_SCORE_HPP

#include <string>

struct SimScore {

    long id;
    std::string name;
    double score;

    SimScore() = default;

    SimScore(const long id, const std::string& name, const double score)
    : id(id), name(name), score(score) {}

};

#endif //SIM_SCORE_HPP