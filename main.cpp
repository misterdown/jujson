#include "jujson.hpp"
#include <string>
#include <iostream>
// >:3 Custom trait
struct custom_trait {
    public:
    typedef char char_type;

    public:
    [[nodiscard]] constexpr static char_type get_string_litteral_begin() noexcept   {return '<';}
    [[nodiscard]] constexpr static char_type get_string_litteral_end() noexcept     {return '>';}
    [[nodiscard]] static bool is_string_litteral_begin(char_type c) noexcept        {return c == get_string_litteral_begin();}
    [[nodiscard]] static bool is_string_litteral_end(char_type c) noexcept          {return c == get_string_litteral_end();}
    [[nodiscard]] constexpr static char_type get_array_begin() noexcept             {return '[';}
    [[nodiscard]] constexpr static char_type get_array_end() noexcept               {return ']';}
    [[nodiscard]] static bool is_array_begin(char_type c) noexcept                  {return c == get_array_begin();}
    [[nodiscard]] static bool is_array_end(char_type c) noexcept                    {return c == get_array_end();}
    [[nodiscard]] constexpr static char_type get_object_begin() noexcept            {return '{';}
    [[nodiscard]] constexpr static char_type get_object_end() noexcept              {return '}';}
    [[nodiscard]] static bool is_object_begin(char_type c) noexcept                 {return c == get_object_begin();}
    [[nodiscard]] static bool is_object_end(char_type c) noexcept                   {return c == get_object_end();}
    [[nodiscard]] constexpr static char_type get_comma() noexcept                   {return ',';}
    [[nodiscard]] static bool is_comma(char_type c) noexcept                        {return c == get_comma();}
    [[nodiscard]] constexpr static char_type get_colon() noexcept                   {return ':';}
    [[nodiscard]] static bool is_colon(char_type c) noexcept                        {return c == get_colon();}
    [[nodiscard]] static bool is_new_line(char_type c) noexcept                     {return c == '\n';}
    [[nodiscard]] static bool is_escape_spec(char_type c) noexcept                  {return c == '\\';}
    [[nodiscard]] static bool is_minus(char_type c) noexcept                        {return c == '-';}
    [[nodiscard]] static bool is_exp_symbol(char_type c) noexcept                   {return (c == 'e') || (c == 'E');}
    [[nodiscard]] static bool is_dec_separator_symbol(char_type c) noexcept         {return (c == '.');}
    template<class StringT_>
    [[nodiscard]] static jujson::json_value_kind value_kind_from_string(const StringT_& str) {
        if (str == "null") {            return jujson::JUSJON_JSON_VALUE_NULL;
        } else if (str == "true") {     return jujson::JUSJON_JSON_VALUE_TRUE;
        } else if (str == "false") {    return jujson::JUSJON_JSON_VALUE_FALSE;
        } else  {                       return jujson::JUSJON_JSON_VALUE_INVALID;
        }
        return jujson::JUSJON_JSON_VALUE_INVALID;}
};

bool default_test() {
    std::cout << "basic test\n";
    std::string data = R"({"name":"JohnDoe","age":30,"isEmployed":true,"address":{"street":"123MainSt","city":"Anytown","state":"CA","postalCode":"12345"},"phoneNumbers":[{"type":"home","number":"555-1234"},{"type":"work","number":"555-5678"}],"children":["Jane","Doe"],"spouse":null,"salary":50000.50,"projects":[{"name":"ProjectAlpha","status":"completed","team":["Alice","Bob","Charlie"]},{"name":"ProjectBeta","status":"inprogress","team":["David","Eve"]}]})";
    jujson::json_parser<std::string> parser(data.begin(), data.end());

    auto val = parser.parse_json_object_expected();
    auto nameValue = val.find("name");
    std::string name = ((nameValue != val.get_childs().end()) ? nameValue->value().data() : "ERROR");
    std::cout << name << "\n\n";
    return (val.to_string() == data) && (name == "JohnDoe");
}
bool wchar_test() {
    std::cout << "wchar test\n";

    std::wstring data = LR"({"name":"JohnDoe","age":30,"isEmployed":true,"address":{"street":"123MainSt","city":"Anytown","state":"CA","postalCode":"12345"},"phoneNumbers":[{"type":"home","number":"555-1234"},{"type":"work","number":"555-5678"}],"children":["Jane","Doe"],"spouse":null,"salary":50000.50,"projects":[{"name":"ProjectAlpha","status":"completed","team":["Alice","Bob","Charlie"]},{"name":"ProjectBeta","status":"inprogress","team":["David","Eve"]}]})";
    jujson::json_parser<std::wstring> parser(data.begin(), data.end());

    auto val = parser.parse_json_object_expected();
    auto nameValue = val.find(L"name");
    std::wstring name = ((nameValue != val.get_childs().end()) ? nameValue->value().data() : L"ERROR");
    std::wcout << name << L"\n\n";
    return (val.to_string() == data) && (name == L"JohnDoe");
}
bool accessing() {
    std::cout << "accessing test\n";
    std::string jsonStr = R"({"name": "John", "age": 30, "isStudent": false, "courses": ["Math", "Science"]})";

    jujson::json_parser<std::string> parser(jsonStr);
    jujson::json_value<std::string> jsonObj = parser.parse_json_object_expected();

    if (jsonObj.is_valid()) {
        auto nameKV = jsonObj.find("name");
        if (nameKV != jsonObj.get_childs().end()) {
            std::cout << "Name: " << nameKV->value().data() << '\n';
        }

        auto ageKV = jsonObj.find("age");
        if (ageKV != jsonObj.get_childs().end()) {
            std::cout << "Age: " << ageKV->value().data() << '\n';
        }

        auto isStudentKV = jsonObj.find("isStudent");
        if (isStudentKV != jsonObj.get_childs().end()) {
            std::cout << "Is Student: " << isStudentKV->value().data() << '\n';
        }

        auto coursesKV = jsonObj.find("courses");
        if (coursesKV != jsonObj.get_childs().end()) {
            for (const auto& courseKVs : coursesKV->value().get_childs()) {
                std::cout << "Course: " << courseKVs.data() << '\n';
            }
        }
        std::cout << '\n';
        return true;
    } else {
        std::cerr << parser.line() << ':' << parser.column() << ':' << " error parsing JSON: " << parser.get_last_error() << '\n';
        return false;
    }
}
bool custom_type_test() {
    std::cout << "custom type test\n";
    std::string jsonStr = "{<name>: <John>, <age>: 30, <isStudent>: false, <courses>: [<Math - <about geometry>>, <Science>]}";

    jujson::json_parser<std::string, custom_trait> parser(jsonStr);
    jujson::json_value<std::string, custom_trait> jsonObj = parser.parse_json_object_expected();

    if (jsonObj.is_valid()) {
        auto nameKV = jsonObj.find("name");
        if (nameKV != jsonObj.get_childs().end()) {
            std::cout << "Name: " << nameKV->value().data() << '\n';
        }

        auto ageKV = jsonObj.find("age");
        if (ageKV != jsonObj.get_childs().end()) {
            std::cout << "Age: " << ageKV->value().data() << '\n';
        }

        auto isStudentKV = jsonObj.find("isStudent");
        if (isStudentKV != jsonObj.get_childs().end()) {
            std::cout << "Is Student: " << isStudentKV->value().data() << '\n';
        }

        auto coursesKV = jsonObj.find("courses");
        if (coursesKV != jsonObj.get_childs().end()) {
            for (const auto& courseKVs : coursesKV->value().get_childs()) {
                std::cout << "Course: " << courseKVs.data() << '\n';
            }
        }
        std::cout << '\n';
        return true;
    } else {
        std::cerr << parser.line() << ':' << parser.column() << ':' << " error parsing JSON: " << parser.get_last_error() << "\n\n";
        return false;
    }
}
int main() {
    std::cout << (int)default_test() + (int)wchar_test() + (int)accessing() + (int)custom_type_test();
    return 0;
}
