#include "json/json.h"
#include <iostream>

const auto jsonStr = std::string{R"_(

{
  "test": "hello",
  "sub": {
      "array": [1, 2, 3],
      "value": 10
  }
}

)_"};

int main(int, char *[]) {
    {
        const auto json = Json::Parse(jsonStr);

        std::cout << json["test"] << "\n"; // Prints "hello"

        // Print all elements in sub.array
        for (auto &j : json["sub"]["array"]) {
            std::cout << "array element: " << j.value << "\n";
        }
    }

    {
        // Load file
        // Presume that the file exists
        auto json = Json::LoadFile("test.json");

        // Prints "hello"
        // string() method verify that the value is actually of string type
        std::cout << json["test"].string() << std::endl;

        json["test"] = "bye";

        // Save to file
        json.saveFile("test-out.json");
    }

    // Other ways to parse strings
    {
        auto json = Json{};
        std::ifstream{"test.json"} >> json;
    }

    {
        auto json = Json{};
        auto file = std::fstream{"test.json"};
        json.parse(file);
    }

    return 0;
}
