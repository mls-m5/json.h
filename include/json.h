/*
 * json.h
 *
 *  Created on: 19 nov. 2016
 *      Author: mattias
 */

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <exception>
#include <fstream>

class Json: public std::vector<Json> {
public:
	enum Type {
		None = 0,
		Null,
		Object,
		Array,
		String,
		Number,
	};

	Type type = None;
	std::string name;
	std::string value;

	struct ParsingError: public std::exception {
		ParsingError(std::string info):
		errorString(info){
			what();
		}

		const char* what() const _GLIBCXX_USE_NOEXCEPT override {
			return errorString.c_str();
		}

		std::string errorString;
	};

	class Token {
	public:
		enum Type {
			None,
			Null,
			BeginBrace,
			EndBrace,
			BeginBracket,
			EndBracket,
			String,
			Number,
			Coma,
			Colon,
		};

		std::string value;
		Type type = None;
		Token() {};
//		Token(std::string value):
//			type(None),
//			value(value) {
//			if (!this->value.empty()) {
//				type == None;
//			}
//		}
		Token(Type type):
			type(type) {
		}
		Token(const Token&) = default;
		Token(Token && token):
			type(token.type),
			value(std::move(token.value)){

		}

		bool empty() {
			return value.empty();
		}

		Token &operator=(const Token &token) {
			this->type = token.type;
			this->value = token.value;
		}

	};

	Json() = default;
	Json(std::string str) {
		parse(str);
	}
	Json(const Json &json) = default;
	Json(Json &&) = default;
	Json(std::istream &stream) {
		parse(stream);
	}

	static Json loadFile(std::string fname) {
		std::ifstream file(fname);
		return Json(file);
	}

	Json &operator[] (const char *name) {
		return operator[] (std::string(name));
	}

	Json &operator[] (std::string n) {
		type = Object; //The operation converts the Json-object to a object
		value = "";
		this->name = "";
		auto f = find(n);
		if (f != end()) {
			return *f;
		}

		push_back(Json());
		back().name = n;
		return back();
	}


	bool operator==(std::string value) {
		return this->value == value;
	}

	Json& operator=(const Json &json) {
		type = json.type;
		if (type == Array || type == Object) {
			value.clear();
			auto n = name; //The name get cleared for some reason
			vector() = json.vector();
			name = n;
		}
		else {
			value = json.value;
			name = json.name;
		}
		return *this;
	}

	Json& operator=(Json &&json) {
		type = json.type;
		if (type == Array || type == Object) {
			value.clear();
			auto n = name;
			vector() = std::move(json.vector());
			name = n;
		}
		else
			value = std::move(json.value);
			name = std::move(json.name);{
		}
		return *this;
	}

	std::vector<Json> &vector() {
		return *((std::vector<Json>*) this);
	}


	std::vector<Json> &vector() const {
		return *((std::vector<Json>*) this);
	}

	operator bool() {
		if (type == None || type == Null || value.empty()) {
			return false;
		}
		else {
			return true;
		}
	}

	iterator find(std::string name) {
		for (auto it= begin(); it != end(); ++it) {
			if ((*it).name == name) {
				return it;
			}
		}
		return end();
	}

	iterator remove(const char* name) {
		remove (std::string(name));
	}

	iterator remove(std::string name) {
		auto f = find(name);
		return vector().erase(f);
	}

	static char getChar(std::istream &stream) {
		if (stream.eof()) {
			throw ParsingError("Unexpected end of file");
		}
		return stream.get();
	}

	static Token getNextToken(std::istream &stream) {
		Token ret;

		if (stream.eof()) {
			throw ParsingError("End of file when expecting character");
		}
		char c = getChar(stream);
		while (isspace(c)) {
			c = getChar(stream);
			if (stream.eof()) {
				throw ParsingError("End of file when expecting character");
			}
		}

		if (c == '"') {
			ret.type = Token::String;
			c = getChar(stream);
			while (c != '"') {
				if (c == '\\') {
					c = getChar(stream);
					switch (c) {
						case '"':
							ret.value += c;
							break;
						case 'b':
							ret.value += '\b';
							break;
						case 'r':
							ret.value += '\r';
							break;
						case 't':
							ret.value += '\t';
							break;
						case '\\':
							ret.value += '\\';
							break;
						case '\f':
							ret.value += '\f';
							break;
						default:
							throw ParsingError("illegal character in string");
							break;
					}
				}
				else {
					ret.value += c;
				}
				c = getChar(stream);
			}
			return ret;
		}
		if (isdigit(c) || c == '.' || c == '-') {
			ret.value += c;
			c = getChar(stream);

			while (isdigit(c) || c == '.') {
				ret.value += c;
				c = getChar(stream);
			}
			stream.unget();

			ret.type = Token::Number;
			return ret;
		}
		else if (c == '{') {
			return Token(Token::BeginBrace);
		}
		else if(c == '}') {
			return Token(Token::EndBrace);
		}
		else if (c == '[') {
			return Token(Token::BeginBracket);
		}
		else if(c == ']') {
			return Token(Token::EndBracket);
		}
		else if(c == ',') {
			return Token(Token::Coma);
		}
		else if(c == ':') {
			return Token(Token::Colon);
		}
		else if (c == 'n') {
			char c2, c3, c4;
			auto assertEq = [](std::istream & s, char c) {
				char nc = getChar(s);
				return (nc == c);
			};
			if (assertEq(stream, 'u') && assertEq(stream, 'l') && assertEq(stream, 'l')) {
				return Token(Token::Null);
			}
			else {
				throw ParsingError("unexpected token");
			}
		}
		else{
			throw "unexpected character";
		}
		return Token(Token::None);
	}

	void parse(std::string str) {
		std::istringstream ss(str);
		parse (ss);
	}

	void parse(std::istream &ss, Token rest = Token()) {
		Token token = rest;
		if (rest.type == rest.None) {
			token = getNextToken(ss);
			value = "";
		}
		if (token.type == token.String) {
			type = String;
			value = token.value;
		}
		else if (token.type == token.Number) {
			type = Number;
			value = token.value;
		}
		else if (token.type == token.BeginBrace) {
			type = Object;

			auto token = getNextToken(ss);

			if (token.type == token.EndBracket) {
				return; //Empty array
			}

			bool running = true;
			while (running) {
				Json json;
				json.name = token.value;

				token = getNextToken(ss);

				if (token.type != Token::Colon) {
					throw ParsingError("unexpexted token in object, expected ':'");
				}

				token = getNextToken(ss);

				json.parse(ss, token);
				if (json.type == None) {
					throw ParsingError("error in array");
				}

				push_back(json);

				token = getNextToken(ss);

				if (token.type == token.EndBrace) {
					return; //End of array
				}
				if (token.type == token.Coma) {
					token = getNextToken(ss);
				}
				else {
					throw ParsingError("unexpected character in array");
				}
			}
		}
		else if (token.type == token.Null) {
			type = Null;
		}
		else if (token.type == token.BeginBracket) {
			type = Array;

			auto token = getNextToken(ss);

			if (token.type == token.EndBracket) {
				return; //Empty array
			}

			bool running = true;
			while (running) {
				Json json;

				json.parse(ss, token);
				if (json.type == None) {
					throw ParsingError("error in array");
				}

				push_back(json);

				token = getNextToken(ss);

				if (token.type == token.EndBracket) {
					return; //End of array
				}
				if (token.type == token.Coma) {
					token = getNextToken(ss);
				}
				else {
					throw ParsingError("unexpected character in array");
				}
			}
		}
	}


	void indent(std::ostream &stream, int spaces) {
		for (int i = 0; i < spaces; ++i) {
			stream << " ";
		}
	}

	std::string stringify(int indent = 4) {
		std::ostringstream ss;
		stringify(ss, indent, 0);
		return ss.str();
	}

	static void escapeString(std::ostream &stream, std::string str) {
		stream << '"';
		for (auto c: str) {
			switch (c) {
			case '\n':
				stream << "\\n";
				break;
			case '\t':
				stream << "\\t";
				break;
			case '\f':
				stream << "\\f";
				break;
			case '\b':
				stream << "\\b";
				break;
			case '"':
				stream << "\\\"";
				break;
			default:
				stream << c;
			}
		}
		stream << '"';
	}

	void stringify(std::ostream &stream, int indent = 4, int startIndent = 0) {
		if (type == Number) {
			stream << value;
		}
		else if (type == String) {
			escapeString(stream, value);
		}
		else if (type == Null) {
			stream << "null";
		}
		else if (type == Object) {
			if (empty()) {
				stream << "{}";
				return;
			}
			stream << "{\n";
			for (auto &it: *this) {
				this->indent(stream, (startIndent + 1) * indent);
				escapeString(stream, it.name);
				stream << ": ";
				it.stringify(stream, indent, startIndent + 1);
				stream << ",\n";
			}
			this->indent(stream, startIndent * indent);
			stream << "}";
		}
		else if (type == Array) {
			if (empty()) {
				stream << "[]";
				return;
			}
			stream << "[\n";
			for (auto &it: *this) {
				this->indent(stream, (startIndent + 1) * indent);
				it.stringify(stream, indent, startIndent + 1);
				stream << ",\n";
			}
			this->indent(stream, startIndent * indent);
			stream << "]";
		}
	}

	inline std::string toString() {
		return stringify(2);
	}
};

std::ostream& operator << (std::ostream &stream, Json &json) {
	json.stringify(stream);
	return stream;
}



