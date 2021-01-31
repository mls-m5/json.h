/*
 * json_test.cpp
 *
 *  Created on: 19 nov. 2016
 *      Author: mattias
 */

#define MATUNIT_DISABLE_ERROR_HANDLING
#include "unittest.h"
#include "json/json.h"

using namespace std::literals;

TEST_SUIT_BEGIN

TEST_CASE("simple parsing test") {
    {
        Json json("[ 1, 2]");

        ASSERT_EQ(json.type, json.Array);
        ASSERT_EQ(json.size(), 2);
    }

    {
        Json json("{ \"hej\": 1 }");

        ASSERT_EQ(json.type, Json::Object);
        auto res = json[std::string("hej")];
        ASSERT_EQ(res.type, Json::Number);
    }
}

TEST_CASE("dump") {
    Json json("{\"a\": [1, 1]}");

    std::string outputTarget = "{\n"
                               "  \"a\": [\n"
                               "    1,\n"
                               "    1\n"
                               "  ]\n"
                               "}";

    ASSERT_EQ(json.toString(), outputTarget);
}

TEST_CASE("remove values") {
    Json json("{\"a\": null, \"b\": null}");
    ASSERT_EQ(json["a"].type, json.Null);

    ASSERT_EQ(json.size(), 2);

    json.remove("a");

    ASSERT_EQ(json.size(), 1);
}

TEST_CASE("other test") {
    auto testJson = R"_(
            {
               "palette": {
                  "hello": "#abcdef",
                   "aluminium4-3-blend":"#a1a49e",
                   "aluminium6-5-blend":"#34393A"
               },
               "style": {
                  "standard": "hello"
               }
            }
     )_"s;

    Json json(testJson);

    ASSERT_EQ(json["palette"].type, Json::Object);
    ASSERT_EQ(json.front().name, "palette");
    ASSERT_EQ(json["style"].type, Json::Object);
    ASSERT_EQ(json.back().name, "style");
}

TEST_CASE("special characters") {
    auto testJson = R"_(
        {
            "x": "\"\n\b\r\t\n\\\f\u"
        }
    )_"s;

    auto json = Json{testJson};

    (void)json; // The test is not to crash
}

TEST_SUIT_END;
