
# jujson
**jujson** is a lightweight, header-only C++11 library designed for parsing JSON. It provides a simple and efficient way to parse JSON strings into structured `json_value` objects, supporting various JSON data types including null, boolean, string, number, array, and object.

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Examples](#examples)
- [Acknowledgments](#acknowledgments)
- [License](#license)
  
## Features

- Header-Only: No need to compile separate libraries; just include the header file.
- C++11 Compatible: Works with C++11 and later standards.
- Efficient Parsing: Designed to be fast and lightweight.
- Customizable: Easily extendable with custom traits and value types.
- ### JSON Value Types

`jujson` supports the following JSON value types:

- Null: `JUSJON_JSON_VALUE_NULL`
- Boolean: `JUSJON_JSON_VALUE_TRUE`, `JUSJON_JSON_VALUE_FALSE`
- String: `JUSJON_JSON_VALUE_STRING`
- Number: `JUSJON_JSON_VALUE_NUMBER`
- Array: `JUSJON_JSON_VALUE_ARRAY`
- Object: `JUSJON_JSON_VALUE_OBJECT`

### JSON Traits

You can customize the JSON traits by defining your own traits class. The default traits are provided for `char` and `wchar_t`.

```cpp
struct custom_json_traits {
    typedef char char_type;

    static constexpr char_type get_string_litteral_begin() noexcept { return '>'; } // default - "
    static constexpr char_type get_string_litteral_end() noexcept { return '<'; } // default - "
    static bool is_string_litteral_begin(char_type c) noexcept { return c == get_string_litteral_begin(); }
    static bool is_string_litteral_end(char_type c) noexcept { return c == get_string_litteral_end(); }
    // Define other traits as needed...
};
```

### Error Handling

`jujson` provides error codes to help with debugging:

- `jujson_ERROR_CODE_SUCCESS`: Successful parsing.
- `jujson_ERROR_CODE_UNEXPECTED`: Unexpected character encountered.
- `jujson_ERROR_CODE_EOF`: End of file reached unexpectedly.

You can check the last error code using `parser.get_last_error()`.

## Installation

To use `jujson`, simply include the header file in your project:

```cpp
#include "jujson.hpp"
```


## Usage
jujson is designed to be a simple and (maybe)efficient JSON parsing library. 
It can be used in various applications where JSON data needs to be parsed and manipulated.
## Examples
### Basic
Here is a basic example of how to use `jujson` to parse a JSON string:

```cpp
#include <iostream>
#include "jujson.hpp"

int main() {
    std::string jsonStr = R"({"name": "John", "age": 30, "isStudent": false, "courses": ["Math", "Science"]})";

    jujson::json_parser<std::string> parser(jsonStr);
    jujson::json_value<std::string> jsonObj = parser.parse_json_object_expected();

    if (jsonObj.is_valid()) {
        std::cout << "Parsed JSON: " << jsonObj.to_string() << '\n';
        return 0;
    } else {
        std::cerr << "Error parsing JSON: " << parser.get_last_error() << '\n';
        return 1;
    }

    return 0;
}
```
### Accessing 
```cpp
#include <iostream>
#include "jujson.hpp"

int main() {
    std::string jsonStr = R"({"name": "John", "age": 30, "isStudent": false, "courses": ["Math", "Science"]})";

    jujson::json_parser<std::string> parser(jsonStr);
    jujson::json_value<std::string> jsonObj = parser.parse_json_object_expected();

    if (jsonObj.is_valid()) {
        // Accessing string value
        auto nameKV = jsonObj.find("name");
        if (nameKV != jsonObj.get_childs().end()) {
            std::cout << "Name: " << nameKV->value().data() << '\n';
        }

        // Accessing number value
        auto ageKV = jsonObj.find("age");
        if (ageKV != jsonObj.get_childs().end()) {
            std::cout << "Age: " << ageKV->value().data() << '\n';
        }

        // Accessing boolean value
        auto isStudentKV = jsonObj.find("isStudent");
        if (isStudentKV != jsonObj.get_childs().end()) {
            std::cout << "Is Student: " << isStudentKV->value().data() << '\n';
        }

        // Accessing array value
        auto coursesKV = jsonObj.find("courses");
        if (coursesKV != jsonObj.get_childs().end()) {
            for (const auto& courseKVs : coursesKV->value().get_childs()) {
                std::cout << "Course: " << courseKVs.data() << '\n';
            }
        }
        return 0;
    } else {
        std::cerr << parser.line() << ':' << parser.column() << ':' << " error parsing JSON: " << parser.get_last_error() << '\n';
        return 1;
    }
    return 0;
}
```
### Custom Trait Type 
```cpp
#include <iostream>
#include "jujson.hpp"
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
    // . . . - copy from default traits
};

int main() {
    std::string jsonStr = R"({<name>: <John>, <age>: 30, <isStudent>: false, <courses>: [<Math>, <Science>]})";

    jujson::json_parser<std::string> parser(jsonStr);
    jujson::json_value<std::string> jsonObj = parser.parse_json_object_expected();

    if (jsonObj.is_valid()) {
        std::cout << "Parsed JSON: " << jsonObj.to_string() << '\n';
        return 0;
    } else {
        std::cerr << "Error parsing JSON: " << parser.get_last_error() << '\n';
        return 1;
    }
    return 0;
}
```
## Acknowledgments
- JSON Specification: https://www.json.org/json-en.html

## License

This library is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
