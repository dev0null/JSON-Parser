#include "utils.hpp"

#include <fstream>

int main(){

    std::string json;
    std::string data;
    std::ifstream file("example.json");

    if (file.good())
    {
        while (std::getline(file, data))
            json.append(data);
        
        file.close();
    }
    else {
        json = R"(
            {
                "type": "Example JSON",
                "name": "Alice",
                "age": 30,
                "is_student": false,
                "address": {
                    "street": "123 Main St",
                    "city": "Wonderland"
                },
                "grades": [95.5, 88, 92.5],
                "metadata": null,
                "courses": [
                    "Math",
                    "Science", {
                        "name": "CS101",
                        "credits": 3
                    }
                ]
            }
        )";
    }

    try {
        JsonParser parser(json);
        JsonValue value = parser.parseValue();

        std::cout << "Parsed JSON:\n";
        printValue(value);
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}