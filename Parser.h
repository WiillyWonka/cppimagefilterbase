#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Parser {
public:
	static vector<pair<string, vector<int>>> parse(string path);
};

#endif
