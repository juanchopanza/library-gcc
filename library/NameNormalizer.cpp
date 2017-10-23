#include "include/NameNormalizer.h"
#include "StringUtil.h"

#include<tuple>

namespace
{
    const std::string& firstName(const std::vector<std::string>& parts)
    {
        return parts.front();
    }
    const std::string& lastName(const std::vector<std::string>& parts)
    {
        return parts.back();
    }

    struct Range {
        using const_iterator = std::vector<std::string>::const_iterator;

        const_iterator begin() const { return begin_; }
        const_iterator end() const { return end_; }
        const_iterator begin_;
        const_iterator end_;
    };

    Range middleNames(const std::vector<std::string>& parts)
    {
        Range names{parts.end(), parts.end()};

        if (parts.size() < 3) return names;

        names.begin_ = ++parts.begin();
        return names;
    }

}

NormalizedName::NormalizedName(const std::string& name)
{
    using stringutil::split;
    using stringutil::trim;
    //TODO: only split if string contains a space
    // 1. split
    // 2. last part is surname
    // 3. first part is first name
    // 4. anything in between is middle names
    // 5. If middle name initials, omit dot
    auto parts{split(trim(name),' ')};

    switch (parts.size()) {
        case 1:
            m_name = name;
            break;
        case 2:
            m_name = lastName(parts) + ", " + firstName(parts);
            break;
        default:
            m_name = lastName(parts) + ", " + firstName(parts) + ' ' + parts[1][0];
            if (parts[1].size() > 1) m_name += '.';
    }
}

std::string NormalizedName::AsString() const {
    auto trimmed{stringutil::trim(_name)};
    auto baseName{removeSuffix(trimmed)};
    auto parts{stringutil::split(baseName, ' ')};

    if (parts.size() < 2) return trimmed;

    return lastName(parts) + ", " + firstName(parts) 
        + middleInitials(parts) + suffix(trimmed);
}
