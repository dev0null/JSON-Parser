#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <map>
#include <vector>


// Define json value types

// Forward declarations
struct JsonValue;
using JsonArray     = std::vector<JsonValue>;
using JsonArrayPtr  = std::unique_ptr<JsonArray>;
using JsonObject    = std::map<std::string, JsonValue>; // represent the json key-value pair
using JsonObjectPtr = std::unique_ptr<JsonObject>;

// represent a type-safe union containing the main JSON value types
typedef struct JsonValue {

    std::variant<
        std::string,
        int,
        double,
        bool,
        std::monostate, // null
        JsonArrayPtr,   // JSON Array
        JsonObjectPtr   // JSON Object
    > value;

    // Constructors
    JsonValue()                 : value(std::monostate{}) {}
    JsonValue(std::string val)  : value(std::move(val)) {}
    JsonValue(int val)          : value(val) {}
    JsonValue(double val)       : value(val) {}
    JsonValue(bool val)         : value(val) {}
    JsonValue(std::monostate)   : value(std::monostate{}) {}
    JsonValue(JsonObjectPtr obj) : value(std::move(obj)) {}
    JsonValue(JsonArrayPtr arr) : value(std::move(arr)) {}

} JsonValue;



class JsonParser {
public:

    explicit JsonParser(const std::string& input);
    ~JsonParser();

    // Main parsing interface
    JsonValue parseValue();

    // Error information
    struct ParseError : public std::runtime_error {
        size_t position;
        ParseError(const std::string& msg, size_t pos)
            : std::runtime_error(msg + " at position " + std::to_string(pos)),
              position(pos) {}
    };

private:

    const std::string& m_input;
    size_t m_position = 0;

    /**
     * @brief Skip all sequential whitespaces (tab, newline, space)
     */
    void skipWhitespace();

    // Get current character (or null terminator if at end)
    char currentChar() const;

    // Advance to next character
    void advance();

    // Parse a quoted string
    std::string parseString();

    // Parse a number (int or double)
    JsonValue parseNumber();

    // Keyword parsing for true/false/null
    JsonValue parseKeyword();

    // Parse JSON array
    JsonValue parseArray();

    // Parse JSON object
    JsonValue parseObject();

};