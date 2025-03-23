#ifndef MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP

#include <vector>
#include "embedding.hpp"
#include <pqxx/pqxx>

class MathFunctions {

    public:

        static double dotProduct(const std::vector<float>& A, const std::vector<float>& B);

        static double vectorMagnitude(const std::vector<float>& vec);

        static double cosineSimilarity(const std::vector<float>& A, const std::vector<float>& B);
};

#endif  // MATH_FUNCTIONS_HPP