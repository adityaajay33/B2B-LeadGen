#include "mathFunctions.hpp"
#include <iostream>

double MathFunctions::dotProduct(const std::vector<float>& A, const std::vector<float>& B) {
    double dot = 0.0;
    for (size_t i = 0; i < A.size(); i++) {
        dot += A[i] * B[i];
    }
    return dot;
}

double MathFunctions::vectorMagnitude(const std::vector<float>& vec) {
    double sum = 0.0;
    for (float val : vec) {
        sum += val * val;
    }
    return std::sqrt(sum);
}

double MathFunctions::cosineSimilarity(const std::vector<float>& A, const std::vector<float>& B) {
    double dot = dotProduct(A, B);
    double magnitudeA = vectorMagnitude(A);
    double magnitudeB = vectorMagnitude(B);
    
    if (magnitudeA == 0 || magnitudeB == 0) return 0.0; 
    return dot / (magnitudeA * magnitudeB);
}
