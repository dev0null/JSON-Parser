#include "JSONParser.h"

JsonParser::JsonParser(const std::string& input) : m_input(input){}

void JsonParser::skipWhitespace()
{
    while (m_position < m_input.size() 
        && std::isspace(static_cast<unsigned char>(m_input[m_position])))
    {
        ++m_position;
    }
}

char JsonParser::currentChar() const
{
    return (m_position < m_input.size()) ? m_input[m_position] : '\0';
}

void JsonParser::advance()
{
    if (m_position < m_input.size())
        ++m_position;
}

std::string JsonParser::parseString()
{
    if (currentChar() != '"')
        throw std::runtime_error("Expected '\"' at start of string");

    // Skip opening quote
    advance();

    std::string result;
    while (currentChar() != '"')
    {
        if (m_position >= m_input.size())
            throw std::runtime_error("Unterminated string");

        // Handle escape sequence
        if (currentChar() == '\\') {
            advance();
            switch (currentChar())
            {
                case '"':   result += '"';  break;
                case '\\':  result += '\\'; break;
                case '/':   result += '/';  break;
                case 'b':   result += '\b'; break;
                case 'f':   result += '\f'; break;
                case 'n':   result += '\n'; break;
                case 'r':   result += '\r'; break;
                case 't':   result += '\t'; break;
                default:
                    throw std::runtime_error("Invalid escape sequence");
            }
            advance();
        }
        else {
            result += currentChar();
            advance();
        }
    }
    
    advance(); // skip closing quote
    return result;
}


JsonValue JsonParser::parseNumber()
{
    std::string numStr;
    bool isDouble = false;

    // Handle negative sign
    if (currentChar() == '-') {
        numStr += '-';
        advance();
    }

    // Parse integer part
    while (std::isdigit(static_cast<unsigned char>(currentChar()))) {
        numStr += currentChar();
        advance();
    }

    // Check for decimal point
    if (currentChar() == '.') {
        isDouble = true;
        numStr += '.';
        advance();

        // Parse decimal part
        while (std::isdigit(static_cast<unsigned char>(currentChar()))) {
            numStr += currentChar();
            advance();
        }
    }

    // Check for exponent
    if (currentChar() == 'e' || currentChar() == 'E') {
        isDouble = true;
        numStr += currentChar();
        advance();

        if (currentChar() == '+' || currentChar() == '-') {
            numStr += currentChar();
            advance();
        }

        while (std::isdigit(static_cast<unsigned char>(currentChar()))) {
            numStr += currentChar();
            advance();
        }
    }

    // Convert to appropriate numeric type
    try {
        if (isDouble)
            return JsonValue{std::stod(numStr)};
        else
            return JsonValue{std::stoi(numStr)};
    }
    catch (...) {
        throw std::runtime_error("Invalid number format: " + numStr);
    }
    
}

JsonValue JsonParser::parseKeyword()
{
    // Check for true
    if (m_position + 4 <= m_input.size() && m_input.substr(m_position, 4) == "true")
    {
        m_position += 4;
        return JsonValue{true};
    }

    // Check for false
    if (m_position + 5 <= m_input.size() && m_input.substr(m_position, 5) == "false")
    {
        m_position += 5;
        return JsonValue{false};
    }

    // Check for null
    if (m_position + 4 <= m_input.size() && m_input.substr(m_position, 4) == "null")
    {
        m_position += 4;
        return JsonValue{std::monostate{}}; // Special value for null
    }
    throw std::runtime_error("Invalid keyword");
}


JsonValue JsonParser::parseArray()
{
    if (currentChar() != '[')
        throw std::runtime_error("Expected '[' at start of array");

    advance();
    skipWhitespace();

    auto array = std::make_unique<JsonArray>();

    // Handle empty array
    if (currentChar() == ']') {
        advance();
        return JsonValue{std::move(array)};
    }

    // Parse array elements
    while (true) {
        skipWhitespace();
        // Recursively parse elements
        array->push_back(parseValue());

        skipWhitespace();
        // Check for array end or element separator
        if (currentChar() == ']'){
            advance();
            break;
        }
        else if (currentChar() == ',') {
            advance();
        }
        else
            throw std::runtime_error("Expected separator ',' or closing bracket ']' in array");
        
    }
    return JsonValue{std::move(array)};
}


JsonValue JsonParser::parseObject()
{

    if (currentChar() != '{')
        throw std::runtime_error("Expected '{' at start of object");

    advance(); // Skip '{'
    skipWhitespace();

    auto object = std::make_unique<JsonObject>();

    // Handle empty object: {}
    if (currentChar() == '}'){
        advance();
        return JsonValue{std::move(object)};
    }
    
    while (true) {

        // Parse key
        if (currentChar() != '"')
            throw std::runtime_error("Expected string key as first field in object");

        std::string key = parseString();

        // Expect colon separator
        skipWhitespace();
        if (currentChar() != ':')
            throw std::runtime_error("Expected ':' after object key");

        advance();
        skipWhitespace();

        // Parse value and populate JsonObject (key-value pair)
        JsonValue value = parseValue();
        (*object)[key] = std::move(value);


        // Check for next element or object end
        skipWhitespace();
        if (currentChar() == '}') {
            advance();
            break;
        }
        else if (currentChar() == ',') {
            advance();
            skipWhitespace();
        }
        else {
            throw std::runtime_error("Expected '}' or ',' after object value");
        }

    }
    return JsonValue{std::move(object)};
}


JsonValue JsonParser::parseValue()
{
    skipWhitespace();
    char c = currentChar();


    switch (c) {
        case '"':
            return JsonValue{parseString()};
            
        case '{':
            return parseObject();

        case '[':
            return parseArray();

        case 't': case 'f': case 'n':
            return parseKeyword();

        case '-': case '0'...'9':
            return parseNumber();

        default:
            throw std::runtime_error(std::string("Unexpected character: '") + c + "'");
    }
}
