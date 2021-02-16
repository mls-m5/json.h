#include "json/json.h"
#include <iostream>

int main(int, char *[]) {
    {
        auto vec = std::vector<std::string>{"hello", "there"};

        // Construct json object from vector of strings
        auto json = Json{}.vector(vec);

        // Print json structure to standard out
        std::cout << json << "\n";
    }

    // Create hierachies
    {
        auto root = Json{};

        //! Creates new child
        auto &child = root["child1"];

        child["test1"] = "hello";
        child["test2"] = "hello";

        std::cout << root << "\n";
    }

    // Create arrays with values
    {
        auto json = Json{Json::Array};

        json.reserve(20); // Optional, speed up allocating when allocating much
        for (size_t i = 0; i < 20; ++i) {
            json.emplace_back(std::to_string(i));
        }

        std::cout << json << "\n";
    }

    return 0;
}
