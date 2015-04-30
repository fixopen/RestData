#include <stdlib.h>
#include <stdio.h>
#include "StringOp.h"

namespace Util {
    namespace Text {
        namespace StringOp {
            int const ToInt(std::string const &content, size_t const mode) {
                char *endPos = 0;
                int result = ::strtol(content.c_str(), &endPos, mode); //atoi(content.c_str());
                return result;
            }

            double const ToDouble(std::string const &content) {
                char *endPos = 0;
                double result = ::strtod(content.c_str(), &endPos); //atod(content.c_str());
                return result;
            }

            std::wstring const ToUpper(std::wstring const &v) {
                std::wstring result;

                for (size_t i = 0, e = v.size(); i < e; ++i) {
                    result += static_cast<wchar_t>(toupper(v[i]));
                }

                return result;
            }

            std::wstring const ToLower(std::wstring const &v) {
                std::wstring result;

                for (size_t i = 0, e = v.size(); i < e; ++i) {
                    result += static_cast<wchar_t>(tolower(v[i]));
                }

                return result;
            }

            int const ToInt(std::wstring const &content, size_t const mode) {
                return ToInt(ToUTF8(content), mode);
            }

            double const ToDouble(std::wstring const &content) {
                return ToDouble(ToUTF8(content));
            }

            std::wstring const FromInt(int const value) {
                char stringValue[32] = {0};
                //sprintf_s(stringValue, 32, "%d", value);
                sprintf(stringValue, "%d", value);
                return FromUTF8(stringValue);
            }

            std::wstring const FromDouble(double const value, size_t const prec) {
                char format[16] = {0};
                //sprintf_s(format, 16, "%%.%df", prec);
                sprintf(format, "%%.%df", (int)prec);
                char stringValue[64] = {0};
                //sprintf_s(stringValue, 64, format, value);
                sprintf(stringValue, format, value);
                return FromUTF8(stringValue);
            }

            std::string const FromIntU(int const value) {
                char stringValue[32] = {0};
                //sprintf_s(stringValue, 32, "%d", value);
                sprintf(stringValue, "%d", value);
                return stringValue;
            }

            std::string const FromDoubleU(double const value, size_t const prec) {
                char format[16] = {0};
                //sprintf_s(format, 16, "%%.%df", prec);
                sprintf(format, "%%.%df", (int)prec);
                char stringValue[64] = {0};
                //sprintf_s(stringValue, 64, format, value);
                sprintf(stringValue, format, value);
                return stringValue;
            }

            std::vector<std::wstring> const Split(std::wstring const &value, std::wstring const &delimeter) {
                std::vector<std::wstring> result;
                /*std::*/
                size_t startPos = 0;
                /*std::*/
                size_t findPos = 0;

                do {
                    findPos = value.find(delimeter, startPos);

                    if (findPos == std::wstring::npos) {
                        result.push_back(value.substr(startPos, value.length()));
                    } else {
                        result.push_back(value.substr(startPos, findPos - startPos));
                    }

                    startPos = findPos + delimeter.length();
                } while (findPos != std::wstring::npos);

                return result;
            }

            std::wstring const Merge(std::vector<std::wstring> const &value, std::wstring const &delimeter) {
                std::wstring result;

                for (size_t i = 0; i < value.size(); ++i) {
                    result += value[i];

                    if (i != value.size() - 1) {
                        result += delimeter;
                    }
                }

                return result;
            }

            std::vector<std::string> const Split(std::string const &value, std::string const &delimeter) {
                std::vector<std::string> result;
                /*std::*/
                size_t startPos = 0;
                /*std::*/
                size_t findPos = 0;

                do {
                    findPos = value.find(delimeter, startPos);

                    if (findPos == std::string::npos) {
                        result.push_back(value.substr(startPos, value.length()));
                    } else {
                        result.push_back(value.substr(startPos, findPos - startPos));
                    }

                    startPos = findPos + delimeter.length();
                } while (findPos != std::string::npos);

                return result;
            }

            std::string const Merge(std::vector<std::string> const &value, std::string const &delimeter) {
                std::string result;

                for (size_t i = 0; i < value.size(); ++i) {
                    result += value[i];

                    if (i != value.size() - 1) {
                        result += delimeter;
                    }
                }

                return result;
            }

            namespace {
                inline bool const isSpace(std::wstring const &spaceChars, wchar_t const chr) {
                    bool result = false;

                    for (size_t i = 0; i < spaceChars.length(); ++i) {
                        if (spaceChars[i] == chr) {
                            result = true;
                            break;
                        }
                    }

                    return result;
                }

                inline bool const isSpace(std::string const &spaceChars, char const chr) {
                    bool result = false;

                    for (size_t i = 0; i < spaceChars.length(); ++i) {
                        if (spaceChars[i] == chr) {
                            result = true;
                            break;
                        }
                    }

                    return result;
                }
            }

            std::wstring const Trim(std::wstring const &value, std::wstring const &spaceChars) {
                //std::wstring result;
                size_t startPos = 0;

                for (size_t i = 0; i < value.length(); ++i) {
                    if (!isSpace(spaceChars, value[i])) {
                        startPos = i;
                        break;
                    }
                }

                size_t stopPos = value.length() - 1;

                for (size_t i = value.length() - 1; i >= startPos; --i) {
                    if (!isSpace(spaceChars, value[i])) {
                        stopPos = i;
                        break;
                    }
                }

                return value.substr(startPos, stopPos - startPos + 1);
                //return result;
            }

            std::string const Trim(std::string const &value, std::string const &spaceChars) {
                //std::wstring result;
                size_t startPos = 0;

                for (size_t i = 0; i < value.length(); ++i) {
                    if (!isSpace(spaceChars, value[i])) {
                        startPos = i;
                        break;
                    }
                }

                size_t stopPos = value.length() - 1;

                for (size_t i = value.length() - 1; i >= startPos; --i) {
                    if (!isSpace(spaceChars, value[i])) {
                        stopPos = i;
                        break;
                    }
                }

                return value.substr(startPos, stopPos - startPos + 1);
                //return result;
            }

            std::wstring const Replace(std::wstring const &value, std::wstring const &src, std::wstring const &dst) {
                std::wstring result = value;
                std::wstring::size_type pos = 0;
                auto srcLen = src.size();
                auto dstLen = dst.size();

                while ((pos = result.find(src, pos)) != std::wstring::npos) {
                    result.replace(pos, srcLen, dst);
                    pos += dstLen;
                }

                return result;
            }

            std::string const Replace(std::string const &value, std::string const &src, std::string const &dst) {
                std::string result = value;
                std::string::size_type pos = 0;
                auto srcLen = src.size();
                auto dstLen = dst.size();

                while ((pos = result.find(src, pos)) != std::string::npos) {
                    result.replace(pos, srcLen, dst);
                    pos += dstLen;
                }

                return result;
            }

            std::wstring const ToUnicode(std::string const &content) {
//                //step1 : get converted length
//                size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), -1, 0, 0);
//                //size_t wideContentLength = mbstowcs(0, content.c_str(), content.length());
//                //step2 : alloc converted space
//                wchar_t *wideContent = new wchar_t[wideContentLength + 2];
//                memset(wideContent, 0, sizeof(wchar_t) * (wideContentLength + 2));
//                //step3 : convert
//                size_t convLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), wideContent,
//                                                        wideContentLength);
//                //size_t convLength = mbstowcs(wideContent, nullTermContent, wideContentLength + 1);
//                //assert(convLength == wideContentLength);
//                //step4 : free converted space and return
//                std::wstring result(wideContent);
//                delete[] wideContent;
//                return result;
                return L"";
            }

            std::string const FromUnicode(std::wstring const &value) {
//                //step1 : get converted length
//                BOOL usedDefaultChar = false;
//                int length = WideCharToMultiByte(CP_ACP, 0, value.c_str(), -1, 0, 0, 0, &usedDefaultChar);
//                //int length = wcstombs(0, (LPCTSTR)value, value.GetLength() * 2);
//                //step2 : alloc converted space
//                char *content = (char *) malloc(length + 1);
//                memset(content, 0, length + 1);
//                //step3 : convert
//                int convLength = WideCharToMultiByte(CP_ACP, 0, value.c_str(), -1, content, length + 1, 0,
//                                                     &usedDefaultChar);
//                //int convLength = wcstombs(content, (LPCTSTR)value, value.GetLength() * 2);
//                //step4 : free converted space and return
//                std::string result(content);
//                free(content);
//                return result;
                return "";
            }

            std::string const ToUTF8(std::wstring const &unicode) {
#if 0
					  | Unicode���ŷ�Χ       | UTF - 8���뷽ʽ
					n | (ʮ������)            | (������)
					--+-----------------------+------------------------------------------------------
					1 | 0000 0000 - 0000 007F |                                              0xxxxxxx
					2 | 0000 0080 - 0000 07FF |                                     110xxxxx 10xxxxxx
					3 | 0000 0800 - 0000 FFFF |                            1110xxxx 10xxxxxx 10xxxxxx
					4 | 0001 0000 - 0010 FFFF |                   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
					5 | 0020 0000 - 03FF FFFF |          111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
					6 | 0400 0000 - 7FFF FFFF | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

					�� 1. UTF - 8�ı������
#endif
                std::string result;
                enum Hex {
                    Hex80 = 0x80,
                    Hex800 = 0x800,
                    Hex10000 = 0x10000,
                    Hex200000 = 0x200000,
                    Hex4000000 = 0x4000000,
                    Hex80000000 = 0x80000000
                };
                /*std::*/
                size_t length = unicode.length();
                char chr = 0;

                for (/*std::*/size_t i = 0; i < length; ++i) {
                    if (unicode[i] < Hex80) { //1byte
                        chr = (char) (((unicode[i] >> 0) & 0x7F) | 0x00);
                        result += chr;
                    } else if (unicode[i] < Hex800) { //2byte
                        chr = (char) (((unicode[i] >> 6) & 0x1F) | 0xC0);
                        result += chr;
                        chr = (char) (((unicode[i] >> 0) & 0x3F) | 0x80);
                        result += chr;
                    } else if (unicode[i] < Hex10000) { //3byte
                        chr = (char) (((unicode[i] >> 12) & 0x0F) | 0xE0);
                        result += chr;
                        chr = (char) (((unicode[i] >> 6) & 0x3F) | 0x80);
                        result += chr;
                        chr = (char) (((unicode[i] >> 0) & 0x3F) | 0x80);
                        result += chr;
                    } else if (unicode[i] < Hex200000) { //4byte
                        chr = (char) (((unicode[i] >> 18) & 0x07) | 0xF0);
                        result += chr;
                        chr = (char) (((unicode[i] >> 12) & 0x3F) | 0x80);
                        result += chr;
                        chr = (char) (((unicode[i] >> 6) & 0x3F) | 0x80);
                        result += chr;
                        chr = (char) (((unicode[i] >> 0) & 0x3F) | 0x80);
                        result += chr;
                    } else if (unicode[i] < Hex4000000) { //5byte
                        chr = (char) (((unicode[i] >> 24) & 0x03) | 0xF8);
                        result += chr;
                        chr = (char) (((unicode[i] >> 18) & 0x3F) | 0x80);
                        result += chr;
                        chr = (char) (((unicode[i] >> 12) & 0x3F) | 0x80);
                        result += chr;
                        chr = (char) (((unicode[i] >> 6) & 0x3F) | 0x80);
                        result += chr;
                        chr = (char) (((unicode[i] >> 0) & 0x3F) | 0x80);
                        result += chr;
                    } else if (unicode[i] < Hex80000000) { //6byte
                        chr = (char) (((unicode[i] >> 30) & 0x01) | 0xFC);
                        result += chr;
                        chr = (char) (((unicode[i] >> 24) & 0x3F) | 0x80);
                        result += chr;
                        chr = (char) (((unicode[i] >> 18) & 0x3F) | 0x80);
                        result += chr;
                        chr = (char) (((unicode[i] >> 12) & 0x3F) | 0x80);
                        result += chr;
                        chr = (char) (((unicode[i] >> 6) & 0x3F) | 0x80);
                        result += chr;
                        chr = (char) (((unicode[i] >> 0) & 0x3F) | 0x80);
                        result += chr;
                    }
                }

                return result;
            }

            std::wstring const FromUTF8(std::string const &value) {
                std::wstring result;
                wchar_t chr;

                for (size_t i = 0; i < value.length(); ++i) {
                    if ((value[i] & 0x80) == 0x0) { //1byte
                        chr = (value[i + 0] & 0x7F) << 0;
                    } else if ((value[i] & 0xE0) == 0xC0) { //2byte
                        chr = ((value[i + 0] & 0x1F) << 6)
                              | ((value[i + 1] & 0x3F) << 0);
                        ++i;
                    } else if ((value[i] & 0xF0) == 0xE0) { //3byte
                        chr = ((value[i + 0] & 0x0F) << 12)
                              | ((value[i + 1] & 0x3F) << 6)
                              | ((value[i + 2] & 0x3F) << 0);
                        i += 2;
                    } else if ((value[i] & 0xF8) == 0xF0) { //4byte
                        chr = ((value[i + 0] & 0x07) << 18)
                              | ((value[i + 1] & 0x3F) << 12)
                              | ((value[i + 2] & 0x3F) << 6)
                              | ((value[i + 3] & 0x3F) << 0);
                        i += 3;
                    } else if ((value[i] & 0xFC) == 0xF8) { //5byte
                        chr = ((value[i + 0] & 0x03) << 24)
                              | ((value[i + 1] & 0x3F) << 18)
                              | ((value[i + 2] & 0x3F) << 12)
                              | ((value[i + 3] & 0x3F) << 6)
                              | ((value[i + 4] & 0x3F) << 0);
                        i += 4;
                    } else if ((value[i] & 0xFE) == 0xFC) { //6byte
                        chr = ((value[i + 0] & 0x01) << 30)
                              | ((value[i + 1] & 0x3F) << 24)
                              | ((value[i + 2] & 0x3F) << 18)
                              | ((value[i + 3] & 0x3F) << 12)
                              | ((value[i + 4] & 0x3F) << 6)
                              | ((value[i + 5] & 0x3F) << 0);
                        i += 5;
                    }

                    result += chr;
                }

                return result;
            }
        }
    }
}
