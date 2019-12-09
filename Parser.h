#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Parser {
public:
	static vector<vector<string>> parse(string path);
};

#endif
