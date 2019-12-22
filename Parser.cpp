#include "Parser.h"

vector<vector<string>> Parser::parse(string path) {
	string line, token;
	string delimiter = " ";
	vector<vector<string>> out;

	ifstream in(path);
	if (in.is_open())
	{
		while (getline(in, line))
		{
			vector<string> oneFilter;
			int pos = 0;
			while ((pos = line.find(delimiter)) != std::string::npos) {
				token = line.substr(0, pos);
				oneFilter.push_back(token);
				line.erase(0, pos + delimiter.length());
			}
			oneFilter.push_back(line.substr(0, pos));
			out.push_back(oneFilter);
		}
	}
	in.close();
	return out;
}