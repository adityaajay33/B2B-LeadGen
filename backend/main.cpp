#include <iostream>
#include "src/dbConnector.hpp"
#include "src/dbHandler.hpp"
#include "src/simCalculator.hpp"
#include "src/mathFunctions.hpp"
#include "src/jsonConverter.hpp"

#include <vector>
#include "unordered_map"
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>

#include <crow_all.h>

int main(){

    crow::Simple app;

    app.route_dynamic("app/data")
    .methods("GET"_method)
    ([](const crow::request& req){
        crow::json::wvalue data;
        data["message"] = "Hello from C++ backend!";
        return data;
    });

    app.port(8080).multithreaded().run();
}