#pragma once
#include <crow.h>

struct CORS {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context&) {
        // Allow OPTIONS preflight requests through immediately
        if (req.method == crow::HTTPMethod::Options) {
            res.code = 200;
            res.end();
        }
    }

    void after_handle(crow::request& req, crow::response& res, context&) {
        res.add_header("Access-Control-Allow-Origin", "http://localhost:4000");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
    }
};
