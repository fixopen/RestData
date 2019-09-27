//
// Created by up duan on 23/7/15.
//

#include "CSV.h"

#if 0
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void csvline_populate(vector<string> &record, const string& line, char delimiter);

int main(int argc, char *argv[])
{
    vector<string> row;
    string line;
    ifstream in("input.csv");
    if (in.fail())  { cout << "File not found" <<endl; return 0; }

    while(getline(in, line)  && in.good() )
    {
        csvline_populate(row, line, ',');
        for(int i=0, leng=row.size(); i<leng; i++)
            cout << row[i] << "\t";
        cout << endl;
    }
    in.close();
    return 0;
}

void csvline_populate(vector<string> &record, const string& line, char delimiter)
{
    int linepos=0;
    int inquotes=false;
    char c;
    int i;
    int linemax=line.length();
    string curstring;
    record.clear();

    while(line[linepos]!=0 && linepos < linemax)
    {

        c = line[linepos];

        if (!inquotes && curstring.length()==0 && c=='"')
        {
//beginquotechar
            inquotes=true;
        }
        else if (inquotes && c=='"')
        {
//quotechar
            if ( (linepos+1 <linemax) && (line[linepos+1]=='"') )
            {
//encountered 2 double quotes in a row (resolves to 1 double quote)
                curstring.push_back(c);
                linepos++;
            }
            else
            {
//endquotechar
                inquotes=false;
            }
        }
        else if (!inquotes && c==delimiter)
        {
//end of field
            record.push_back( curstring );
            curstring="";
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            record.push_back( curstring );
            return;
        }
        else
        {
            curstring.push_back(c);
        }
        linepos++;
    }
    record.push_back( curstring );
    return;
}

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <istream>

using std::cout;
using std::endl;

std::vector<std::string> csv_read_row(std::istream &in, char delimiter);
std::vector<std::string> csv_read_row(std::string &in, char delimiter);

int main(int argc, char *argv[])
{
    std::ifstream in("input.csv");
    if (in.fail()) return (cout << "File not found" << endl) && 0;
    while(in.good())
    {
        std::vector<std::string> row = csv_read_row(in, ',');
        for(int i=0, leng=row.size(); i<leng; i++)
            cout << "[" << row[i] << "]" << "\t";
        cout << endl;
    }
    in.close();

    std::string line;
    in.open("input.csv");
    while(getline(in, line)  && in.good())
    {
        std::vector<std::string> row = csv_read_row(line, ',');
        for(int i=0, leng=row.size(); i<leng; i++)
            cout << "[" << row[i] << "]" << "\t";
        cout << endl;
    }
    in.close();

    return 0;
}

std::vector<std::string> csv_read_row(std::string &line, char delimiter)
{
    std::stringstream ss(line);
    return csv_read_row(ss, delimiter);
}

std::vector<std::string> csv_read_row(std::istream &in, char delimiter)
{
    std::stringstream ss;
    bool inquotes = false;
    std::vector<std::string> row;//relying on RVO
    while(in.good())
    {
        char c = in.get();
        if (!inquotes && c=='"') //beginquotechar
        {
            inquotes=true;
        }
        else if (inquotes && c=='"') //quotechar
        {
            if ( in.peek() == '"')//2 consecutive quotes resolve to 1
            {
                ss << (char)in.get();
            }
            else //endquotechar
            {
                inquotes=false;
            }
        }
        else if (!inquotes && c==delimiter) //end of field
        {
            row.push_back( ss.str() );
            ss.str("");
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            if(in.peek()=='\n') { in.get(); }
            row.push_back( ss.str() );
            return row;
        }
        else
        {
            ss << c;
        }
    }
}

std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str)
{
    std::vector<std::string>   result;
    std::string                line;
    std::getline(str,line);

    std::stringstream          lineStream(line);
    std::string                cell;

    while(std::getline(lineStream,cell,','))
    {
        result.push_back(cell);
    }
    return result;
}

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSVRow
{
public:
    std::string const& operator[](std::size_t index) const
    {
        return m_data[index];
    }
    std::size_t size() const
    {
        return m_data.size();
    }
    void readNextRow(std::istream& str)
    {
        std::string         line;
        std::getline(str,line);

        std::stringstream   lineStream(line);
        std::string         cell;

        m_data.clear();
        while(std::getline(lineStream,cell,','))
        {
            m_data.push_back(cell);
        }
    }
private:
    std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str,CSVRow& data)
{
    data.readNextRow(str);
    return str;
}
int main()
{
    std::ifstream       file("plop.csv");

    CSVRow              row;
    while(file >> row)
    {
        std::cout << "4th Element(" << row[3] << ")\n";
    }
}

class CSVIterator
{
public:
    typedef std::input_iterator_tag     iterator_category;
    typedef CSVRow                      value_type;
    typedef std::size_t                 difference_type;
    typedef CSVRow*                     pointer;
    typedef CSVRow&                     reference;

    CSVIterator(std::istream& str)  :m_str(str.good()?&str:NULL) { ++(*this); }
    CSVIterator()                   :m_str(NULL) {}

    // Pre Increment
    CSVIterator& operator++()               {if (m_str) { (*m_str) >> m_row;m_str = m_str->good()?m_str:NULL;}return *this;}
    // Post increment
    CSVIterator operator++(int)             {CSVIterator    tmp(*this);++(*this);return tmp;}
    CSVRow const& operator*()   const       {return m_row;}
    CSVRow const* operator->()  const       {return &m_row;}

    bool operator==(CSVIterator const& rhs) {return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));}
    bool operator!=(CSVIterator const& rhs) {return !((*this) == rhs);}
private:
    std::istream*       m_str;
    CSVRow              m_row;
};


int main()
{
    std::ifstream       file("plop.csv");

    for(CSVIterator loop(file);loop != CSVIterator();++loop)
    {
        std::cout << "4th Element(" << (*loop)[3] << ")\n";
    }
}

#include <boost/tokenizer.hpp>

std::vector<std::string> vec;
using namespace boost;
tokenizer<escaped_list_separator<char> > tk(
        line, escaped_list_separator<char>('\\', ',', '\"'));
for (tokenizer<escaped_list_separator<char> >::iterator i(tk.begin());
i!=tk.end();++i)
{
vec.push_back(*i);
}


//http://www.partow.net/programming/strtk/index.html
void foo()
{
    std::string data = "1,2,3,4,5\n"
            "0,2,4,6,8\n"
            "1,3,5,7,9\n";

    strtk::token_grid grid(data,data.size(),",");

    for(std::size_t i = 0; i < grid.row_count(); ++i)
    {
        strtk::token_grid::row_type r = grid.row(i);
        for(std::size_t j = 0; j < r.size(); ++j)
        {
            std::cout << r.get<int>(j) << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


bool r = phrase_parse(first, last,

        //  Begin grammar
                      (
                              double_ % ','
                      )
        ,
        //  End grammar

                      space, v);


#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <boost/tokenizer.hpp>

int main()
{
    using namespace std;
    using namespace boost;
    string data("data.csv");

    ifstream in(data.c_str());
    if (!in.is_open()) return 1;

    typedef tokenizer< escaped_list_separator<char> > Tokenizer;
    vector< string > vec;
    string line;

    while (getline(in,line))
    {
        Tokenizer tok(line);
        vec.assign(tok.begin(),tok.end());

        // vector now contains strings from one row, output to cout here
        copy(vec.begin(), vec.end(), ostream_iterator<string>(cout, "|"));

        cout << "\n----------------------" << endl;
    }
}

void ParseCSV(const string& csvSource, vector<vector<string> >& lines)
{
    bool inQuote(false);
    bool newLine(false);
    string field;
    lines.clear();
    vector<string> line;

    string::const_iterator aChar = csvSource.begin();
    while (aChar != csvSource.end())
    {
        switch (*aChar)
        {
            case '"':
                newLine = false;
                inQuote = !inQuote;
                break;

            case ',':
                newLine = false;
                if (inQuote == true)
                {
                    field += *aChar;
                }
                else
                {
                    line.push_back(field);
                    field.clear();
                }
                break;

            case '\n':
            case '\r':
                if (inQuote == true)
                {
                    field += *aChar;
                }
                else
                {
                    if (newLine == false)
                    {
                        line.push_back(field);
                        lines.push_back(line);
                        field.clear();
                        line.clear();
                        newLine = true;
                    }
                }
                break;

            default:
                newLine = false;
                field.push_back(*aChar);
                break;
        }

        aChar++;
    }

    if (field.size())
        line.push_back(field);

    if (line.size())
        lines.push_back(line);
}

/*=============================================================================
    Copyright (c) 2001-2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
// This sample shows the usage of the list_p utility parser
//  1. parsing a simple ',' delimited list w/o item formatting
//  2. parsing a CSV list (comma separated values - strings, integers or reals)
//  3. parsing a token list (token separated values - strings, integers or
//     reals)
// with an action parser directly attached to the item part of the list_p
// generated parser

#include <string>
#include <iostream>
#include <cassert>
#include <vector>

#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <boost/spirit/utility/escape_char.hpp>

///////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace boost::spirit;

///////////////////////////////////////////////////////////////////////////////
// actor, attached to the list_p parser
class list_actor
{
public:
    list_actor (std::vector<std::string> &vec_) : vec(vec_) {}

    // The following operator() is called by the action parser generated by
    // attaching this actor to a list_p generated list parser.

    template <typename ActionIterT>
    void operator() (ActionIterT const &first, ActionIterT const &last) const
    {
        vec.push_back(std::string(first, last-first));
    }

private:
    std::vector<std::string> &vec;
};

///////////////////////////////////////////////////////////////////////////////
// main entry point
int main ()
{
    // 1. parsing a simple ',' delimited list w/o item formatting
    char const*                 plist_wo_item = "element1,element2,element3";
    rule<>                      list_wo_item;
    std::vector<std::string>    vec_list;

    list_wo_item =
            list_p[push_back_a(vec_list)]
            ;

    parse_info<> result = parse (plist_wo_item, list_wo_item);

    cout << "-----------------------------------------------------------------"
    << endl;

    if (result.hit)
    {
        cout
        << "Parsing simple list" << endl
        << "\t" << plist_wo_item << endl
        << "Parsed successfully!" << endl << endl;

        cout
        << "Actor was called " << (int)vec_list.size()
        << " times: " << endl;

        cout
        << "Results got from the list parser:" << endl;
        for (std::vector<std::string>::iterator it = vec_list.begin();
             it != vec_list.end(); ++it)
        {
            cout << *it << endl;
        }
    }
    else
    {
        cout << "Failed to parse simple list!" << endl;
    }

    cout << endl;

    // 2. parsing a CSV list (comma separated values - strings, integers or
    // reals)
    char const *plist_csv = "\"string\",\"string with an embedded \\\"\","
            "12345,0.12345e4,,2";
    rule<> list_csv, list_csv_item;
    std::vector<std::string> vec_item;

    vec_list.clear();

    list_csv_item =
            !(
                    confix_p('\"', *c_escape_ch_p, '\"')
                    |   longest_d[real_p | int_p]
            );

    list_csv =
            list_p(
                    list_csv_item[push_back_a(vec_item)],
                    ','
            )[push_back_a(vec_list)]
            ;

    result = parse (plist_csv, list_csv);

    cout << "-----------------------------------------------------------------"
    << endl;
    if (result.hit)
    {
        cout
        << "Parsing CSV list (comma separated values) " << endl
        << "\t" << plist_csv << endl
        << "Parsed successfully!" << endl << endl;

        if (result.full)
        {
            cout << "Matched " << (int)vec_list.size() <<
            " list elements (full list): " << endl;
        }
        else
        {
            cout << "Matched " << (int)vec_list.size() <<
            " list elements: " << endl;
        }

        cout << "The list parser matched:" << endl;
        for (std::vector<std::string>::iterator itl = vec_list.begin();
             itl != vec_list.end(); ++itl)
        {
            cout << *itl << endl;
        }

        cout << endl << "Item(s) got directly from the item parser:" << endl;
        for (std::vector<std::string>::iterator it = vec_item.begin();
             it != vec_item.end(); ++it)
        {
            cout << *it << endl;
        }

    }
    else
    {
        cout << "Failed to parse CSV list!" << endl;
    }

    cout << endl;

    // 3. parsing a token list (token separated values - strings, integers or
    // reals) with an action parser directly attached to the item part of the
    // list_p generated parser
    char const *plist_csv_direct = "\"string\"<par>\"string with an embedded "
            "\\\"\"<par>12345<par>0.12345e4";
    rule<> list_csv_direct, list_csv_direct_item;

    vec_list.clear();
    vec_item.clear();

    // Note: the list parser is here generated through the list_p.direct()
    // generator function. This inhibits re-attachment of the item_actor_direct
    // during parser construction (see: comment in utility/lists.hpp)
    list_csv_direct_item =
            confix_p('\"', *c_escape_ch_p, '\"')
            |   longest_d[real_p | int_p]
            ;

    list_csv_direct =
            list_p.direct(
                    (*list_csv_direct_item)[list_actor(vec_item)],
                    "<par>"
            )[list_actor(vec_list)]
            ;

    result = parse (plist_csv_direct, list_csv_direct);

    cout << "-----------------------------------------------------------------"
    << endl;
    if (result.hit)
    {
        cout
        << "Parsing CSV list (comma separated values)" << endl
        << "The list parser was generated with 'list_p.direct()'" << endl
        << "\t" << plist_csv_direct << endl
        << "Parsed successfully!" << endl << endl;

        if (result.full)
        {
            cout << "Matched " << vec_list.size() <<
            " list elements (full list): " << endl;
        }
        else
        {
            cout << "Matched " << vec_list.size() <<
            " list elements: " << endl;
        }

        cout << "The list parser matched:" << endl;
        for (std::vector<std::string>::iterator itl = vec_list.begin();
             itl != vec_list.end(); ++itl)
        {
            cout << *itl << endl;
        }

        cout << endl << "Items got directly from the item parser:" << endl;
        for (std::vector<std::string>::iterator it = vec_item.begin();
             it != vec_item.end(); ++it)
        {
            cout << *it << endl;
        }

    }
    else
    {
        cout << "Failed to parse CSV list!" << endl;
    }

    cout << endl;

    return 0;
}


#include <stdbool.h>
#include <wchar.h>
#include <wctype.h>

extern const wchar_t *nextCsvField(const wchar_t *p, wchar_t sep, bool *newline);

// Returns a pointer to the start of the next field,
// or zero if this is the last field in the CSV
// p is the start position of the field
// sep is the separator used, i.e. comma or semicolon
// newline says whether the field ends with a newline or with a comma
const wchar_t *nextCsvField(const wchar_t *p, wchar_t sep, bool *newline)
{
    // Parse quoted sequences
    if ('"' == p[0]) {
        p++;
        while (1) {
            // Find next double-quote
            p = wcschr(p, L'"');
            // If we don't find it or it's the last symbol
            // then this is the last field
            if (!p || !p[1])
                return 0;
            // Check for "", it is an escaped double-quote
            if (p[1] != '"')
                break;
            // Skip the escaped double-quote
            p += 2;
        }
    }

    // Find next newline or comma.
    wchar_t newline_or_sep[4] = L"\n\r ";
    newline_or_sep[2] = sep;
    p = wcspbrk(p, newline_or_sep);

    // If no newline or separator, this is the last field.
    if (!p)
        return 0;

    // Check if we had newline.
    *newline = (p[0] == '\r' || p[0] == '\n');

    // Handle "\r\n", otherwise just increment
    if (p[0] == '\r' && p[1] == '\n')
        p += 2;
    else
        p++;

    return p;
}

static wchar_t *csvFieldData(const wchar_t *fld_s, const wchar_t *fld_e, wchar_t *buffer, size_t buflen)
{
    wchar_t *dst = buffer;
    wchar_t *end = buffer + buflen - 1;
    const wchar_t *src = fld_s;

    if (*src == L'"')
    {
        const wchar_t *p = src + 1;
        while (p < fld_e && dst < end)
        {
            if (p[0] == L'"' && p+1 < fld_s && p[1] == L'"')
            {
                *dst++ = p[0];
                p += 2;
            }
            else if (p[0] == L'"')
            {
                p++;
                break;
            }
            else
                *dst++ = *p++;
        }
        src = p;
    }
    while (src < fld_e && dst < end)
        *dst++ = *src++;
    if (dst >= end)
        return 0;
    *dst = L'\0';
    return(buffer);
}

static void dissect(const wchar_t *line)
{
    const wchar_t *start = line;
    const wchar_t *next;
    bool     eol;
    wprintf(L"Input %3zd: [%.*ls]\n", wcslen(line), wcslen(line)-1, line);
    while ((next = nextCsvField(start, L',', &eol)) != 0)
    {
        wchar_t buffer[1024];
        wprintf(L"Raw Field: [%.*ls] (eol = %d)\n", (next - start - eol), start, eol);
        if (csvFieldData(start, next-1, buffer, sizeof(buffer)/sizeof(buffer[0])) != 0)
            wprintf(L"Field %3zd: [%ls]\n", wcslen(buffer), buffer);
        start = next;
    }
}

static const wchar_t multiline[] =
        L"First field of first row,\"This field is multiline\n"
                "\n"
                "but that's OK because it's enclosed in double quotes, and this\n"
                "is an escaped \"\" double quote\" but this one \"\" is not\n"
                "   \"This is second field of second row, but it is not multiline\n"
                "   because it doesn't start \n"
                "   with an immediate double quote\"\n"
;

int main(void) {
    wchar_t line[1024];

    while (fgetws(line, sizeof(line) / sizeof(line[0]), stdin))
        dissect(line);
    dissect(multiline);

    return 0;
}

#include <iostream>
#include <sstream>

#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/sequence/io.hpp>

namespace fusion = boost::fusion;

struct CsvString
{
    std::string value;

    // Stop reading a string once a CSV delimeter is encountered.
    friend std::istream& operator>>(std::istream& s, CsvString& v) {
        v.value.clear();
        for(;;) {
            auto c = s.peek();
            if(std::istream::traits_type::eof() == c || ',' == c || '\n' == c)
                break;
            v.value.push_back(c);
            s.get();
        }
        return s;
    }

    friend std::ostream& operator<<(std::ostream& s, CsvString const& v) {
        return s << v.value;
    }
};

int main() {
    std::stringstream input("abc,123,true,3.14\n"
                                    "def,456,false,2.718\n");

    typedef boost::tuple <CsvString, int, bool, double> CsvRow;

    using fusion::operator<<;
    std::cout << std::boolalpha;

    using fusion::operator>>;
    input >> std::boolalpha;
    input >> fusion::tuple_open("") >> fusion::tuple_close("\n") >> fusion::tuple_delimiter(',');

    for (CsvRow row; input >> row;)
        std::cout << row << '\n';
}

#endif
