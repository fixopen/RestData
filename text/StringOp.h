#ifndef __UTIL_TEXT_STRINGOP_H__
#define __UTIL_TEXT_STRINGOP_H__

#include <string>
#include <vector>

namespace Util::Text::StringOp {
    //native charset interface
    int ToInt(std::string const &content, size_t mode = 10);

    double ToDouble(std::string const &content);

    //unicode charset interface
    std::wstring ToUpper(std::wstring const &v);

    std::wstring ToLower(std::wstring const &v);

    //unicode charset interface
    int ToInt(std::wstring const &content, size_t mode = 10);

    double ToDouble(std::wstring const &content);

    std::wstring FromInt(int value);

    std::wstring FromDouble(double dbl, size_t prec = 2);

    std::string FromIntU(int value);

    std::string FromDoubleU(double dbl, size_t prec = 2);

    //string split merge and trim
    std::vector<std::wstring> Split(std::wstring const &value, std::wstring const &delimeter);

    //std::vector<std::wstring> const Split(std::wstring const& value, std::vector<std::wstring> const& delimeters);
    std::wstring Merge(std::vector<std::wstring> const &value, std::wstring const &delimeter);

    std::vector<std::string> Split(std::string const &value, std::string const &delimeter);

    //std::vector<std::string> const Split(std::string const& value, std::vector<std::string> const& delimeters);
    std::string Merge(std::vector<std::string> const &value, std::string const &delimeter);

    std::wstring Trim(std::wstring const &value, std::wstring const &spaceChars = L" ");

    std::string Trim(std::string const &value, std::string const &spaceChars = " ");

    std::wstring Replace(std::wstring const &value, std::wstring const &src, std::wstring const &dst);

    std::string Replace(std::string const &value, std::string const &src, std::string const &dst);

    //native <=> unicode
    std::wstring ToUnicode(std::string const &content);

    std::string FromUnicode(std::wstring const &unicode);

    //unicode <=> utf8
    std::string ToUTF8(std::wstring const &unicode);

    std::wstring FromUTF8(std::string const &utf8);
}

#endif //__UTIL_TEXT_STRINGOP_H__
