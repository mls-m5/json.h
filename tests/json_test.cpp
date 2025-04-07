/*
 * json_test.cpp
 *
 *  Created on: 19 nov. 2016
 *      Author: mattias
 */

#include "mls-unit-test/unittest.h"
#include "json/json.h"

using namespace std::literals;

TEST_SUIT_BEGIN(Jsontest)

TEST_CASE("simple parsing test") {
    {
        auto json = Json::Parse("[ 1, 2]");

        ASSERT_EQ(json.type, json.Array);
        ASSERT_EQ(json.size(), 2);
    }

    {
        auto json = Json::Parse("{ \"hej\": 1 }");

        ASSERT_EQ(json.type, Json::Object);
        auto res = json[std::string("hej")];
        ASSERT_EQ(res.type, Json::Number);
    }
}

TEST_CASE("dump") {
    auto json = Json::Parse("{\"a\": [1, 1]}");

    std::string outputTarget = "{\n"
                               "  \"a\": [\n"
                               "    1,\n"
                               "    1\n"
                               "  ]\n"
                               "}";

    ASSERT_EQ(json.toString(), outputTarget);
}

TEST_CASE("numbers") {
    auto json = Json{10};

    ASSERT_EQ(json.toString(), "10");
    ASSERT_EQ(json.type, Json::Number);
}

TEST_CASE("remove values") {
    auto json = Json::Parse("{\"a\": null, \"b\": null}");
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

    auto json = Json::Parse(testJson);

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

    auto json = Json::Parse(testJson);

    (void)json; // The test is not to crash
}

TEST_CASE("position") {
    auto testJson = R"_(
{
   "x": "hello",
   "yy": "there"
}
    )_";

    auto json = Json::Parse(testJson);

    auto x = json["x"];
    std::cout << "x position " << x.line() << "," << x.col() << std::endl;
    ASSERT_EQ(x.line(), 3);
    ASSERT_EQ(x.col(), 16);

    auto yy = json["yy"];
    ASSERT_EQ(yy.line(), 4);
    ASSERT_EQ(yy.col(), 17);
}

TEST_CASE("booleans") {
    auto testJson = R"_(
{
   "x": true,
   "yy": false
}
    )_";

    auto json = Json::Parse(testJson);

    auto x = json["x"];
    auto y = json["yy"];

    ASSERT_EQ(x.type, Json::Boolean);
    ASSERT_EQ(x.value, "true");
    ASSERT_EQ(y.type, Json::Boolean);
    ASSERT_EQ(y.value, "false");
}

TEST_SUIT_END;
