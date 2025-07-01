#pragma once

#include "JSONParser.h"
#include <iomanip>
#include <iostream>


void printIndent(int indent);
void printValue(const JsonValue& value, int indent = 0);
void printArray(const JsonArray& arr, int indent);
void printValue(const JsonValue& value, int indent);

inline void printIndent(int indent) {
    for (int i = 0; i < indent; ++i)
        std::cout << "  ";
}

inline void printObject(const JsonObject& obj, int indent) {
    bool first = true;
    std::cout << "{\n";

    for (const auto& [k, v] : obj){
        if (!first)
            std::cout << ",\n";
        first = false;
        printIndent(indent + 1);
        std::cout << std::quoted(k) << ": ";
        printValue(v, indent + 1);
    }

    std::cout << "\n";
    printIndent(indent);
    std::cout << "}";
}

inline void printArray(const JsonArray& arr, int indent){

    bool first = true;
    std::cout << "[\n";

    for (const auto& val : arr){
        if (!first)
            std::cout << ",\n";
        first = false;
        printIndent(indent + 1);
        printValue(val, indent + 1);
    }

    std::cout << "\n";
    printIndent(indent);
    std::cout << "]";

}

// Helper to print jsonvalue
inline void printValue(const JsonValue& value, int indent){
    const auto& val = value.value;

    if (auto s = std::get_if<std::string>(&val))
        std::cout << "\"" << *s << "\"";
    else if (auto i = std::get_if<int>(&val))
        std::cout << *i;
    else if (auto d = std::get_if<double>(&val)){
        std::cout.precision(15);
        std::cout << *d;
    }
    else if (auto b = std::get_if<bool>(&val))
        std::cout << std::boolalpha << *b;
    else if (std::holds_alternative<std::monostate>(val))
        std::cout << "null";
    else if (auto obj = std::get_if<JsonObjectPtr>(&val))
    {
        if (*obj)
            printObject(**obj, indent);
        else
            std::cout << "{}";
    }
    else if (auto arr = std::get_if<JsonArrayPtr>(&val))
    {
        if (*arr)
            printArray(**arr, indent);
        else
            std::cout << "[]";
    }
}