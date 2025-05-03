#define CROW_MAIN
#include <crow.h>
#include "cors_middleware.hpp"
#include "dbConnector.hpp"
#include "dbHandler.hpp"
#include "simCalculator.hpp"
#include "company.hpp"
#include "embedding.hpp"
#include "simScore.hpp"
#include "jsonConverter.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>

int main() {
    // ✅ Use CORS middleware
    crow::App<CORS> app;

    // /search POST
    CROW_ROUTE(app, "/search").methods("POST"_method)
    ([](const crow::request& req) {
        crow::response res;

        auto bodyJson = crow::json::load(req.body);
        if (!bodyJson) {
            res.code = 400;
            res.write("Invalid JSON body");
            return res;
        }

        std::string query = bodyJson["query"].s();
        if (query.empty()) {
            res.code = 400;
            res.write("Query cannot be empty");
            return res;
        }

        std::cout << "Received search query: " << query << std::endl;

        pqxx::connection conn = DBConnector::createConnection();
        std::vector<Embedding> embeddings = DBHandler::getEmbeddings(conn, "description");
        std::vector<SimScore> simScores = SimCalculator::getSimilarCompaniesToPrompt(query, embeddings);

        crow::json::wvalue::list results;
        for (int i = 0; i < 10 && i < simScores.size(); ++i) {
            Company company = DBHandler::getCompanyById(conn, simScores[i].id);
            crow::json::wvalue companyJson({
                {"id", static_cast<int>(simScores[i].id)},
                {"name", company.name},
                {"specificIndustry", company.specificIndustry},
                {"location", company.location},
                {"similarity", simScores[i].score}
            });
            results.push_back(std::move(companyJson));
        }

        res.code = 200;
        res.write(crow::json::wvalue(results).dump());
        return res;
    });

    // /graph POST
    CROW_ROUTE(app, "/graph").methods("POST"_method)
    ([](const crow::request& req) {
        crow::response res;

        auto bodyJson = crow::json::load(req.body);
        if (!bodyJson) {
            res.code = 400;
            res.write("Invalid JSON body");
            return res;
        }

        std::string companyId = bodyJson["id"].s();
        std::cout << "Received company id: " << companyId << std::endl;

        pqxx::connection conn = DBConnector::createConnection();
        std::vector<Embedding> embeddings = DBHandler::getEmbeddings(conn, "specific_industry_locations");

        int id = std::stoi(companyId);
        if (id < 0 || id >= embeddings.size()) {
            res.code = 400;
            res.write("Invalid company ID");
            return res;
        }

        Embedding companyEmbedding = embeddings[id];
        std::vector<SimScore> simScores = SimCalculator::getSimilarCompaniesToCompany(companyEmbedding, embeddings);
        std::vector<SimScore> topkSimScores(simScores.begin(), simScores.begin() + std::min(5, static_cast<int>(simScores.size())));

        rapidjson::Document jsonData = JSONConverter::convertSimScoresToJSON(topkSimScores);
        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        jsonData.Accept(writer);

        res.set_header("Content-Type", "application/json");
        res.write(buffer.GetString());
        res.code = 200;
        return res;
    });

    // Health check
    CROW_ROUTE(app, "/health").methods("GET"_method)
    ([]() {
        crow::response res(200);
        res.write("Server is running");
        return res;
    });

    std::cout << "Starting server on port 3001..." << std::endl;
    app.port(3001).multithreaded().run();

    return 0;
}
