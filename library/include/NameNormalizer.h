#ifndef NAME_NORMALIZER_H
#define NAME_NORMALIZER_H

#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
#include <sstream>

class InvalidNameException : public std::exception {};

class NormalizedName {
public:
    NormalizedName(const std::string& name);

    std::string AsString() const;

private:
    std::string _name;

    std::string removeSuffix(const std::string& name) const {
        auto commaIndex{name.find(',')};
        if (commaIndex == std::string::npos) return name;
        return name.substr(0, commaIndex);
    }

    std::string suffix(const std::string& name) const {
        auto commaIndex{name.find(',')};
        if (commaIndex == std::string::npos) return "";
        return name.substr(commaIndex);
        return m_name;
    }

    std::string initial(const std::string& name) const {
        if (1 == name.length()) return name;

        std::stringstream s;
        s << name[0] << ".";
        return s.str();
    }

    std::string middleInitials(std::vector<std::string>& parts) const {
        if (parts.size() <= 2) return "";
        std::stringstream s;
        for_each(parts.begin() + 1, parts.end() - 1,
                [&](const std::string& part) {
                    s << " " << initial(part);
                });
        return s.str();
    }

    std::string firstName(std::vector<std::string>& parts) const {
        return parts[0];
    }

    std::string lastName(std::vector<std::string>& parts) const {
        return parts.back();
    }

private:

std::string m_name;
};

#endif
