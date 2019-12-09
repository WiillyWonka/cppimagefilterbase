#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <array>
#include <sstream> 
#include "stb_image_write.h"
#include "png_toolkit.h"

png_toolkit::png_toolkit()
{
}

png_toolkit::~png_toolkit()
{
	stbi_image_free(imgData.pixels);
}

bool png_toolkit::load(const std::string& pictureName)
{
	imgData.pixels = stbi_load(pictureName.c_str(), &imgData.w, &imgData.h, &imgData.compPerPixel, 0);
	return imgData.pixels != nullptr;
}

bool png_toolkit::save(const std::string& pictureName)
{
	return stbi_write_png(pictureName.c_str(),
		imgData.w, imgData.h,
		imgData.compPerPixel,
		imgData.pixels, 0) != 0;
}


image_data png_toolkit::getPixelData(void) const
{
	return imgData;
}

void png_toolkit::fillFilters(vector<vector<string>> in) {
	filter.reserve(in.size());
	for (const vector<string>& str : in) {
		if (str[0] == "Red") filter.push_back(new Red(imgData, stoi(str[1]), stoi(str[2]), stoi(str[3]), stoi(str[4])));
		if (str[0] == "Edge") filter.push_back(new Edge(imgData, stoi(str[1]), stoi(str[2]), stoi(str[3]), stoi(str[4])));
		if (str[0] == "Blur") filter.push_back(new Blur(imgData, stoi(str[1]), stoi(str[2]), stoi(str[3]), stoi(str[4])));
		if (str[0] == "Treshold") filter.push_back(new Treshold(imgData, stoi(str[1]), stoi(str[2]), stoi(str[3]), stoi(str[4])));
	}
}

void png_toolkit::applyFilters() {
	for (Filter* inst : filter) {
		inst->apply();
		cout << "done" << endl;
	}
}
