/*
 * json_test.cpp
 *
 *  Created on: 19 nov. 2016
 *      Author: mattias
 */


#define MATUNIT_DISABLE_ERROR_HANDLING
#include "unittest.h"
#include "json.h"


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
			"    1,\n"
			"  ],\n"
			"}";


	ASSERT_EQ(json.toString(), outputTarget);
}

TEST_SUIT_END;


