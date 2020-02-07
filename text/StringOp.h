#ifndef __UTIL_TEXT_STRINGOP_H__
#define __UTIL_TEXT_STRINGOP_H__

#include <string>
#include <vector>

namespace Util {
    namespace Text {
        //charset, encoding, format, layout

        //if unicode project
        //  std::wstring::c_str() ==> CString
        //  static_cast<LPCTSTR>(CString) ==> std::wstring
        //else
        //  std::string::c_str() ==> CString
        //  static_cast<LPCTSTR>(CString) ==> std::string

        namespace StringOp {
            //native charset interface
            int const ToInt(std::string const &content, size_t const mode = 10);

            double const ToDouble(std::string const &content);

            //unicode charset interface
            std::wstring const ToUpper(std::wstring const &v);

            std::wstring const ToLower(std::wstring const &v);

            //unicode charset interface
            int const ToInt(std::wstring const &content, size_t const mode = 10);

            double const ToDouble(std::wstring const &content);

            std::wstring const FromInt(int const value);

            std::wstring const FromDouble(double const dbl, size_t const prec = 2);

            std::string const FromIntU(int const value);

            std::string const FromDoubleU(double const dbl, size_t const prec = 2);

            //string split merge and trim
            std::vector<std::wstring> const Split(std::wstring const &value, std::wstring const &delimeter);

            //std::vector<std::wstring> const Split(std::wstring const& value, std::vector<std::wstring> const& delimeters);
            std::wstring const Merge(std::vector<std::wstring> const &value, std::wstring const &delimeter);

            std::vector<std::string> const Split(std::string const &value, std::string const &delimeter);

            //std::vector<std::string> const Split(std::string const& value, std::vector<std::string> const& delimeters);
            std::string const Merge(std::vector<std::string> const &value, std::string const &delimeter);

            std::wstring const Trim(std::wstring const &value, std::wstring const &spaceChars = L" ");

            std::string const Trim(std::string const &value, std::string const &spaceChars = " ");

            std::wstring const Replace(std::wstring const &value, std::wstring const &src, std::wstring const &dst);

            std::string const Replace(std::string const &value, std::string const &src, std::string const &dst);

            //native <=> unicode
            std::wstring const ToUnicode(std::string const &content);

            std::string const FromUnicode(std::wstring const &unicode);

            //unicode <=> utf8
            std::string const ToUTF8(std::wstring const &unicode);

            std::wstring const FromUTF8(std::string const &utf8);
        }
    }
}

#endif //__UTIL_TEXT_STRINGOP_H__
