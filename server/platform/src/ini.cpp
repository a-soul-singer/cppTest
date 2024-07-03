#include "ini.h"

#include <iostream>
#include <fstream>

using namespace std;

IniParse::IniParse(const string& path)
{
    ifstream ifs(path);
    if (!ifs.is_open()) {
        cout << "not find file" << endl;
        return;
    }
    string line;
    string section;
    while (!ifs.eof()) {
        getline(ifs, line);
        line = Strip(line);
        if (line.empty() || line.front() == '#') {
            continue;
        }
        int index = line.find('=');
        if (line.front() == '[' && line.back() == ']') { // [mysql]
            section = line.substr(1, line.size() - 2);
            data_[section] = {};
        } else if (index != -1) {
            string key = Strip(line.substr(0, index));
            string val = Strip(line.substr(index + 1));
            data_[section][key] = val;
        } else {
            cout << "格式有误" << endl;
            break;
        }
    }
    ifs.close();
}

INI_DATA IniParse::GetAllData()
{
    return data_;
}

SECTION_DATA IniParse::GetSectionData(const std::string& section)
{
    return data_[section];
}

std::string IniParse::GetVal(const std::string& section, const std::string& option)
{
    return data_[section][option];
}

std::string IniParse::Strip(const std::string& src)
{
    int start = src.find_first_not_of(' ');
    int end = src.find_last_not_of(' ');
    if (start == string::npos || end == string::npos) {
        return "";
    }
    return src.substr(start, end - start + 1);
}
