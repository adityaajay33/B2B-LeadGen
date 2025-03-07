#ifndef COMPANY_HPP
#define COMPANY_HPP

#include <string>

struct Company {

    int id;
    std::string name;
    std::string location;
    std::string description;
    std::string batch;
    std::string industry;
    std::string specificIndustry;

    Company(int id, const std::string& name, const std::string& location,
        const std::string& description, const std::string& batch,
        const std::string& industry, const std::string& specificIndustry)
    : id(id), name(name), location(location), description(description),
      batch(batch), industry(industry), specificIndustry(specificIndustry) {}

};

#endif //COMPANY_HPP