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

void png_toolkit::fillFilters(vector<pair<string, vector<int>>> in) {
	filter.reserve(in.size());
	for (const pair<string, vector<int>>& filter_args : in) {
		if (filter_args.first == "Red") {
			vector<int> border = filter_args.second;
			filter.push_back(new Red(imgData, border[0], border[1], border[2], border[3]));
		}
		if (filter_args.first == "Edge") {
			vector<int> border = filter_args.second;
			filter.push_back(new Edge(imgData, border[0], border[1], border[2], border[3]));
		}
		if (filter_args.first == "Blur") {
			vector<int> border = filter_args.second;
			filter.push_back(new Blur(imgData, border[0], border[1], border[2], border[3]));
		}
		if (filter_args.first == "Threshold") {
			vector<int> border = filter_args.second;
			filter.push_back(new Treshold(imgData, border[0], border[1], border[2], border[3]));
		}
	}
}

void png_toolkit::applyFilters() {
	for (Filter* inst : filter) {
		inst->apply();
		cout << "done" << endl;
	}
}
