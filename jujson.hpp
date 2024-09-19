/*  
    MIT License

    Copyright (c) 2024 Aidar Shigapov

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
/*
    about JSON: https://www.json.org/json-en.html
*/

#ifndef JUJSON_HPP_
#   define JUJSON_HPP_ 1

#if (!(defined JUJSON_MOVE))
#   include <utility>
#   define JUJSON_MOVE(val__) (::std::move(val__))
#endif

#if (!(defined JUJSON_DEFAULT_CONTAINER))
#   include <vector>
#   define JUJSON_DEFAULT_CONTAINER ::std::vector
#endif

#if (!(defined JUJSON_SIZE_TYPE))
#   include <cstdint>
#   define JUJSON_SIZE_TYPE ::std::size_t
#endif

#if (!(defined JUJSON_IS_SPACE))
#   include <cctype>
#   define JUJSON_IS_SPACE(c__) (::std::isspace(c__))
#endif

#if (!(defined JUJSON_IS_ALPHA))
#   include <cctype>
#   define JUJSON_IS_ALPHA(c__) (::std::isalpha(c__))
#endif

#if (!(defined JUJSON_IS_DIGIT))
#   include <cctype>
#   define JUJSON_IS_DIGIT(c__) (::std::isdigit(c__))
#endif

#if (!(defined JUJSON_ASSERT))
#   include <cassert>
#   define JUJSON_ASSERT(expr__) assert(expr__)
#endif

#include <iostream>

namespace jujson {
    enum json_value_kind {
        JUSJON_JSON_VALUE_NULL,
        JUSJON_JSON_VALUE_TRUE,
        JUSJON_JSON_VALUE_FALSE,
        JUSJON_JSON_VALUE_STRING,
        JUSJON_JSON_VALUE_KEY,
        JUSJON_JSON_VALUE_NUMBER,
        JUSJON_JSON_VALUE_ARRAY,
        JUSJON_JSON_VALUE_OBJECT,
        JUSJON_JSON_VALUE_INVALID,
    };

    enum error_code {
        JUJSON_ERROR_CODE_SUCCESS,
        JUJSON_ERROR_CODE_UNEXPECTED,
        JUJSON_ERROR_CODE_EOF,
    };

    template<class CharT_>
    struct default_json_traits {
        public:
        typedef CharT_ char_type;

        public: // WEIRD
        [[nodiscard]] constexpr static char_type get_string_litteral_begin() noexcept   {return '\"';}
        [[nodiscard]] constexpr static char_type get_string_litteral_end() noexcept     {return '\"';}
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
        [[nodiscard]] static json_value_kind value_kind_from_string(const StringT_& str) {
            if (str == "null") {            return JUSJON_JSON_VALUE_NULL;
            } else if (str == "true") {     return JUSJON_JSON_VALUE_TRUE;
            } else if (str == "false") {    return JUSJON_JSON_VALUE_FALSE;
            } else  {                       return JUSJON_JSON_VALUE_INVALID;
            }
            return JUSJON_JSON_VALUE_INVALID;
        }
    };
    template<>
    struct default_json_traits<wchar_t> {
        public:
        typedef wchar_t char_type;

        public: // WEIRD
        [[nodiscard]] constexpr static char_type get_string_litteral_begin() noexcept   {return L'\"';}
        [[nodiscard]] constexpr static char_type get_string_litteral_end() noexcept     {return L'\"';}
        [[nodiscard]] static bool is_string_litteral_begin(char_type c) noexcept        {return c == get_string_litteral_begin();}
        [[nodiscard]] static bool is_string_litteral_end(char_type c) noexcept          {return c == get_string_litteral_end();}
        [[nodiscard]] constexpr static char_type get_array_begin() noexcept             {return L'[';}
        [[nodiscard]] constexpr static char_type get_array_end() noexcept               {return L']';}
        [[nodiscard]] static bool is_array_begin(char_type c) noexcept                  {return c == get_array_begin();}
        [[nodiscard]] static bool is_array_end(char_type c) noexcept                    {return c == get_array_end();}
        [[nodiscard]] constexpr static char_type get_object_begin() noexcept            {return L'{';}
        [[nodiscard]] constexpr static char_type get_object_end() noexcept              {return L'}';}
        [[nodiscard]] static bool is_object_begin(char_type c) noexcept                 {return c == get_object_begin();}
        [[nodiscard]] static bool is_object_end(char_type c) noexcept                   {return c == get_object_end();}
        [[nodiscard]] constexpr static char_type get_comma() noexcept                   {return L',';}
        [[nodiscard]] static bool is_comma(char_type c) noexcept                        {return c == get_comma();}
        [[nodiscard]] constexpr static char_type get_colon() noexcept                   {return L':';}
        [[nodiscard]] static bool is_colon(char_type c) noexcept                        {return c == get_colon();}
        [[nodiscard]] static bool is_new_line(char_type c) noexcept                     {return c == L'\n';}
        [[nodiscard]] static bool is_escape_spec(char_type c) noexcept                  {return c == L'\\';}
        [[nodiscard]] static bool is_minus(char_type c) noexcept                        {return c == L'-';}
        [[nodiscard]] static bool is_exp_symbol(char_type c) noexcept                   {return (c == L'e') || (c == L'E');}
        [[nodiscard]] static bool is_dec_separator_symbol(char_type c) noexcept         {return (c == L'.');}
        template<class StringT_>
        [[nodiscard]] static json_value_kind value_kind_from_string(const StringT_& str) {
            if (str == L"null") {            return JUSJON_JSON_VALUE_NULL;
            } else if (str == L"true") {     return JUSJON_JSON_VALUE_TRUE;
            } else if (str == L"false") {    return JUSJON_JSON_VALUE_FALSE;
            } else  {                       return JUSJON_JSON_VALUE_INVALID;
            }
            return JUSJON_JSON_VALUE_INVALID;}
    };
    /**
     * @brief Represents a JSON value, which can be a null, boolean, string, number, array, or object.
     * @tparam StringT_ the type of the string to use.
     * @tparam JSONTraitsT_ the traits class used to define JSON-specific characteristics.
     */
    template <class StringT_, class JSONTraitsT_ = default_json_traits<typename StringT_::value_type>>
    struct json_value final {
        private:
        typedef JSONTraitsT_ json_traits;

        private:
        json_value_kind kind_;
        StringT_ data_; // data of json value. 
        JUJSON_DEFAULT_CONTAINER<json_value> childs_;

        public:
        ///@brief Constructs a default json_value object with invalid kind.
        json_value() : kind_(JUSJON_JSON_VALUE_INVALID), data_(), childs_() {
            
        }
        /// @brief Constructs a json_value object with the specified kind. @param kind the kind of the json_value.
        json_value(json_value_kind kind) : kind_(kind), data_(), childs_() {

        }
        /**
         * @brief Constructs a json_value object with the specified data and kind.
         * @param data the data of the json_value.
         * @param kind the kind of the json_value.
         */
        json_value(const StringT_& data, json_value_kind kind) : kind_(kind), data_(data), childs_() {

        }
        /** 
         * @brief Constructs a json_value object with the specified data and kind, moving the data.
         * @param data the data of the json_value.
         * @param kind the kind of the json_value.
         */
        json_value(StringT_&& data, json_value_kind kind) : kind_(kind), data_(JUJSON_MOVE(data)), childs_() {

        }

        public:
        /// @brief Gets the child elements of the json_value. @return A reference to the container of child elements.
        [[nodiscard]] JUJSON_DEFAULT_CONTAINER<json_value>& get_childs() {
            return childs_;
        }
        /// @brief Gets the child elements of the json_value. @return A reference to the container of child elements.
        [[nodiscard]] const JUJSON_DEFAULT_CONTAINER<json_value>& get_childs() const noexcept {
            return childs_;
        }
        /// @brief Gets the kind of the json_value. @return The kind of the json_value.
        [[nodiscard]] json_value_kind kind() const noexcept {
            return kind_;
        }
        /// @brief Checks if the json_value is valid. @return True if the json_value is valid, false otherwise.
        [[nodiscard]] bool is_valid() const noexcept {
            return kind_ != JUSJON_JSON_VALUE_INVALID;
        }
        /// @brief Gets the data of the json_value. @return A const reference to the data of the json_value.
        [[nodiscard]] const StringT_& data() const {
            return data_;
        }     
        /// @brief Gets the first child element if kind is equal to JUSJON_JSON_VALUE_KEY. @return A const reference to the child element.
        [[nodiscard]] const json_value& value() const {
            if (kind_ != JUSJON_JSON_VALUE_KEY)
                JUJSON_ASSERT(false);
            return childs_[0];
        }     
        /// @brief Adds a child element to the json_value. @param obj the child element to add.
        void add_child(const json_value& obj) {
            return childs_.push_back(obj);
        }   
        /// @brief Adds a child element to the json_value, moving the element. @param obj the child element to add.
        void add_child(json_value&& obj) {
            return childs_.push_back(JUJSON_MOVE(obj));
        }
        /**
         * @brief Finds a child element with the specified key.
         * @param toFind the key to find.
         * @return An iterator to the found element, or end() if not found. RETURNS KV(KEY/VALUE) NOT A VALUE ITSELF.
         */
        [[nodiscard]] typename JUJSON_DEFAULT_CONTAINER<json_value>::iterator find(const StringT_& toFind) noexcept {
            if (kind_ != JUSJON_JSON_VALUE_OBJECT)
                return childs_.end();
            for (auto i = childs_.begin(); i != childs_.end(); ++i) {
                if (i->data_ == toFind)
                    return i;
            }
            return childs_.end();
        }
        /**
         * @brief Finds a child element with the specified key.
         * @param toFind the key to find.
         * @return An iterator to the found element, or end() if not found.
         */
        [[nodiscard]] typename JUJSON_DEFAULT_CONTAINER<json_value>::const_iterator find(const StringT_& toFind) const noexcept {
            if (kind_ != JUSJON_JSON_VALUE_OBJECT)
                return childs_.end();
            for (auto i = childs_.begin(); i != childs_.end(); ++i) {
                if (i->data_ == toFind)
                    return i;
            }
            return childs_.end();
        }
        /**
         * @brief Converts the json_value to a string.
         * @tparam ToStringT_ the type of the string to use.
         * @tparam ToJSONTraitsT_ the traits class used to define JSON-specific characteristics.
         * @return the string representation of the json_value.
         */
        template<class ToStringT_ = StringT_, class ToJSONTraitsT_ = default_json_traits<typename ToStringT_::value_type>>
        [[nodiscard]] ToStringT_ to_string() const {
            using c_json_traits = ToJSONTraitsT_;
            switch (kind_) {
            case JUSJON_JSON_VALUE_ARRAY: {
                ToStringT_ result;
                result += c_json_traits::get_array_begin();
                auto i = childs_.begin();
                for (; i != childs_.end() - 1; ++i) {
                    result += i->to_string() + c_json_traits::get_comma();
                }
                result += i->to_string() + c_json_traits::get_array_end();
                return result;
            }
            case JUSJON_JSON_VALUE_OBJECT: {
                ToStringT_ result;
                result += c_json_traits::get_object_begin();
                auto i = childs_.begin();
                for (; i != childs_.end() - 1; ++i) {
                    result += i->to_string() + c_json_traits::get_comma();
                }
                result += i->to_string() + c_json_traits::get_object_end();
                return result;
            }
            case JUSJON_JSON_VALUE_KEY:
                return ToStringT_(c_json_traits::get_string_litteral_begin() + data_ + c_json_traits::get_string_litteral_end() + c_json_traits::get_colon() + childs_[0].to_string());
            case JUSJON_JSON_VALUE_STRING:
                return ToStringT_(c_json_traits::get_string_litteral_begin() + data_ + c_json_traits::get_string_litteral_end());
            case JUSJON_JSON_VALUE_NULL:
            case JUSJON_JSON_VALUE_TRUE:
            case JUSJON_JSON_VALUE_FALSE:
            case JUSJON_JSON_VALUE_NUMBER:
                return ToStringT_(data_);
            default:
                return ToStringT_();
            }
            return ToStringT_();
        }
        
    };
    /**
     * @brief A robust JSON parser designed to convert JSON-formatted strings into structured json_value objects.
     *        This class handles various JSON data types, including null, boolean(true, false), string, number, array, and object.
     *        It provides methods to parse JSON strings, manage parsing errors, and track the current position in the input string.
     * @tparam StringT_ the type of the string used to use.
     * @tparam JSONTraitsT_ the traits class used to define JSON-specific characteristics.
     */
    template<class StringT_, class JSONTraitsT_ = default_json_traits<typename StringT_::value_type>>
    struct json_parser final {
        private:
        typedef JSONTraitsT_ json_traits;
        typedef json_value<StringT_, json_traits> json_value_type;

        public:
        typedef typename StringT_::const_iterator const_iterator;

        private:
        JUJSON_SIZE_TYPE line_;
        JUJSON_SIZE_TYPE column_;
        const_iterator current_;
        const_iterator end_;
        error_code lastError_;

        public:
        /// @brief Constructs a json_parser object with the specified string. @param str the string to parse.
        json_parser(const StringT_& str) : line_(0), column_(0), current_(str.begin()), end_(str.end()), lastError_(JUJSON_ERROR_CODE_SUCCESS) {

        }
        /// @brief Constructs a json_parser object with the specified iterators. @param b the beginning iterator. @param e the ending iterator.
        json_parser(const_iterator b, const_iterator e) : line_(0), column_(0), current_(b), end_(e), lastError_(JUJSON_ERROR_CODE_SUCCESS) {

        }

        private:
        bool skip_spaces() {
            for (auto val = *current_;current_ != end_; ++current_, val = *current_) { // skip spaces
                if (!JUJSON_IS_SPACE(val)) {
                    return true;
                }

                if (json_traits::is_new_line(val)) {
                    column_ = 0;
                    ++line_;
                } else {
                    ++column_;
                }
            }
            return false;
        }
        StringT_ parse_word() {
            auto b = current_;
            for (auto val = *current_ ;(current_ != end_) && (JUJSON_IS_ALPHA(val)); ++current_, val = *current_) { // pass non spaces
                ++column_;
            }
            return StringT_(b, current_);
        }
        StringT_ parse_number() {
            const auto skipInteger = [](const_iterator& current, const_iterator end, JUJSON_SIZE_TYPE& column) {
                for (auto val = *current;current != end; ++current, val = *current) { // skip spaces
                    if (!JUJSON_IS_DIGIT(val)) {
                        return true;
                    }
                    ++column;
                }
                return false;
            };
            auto b = current_;
            if (json_traits::is_minus(*current_)) {
                ++current_;
                ++column_;
            }
            skipInteger(current_, end_, column_);
            if (json_traits::is_dec_separator_symbol(*current_)) {
                ++current_;
                ++column_;
                if (!skipInteger(current_, end_, column_))
                    return StringT_(b, current_);
            }
            if (json_traits::is_exp_symbol(*current_)) {
                ++current_;
                ++column_;
                if (!skipInteger(current_, end_, column_))
                    return StringT_(b, current_);
            }
            return StringT_(b, current_);
        }
        StringT_ parse_string_litteral() {
            auto b = current_;
            bool slash = false;
            for (auto val = *current_; current_ != end_; ++current_, val = *current_) { // pass to string itteral end
                if (json_traits::is_escape_spec(val)) {
                    slash = true;
                } else if ((!slash) && json_traits::is_string_litteral_end(val))  {
                    ++current_;
                    ++column_;
                    return StringT_(b, current_ - 1);
                }
                if (json_traits::is_new_line(val)) {
                    column_ = 0;
                    ++line_;
                } else {
                    ++column_;
                }
                slash = false;
            }
            lastError_ = JUJSON_ERROR_CODE_EOF;
            return StringT_();
        }
        StringT_ parse_string_litteral_expected() {
            if (!skip_spaces()) {
                lastError_ = JUJSON_ERROR_CODE_EOF;
                return StringT_();
            }
            if (!json_traits::is_string_litteral_begin(*current_)) {
                lastError_ = JUJSON_ERROR_CODE_UNEXPECTED;
                return StringT_();
            }
            ++current_;
            ++column_;
            return parse_string_litteral();
        }
        json_value_type parse_word_json() {
            StringT_ str = parse_word();
            const json_value_kind kind = json_traits::value_kind_from_string(str);
            if (kind == JUSJON_JSON_VALUE_INVALID) {
                lastError_ = JUJSON_ERROR_CODE_UNEXPECTED;
                return json_value_type(); 
            }
            return json_value_type(JUJSON_MOVE(str), kind);
        }
        json_value_type parse_number_json() {
            const StringT_ str = parse_number();
            if (str.empty()) {
                lastError_ = JUJSON_ERROR_CODE_EOF;
                return json_value_type();
            }
            return json_value_type(str, JUSJON_JSON_VALUE_NUMBER); 
        }
        json_value_type parse_json_array() {
            json_value_type arrayResult(JUSJON_JSON_VALUE_ARRAY);
            bool commaExpected = false;
            for (;;) {
                if (!skip_spaces())
                    break;
                auto val = *current_;

                if (json_traits::is_array_end(val)) {
                    ++column_;
                    ++current_;
                    return arrayResult;
                } else if (commaExpected) { 
                    if (!json_traits::is_comma(val)) {
                        lastError_ = JUJSON_ERROR_CODE_UNEXPECTED;
                        return json_value_type();
                    }
                    commaExpected = false;
                    ++column_;
                    ++current_;
                    continue;
                }
                json_value_type newChild = parse_any_value();
                if (!newChild.is_valid())
                    return json_value_type();
                arrayResult.add_child(JUJSON_MOVE(newChild));
                commaExpected = true;
            }
            lastError_ = JUJSON_ERROR_CODE_EOF;
            return json_value_type();
        }
        
        json_value_type parse_json_array_expected() {
            for (auto val = *current_; current_ != end_; ++current_, val = *current_) { // skip spaces
                if (json_traits::is_array_begin(val)) {
                    ++current_;
                    return parse_json_array();
                }
            } // no string litterals
            lastError_ = JUJSON_ERROR_CODE_EOF;
            return json_value_type();
        }
        json_value_type parse_json_object() {
            json_value_type objectResult(JUSJON_JSON_VALUE_OBJECT);
            bool commaExpected = false;
            for (;;) {
                if (!skip_spaces())
                    break;
                auto val = *current_;
                if (json_traits::is_object_end(val)) {
                    ++column_;
                    ++current_;
                    return objectResult;
                }
                if (commaExpected) { 
                    if (!json_traits::is_comma(val)) {
                        lastError_ = JUJSON_ERROR_CODE_UNEXPECTED;
                        return json_value_type();
                    }
                    commaExpected = false;
                    ++column_;
                    ++current_;
                    continue;
                }
                json_value_type keyChild(parse_string_litteral_expected(), JUSJON_JSON_VALUE_KEY);
                if (keyChild.data().empty())
                    return json_value_type();
                if (!skip_spaces())
                    break;

                val = *current_;
                if (!json_traits::is_colon(val)) {
                    lastError_ = JUJSON_ERROR_CODE_UNEXPECTED;
                    return json_value_type();
                }
                ++column_;
                ++current_;
                if (!skip_spaces())
                    break;
                json_value_type valueChild = parse_any_value();
                if (!valueChild.is_valid())
                    return json_value_type();
                keyChild.add_child(valueChild);
                objectResult.add_child(JUJSON_MOVE(keyChild));
                commaExpected = true;
            }
            lastError_ = JUJSON_ERROR_CODE_EOF;
            return json_value_type();
        }
        json_value_type parse_any_value() {
            const auto val = *current_;
            if (JUJSON_IS_ALPHA(val)) {
                json_value_type wordChild = parse_word_json();
                if (!wordChild.is_valid())
                    return json_value_type();
                return wordChild;
            } else if (json_traits::is_string_litteral_begin(val)) {
                ++column_;
                ++current_;

                StringT_ data = parse_string_litteral();
                if (data.empty())
                    return json_value_type();
                return json_value_type(JUJSON_MOVE(data), JUSJON_JSON_VALUE_STRING);
            } else if (json_traits::is_array_begin(val)) {
                ++column_;
                ++current_;

                json_value_type arrayChild = parse_json_array();
                if (!arrayChild.is_valid())
                    return json_value_type();
                return arrayChild;
            } else if (json_traits::is_object_begin(val)) {
                ++column_;
                ++current_;

                json_value_type objChild = parse_json_object();
                if (!objChild.is_valid())
                    return json_value_type();
                return objChild;
            } else if (JUJSON_IS_DIGIT(val) || json_traits::is_minus(val)) {
                json_value_type numberChild = parse_number_json();
                if (!numberChild.is_valid())
                    return json_value_type();
                return numberChild;
            }
            lastError_ = JUJSON_ERROR_CODE_UNEXPECTED;
            return json_value_type(); // unknown symbol
        }

        public:
        /// @brief Parses a JSON object from the input. @return The parsed json_value object.
        [[nodiscard]] json_value_type parse_json_object_expected() {
            for (auto val = *current_; current_ != end_; ++current_, val = *current_) {
                if (json_traits::is_object_begin(val)) {
                    ++current_;
                    return parse_json_object();
                }
            } // no string litterals
            lastError_ = JUJSON_ERROR_CODE_EOF;
            return json_value_type();
        }
        /// @brief Gets the current column position in the input. @return The current column position.
        [[nodiscard]] JUJSON_SIZE_TYPE column() const {
            return column_;
        }
        /// @brief Gets the current line position in the input. @return The current line position.
        [[nodiscard]] JUJSON_SIZE_TYPE line() const {
            return line_;
        }
        /// @brief Gets the last error code encountered during parsing. @return The last error code.
        [[nodiscard]] error_code get_last_error() const {
            return lastError_;
        }
    };
};
#endif // ifndef JUJSON_HPP_