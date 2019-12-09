#ifndef PNG_TOOLKIT_H
#define PNG_TOOLKIT_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "stb_image.h"
#include "image_data.h"
#include "Filter.h"
#include "Parser.h"

class png_toolkit

{
public:
	enum class Error {
		WrongSize,
		WrongFormat,
		Ok
	};

	png_toolkit();
	~png_toolkit();
	bool load(std::string const& pictureName);
	bool save(std::string const& pictureName);
	image_data getPixelData(void) const;
	void fillFilters(vector<vector<string>>);
	void applyFilters();

private:
	image_data imgData;
	std::vector<Filter*> filter;
};
#endif // PNG_TOOLKIT_H