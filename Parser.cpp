#include "Parser.h"

vector<pair<string, vector<int>>> Parser::parse(string path) {
	std::ifstream f(path);
	vector<pair<string, vector<int>>> out;
	if (!f.is_open()) {
		return out;
	}

	while (!f.eof()) {
		std::string filterName;
		std::vector<int> borders;

		try {
			f >> filterName;

			int num;
			for (int i = 0; i < 4; i++) {
				f >> num;
				borders.push_back(num);
			}
			out.push_back(std::pair<std::string, std::vector<int>>(filterName, borders));
		}
		catch (...) {
			break;
		}
	}
	f.close();
	return out;
	/*
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
	return out;*/
}