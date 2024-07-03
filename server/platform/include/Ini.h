#pragma once
#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, std::string> SECTION_DATA;
typedef std::unordered_map<std::string, SECTION_DATA> INI_DATA;

class IniParse {
public:
    IniParse(const std::string& path);

    INI_DATA GetAllData();

    SECTION_DATA GetSectionData(const std::string& section);

    std::string GetVal(const std::string& section, const std::string& option);
private:
    std::string Strip(const std::string& src);
private:
    INI_DATA data_;
};
