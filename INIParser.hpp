#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <stdexcept>

// Structure to represent INI sections and their key-value pairs
struct INISection {
    std::map<std::string, std::string> keyValues;
};

// INI file parser class
class INIParser {
public:
    // Load an INI file
    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        std::string line;
        INISection* currentSection = nullptr;

        try {
            while (std::getline(file, line)) {
                parseLine(line, currentSection);
            }
        } catch (const std::exception& e) {
            file.close();
            throw e;
        }

        file.close();
    }

    // Save an INI file
    void save(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to create file: " + filename);
        }

        try {
            for (const auto& section : sections) {
                file << "[" << section.first << "]\n";
                for (const auto& keyValue : section.second.keyValues) {
                    file << keyValue.first << "=" << keyValue.second << "\n";
                }
                file << "\n";
            }
        } catch (const std::exception& e) {
            file.close();
            throw e;
        }

        file.close();
    }

    // Get the value of a key within a section as a string
    std::string getValue(const std::string& section, const std::string& key) {
        if (sections.find(section) != sections.end()) {
            INISection& iniSection = sections[section];
            if (iniSection.keyValues.find(key) != iniSection.keyValues.end()) {
                return iniSection.keyValues[key];
            }
        }
        return "";
    }

    // Get the value of a key within a section as an integer
    int getValueAsInt(const std::string& section, const std::string& key, int defaultValue = 0) {
        std::string valueStr = getValue(section, key);
        if (!valueStr.empty()) {
            try {
                return std::stoi(valueStr);
            } catch (const std::exception& e) {
                std::cerr << "Failed to convert value to integer: " << e.what() << std::endl;
            }
        }
        return defaultValue;
    }

    // Set the value of a key within a section
    void setValue(const std::string& section, const std::string& key, const std::string& value) {
        INISection& iniSection = sections[section];
        iniSection.keyValues[key] = value;
    }

private:
    std::map<std::string, INISection> sections;

    // Regular expressions for parsing INI file lines
    const std::regex sectionRegex{"\\[([^\\]]+)\\]"};
    const std::regex keyValueRegex{"\\s*([^=]+)=(.*)"};

    // Parse a line from the INI file
    void parseLine(const std::string& line, INISection*& currentSection) {
        std::smatch match;
        if (std::regex_match(line, match, sectionRegex)) {
            std::string sectionName = match[1];
            currentSection = &sections[sectionName];
        }
        else if (currentSection != nullptr && std::regex_match(line, match, keyValueRegex)) {
            std::string key = match[1];
            std::string value = match[2];
            currentSection->keyValues[key] = value;
        }
    }
};
