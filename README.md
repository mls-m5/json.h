Json.h
=================================

A header only json lib

Example usage

```c++

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

    return 0;
}

```

For more examples check out the `examples/` folder.

Getting started
--------------------

Clone down the library into `lib/json.h` or similar.

### Using CMake

assuming you have the lib installed in "lib/json.h"
```cmake
add_subdirectory(lib/json.h)
target_link_libraries(your-application-name PUBLIC json)
```


### using other build system

Add `lib/json.h/include` to your include directories

