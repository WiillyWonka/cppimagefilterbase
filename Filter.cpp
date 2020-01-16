#include "Filter.h"

Filter::Filter(image_data& imgData, int up, int left, int down, int right) : imgData(imgData) {
	//calculate coords
	if (up == 0) y0 = 0;
	else y0 = imgData.h / up;
	if (left == 0) x0 = 0;
	else x0 = imgData.w / left;
	if (down == 0) y1 = 0;
	else y1 = imgData.h / down;
	if (right == 0) x1 = 0;
	else x1 = imgData.w / right;
}

bool Filter::checkBounds(int x, int y) {
	if (x < x0 || x >= x1 || y < y0 || y >= y1) return false;
	return true;
}

KerFilter::KerFilter(image_data& imgData, int up, int left, int down, int right) :
	Filter(imgData, up, left, down, right) {}

vector<vector<int>> KerFilter::getMatrix(int index, Color color) {
	int centerX = index % imgData.w; //current coords
	int centerY = index / imgData.w;

	int add, currX, currY, k;
	switch (color)
	{
	case RED:
		add = 0;
		break;
	case GREEN:
		add = 1;
		break;
	case BLUE:
		add = 2;
		break;
	}

	vector<vector<int>> matrix(3, vector<int>(3));
	for (int biasX = -1, i = 0; biasX <= 1; biasX++, i++) {
		for (int biasY = -1, j = 0; biasY <= 1; biasY++, j++) {
			currX = centerX + biasX;
			currY = centerY + biasY;
			if (checkBounds(currX, currY) == true) {
				k = (currY * imgData.w + currX) * imgData.compPerPixel + add;
				matrix[i][j] = imgData.pixels[k];
			}
			else matrix[i][j] = 0;
		}
	}

	return matrix;
}

Red::Red(image_data& imgData, int up, int left, int down, int right) :
	Filter(imgData, up, left, down, right) {}

void Red::apply() {
	int index;
	for (int x = x0; x < x1; x++) {
		for (int y = y0; y < y1; y++) {
			index = (x + y * imgData.w) * imgData.compPerPixel;
			imgData.pixels[index] = (char)255; //red
			imgData.pixels[index + 1] = (char)0; //green
			imgData.pixels[index + 2] = (char)0; //blue
		}
	}
}

Blur::Blur(image_data& imgData, int up, int left, int down, int right) :
	KerFilter(imgData, up, left, down, right) {
	vector<vector<int>> matrix(3, vector<int>(3, 1));
	ker = NormKernel(matrix);
}

void Blur::apply() {
	int bufSize = (y1 - y0) * (x1 - x0) * imgData.compPerPixel;
	vector<char> buf(bufSize);
	int index;
	for (int x = x0, j = 0; x < x1; x++) {
		for (int y = y0; y < y1; y++, j += imgData.compPerPixel) {
			index = x + y * imgData.w;
			buf[j] = (char)ker.apply(getMatrix(index, RED));
			buf[j + 1] = (char)ker.apply(getMatrix(index, GREEN));
			buf[j + 2] = (char)ker.apply(getMatrix(index, BLUE));
		}
	}

	for (int x = x0, j = 0; x < x1; x++) {
		for (int y = y0; y < y1; y++, j += imgData.compPerPixel) {
			index = (x + y * imgData.w) * imgData.compPerPixel;
			imgData.pixels[index] = buf[j];
			imgData.pixels[index + 1] = buf[j + 1];
			imgData.pixels[index + 2] = buf[j + 2];
		}
	}
}

BlackWhite::BlackWhite(image_data& imgData, int up, int left, int down, int right)
	: Filter(imgData, up, left, down, right) {}

void BlackWhite::apply() {
	int I, red, green, blue, index;
	for (int x = x0; x < x1; x++) {
		for (int y = y0; y < y1; y++) {
			index = (x + y * imgData.w) * imgData.compPerPixel;
			red = imgData.pixels[index];
			green = imgData.pixels[index + 1];
			blue = imgData.pixels[index + 2];
			I = (3 * red + 6 * green + blue) / 10;
			imgData.pixels[index] = I;
			imgData.pixels[index + 1] = I;
			imgData.pixels[index + 2] = I;
		}
	}
}

inline int Edge::clamp(int i)
{
	if (i > 255) return 255;
	if (i < 0) return 0;
	else return i;
}

Edge::Edge(image_data& imgData, int up, int left, int down, int right)
	: KerFilter(imgData, up, left, down, right), bwFilter(imgData, up, left, down, right)
{
	vector<vector<int>> matrix(3, vector<int>(3, -1));
	matrix[1][1] = 9;
	ker = Kernel(matrix);
}

void Edge::apply() {
	bwFilter.apply();

	int bufSize = (y1 - y0) * (x1 - x0) * imgData.compPerPixel;
	vector<char> buf(bufSize);
	int index;
	for (int x = x0, j = 0; x < x1; x++) {
		for (int y = y0; y < y1; y++, j += imgData.compPerPixel) {
			index = (x + y * imgData.w);
			buf[j] = (char)clamp(ker.apply(getMatrix(index, RED)));
			buf[j + 1] = (char)clamp(ker.apply(getMatrix(index, GREEN)));
			buf[j + 2] = (char)clamp(ker.apply(getMatrix(index, BLUE)));
		}
	}

	for (int x = x0, j = 0; x < x1; x++) {
		for (int y = y0; y < y1; y++, j += imgData.compPerPixel) {
			index = (x + y * imgData.w) * imgData.compPerPixel;
			imgData.pixels[index] = buf[j];
			imgData.pixels[index + 1] = buf[j + 1];
			imgData.pixels[index + 2] = buf[j + 2];
		}
	}
}

unsigned char Treshold::getMediana(int center) {
	vector<unsigned char> intensityVector;

	int centerX = center % imgData.w; //current coords
	int centerY = center / imgData.w;

	int I, red, green, blue, index, currX, currY;
	for (int biasX = -2; biasX <= 2; biasX++) {
		currX = centerX + biasX;
		for (int biasY = -2; biasY <= 2; biasY++) {
			currY = centerY + biasY;
			if (checkBounds(currX, currY) == true) {
				index = ((currY) * imgData.w + currX) * imgData.compPerPixel;
				intensityVector.push_back(imgData.pixels[index]);
			}
		}
	}
	sort(intensityVector.begin(), intensityVector.end());
	return intensityVector[intensityVector.size() / 2];
}

Treshold::Treshold(image_data& imgData, int up, int left, int down, int right)
	: Filter(imgData, up, left, down, right), bwFilter(imgData, up, left, down, right) {}

void Treshold::apply() {
	bwFilter.apply();

	int I, red, green, blue, index;
	int bufSize = (y1 - y0) * (x1 - x0) * imgData.compPerPixel;
	unsigned char* buf = new unsigned char[bufSize];
	/*vector<unsigned char> buf;
	buf.reserve(bufSize);*/

	for (int x = x0, j = 0; x < x1; x++) {
		for (int y = y0; y < y1; y++, j += imgData.compPerPixel) {
			index = (x + y * imgData.w);

			if (imgData.pixels[index * imgData.compPerPixel] < getMediana(index)) {
				buf[j] = (unsigned char)0;
				buf[j + 1] = (unsigned char)0;
				buf[j + 2] = (unsigned char)0;
			}
			else {
				buf[j] = imgData.pixels[index*imgData.compPerPixel];
				buf[j + 1] = imgData.pixels[index * imgData.compPerPixel + 1];
				buf[j + 2] = imgData.pixels[index * imgData.compPerPixel + 2];
			}
		}
	}

	for (int x = x0, j = 0; x < x1; x++) {
		for (int y = y0; y < y1; y++, j += imgData.compPerPixel) {
			index = (x + y * imgData.w) * imgData.compPerPixel;
			imgData.pixels[index] = buf[j];
			imgData.pixels[index + 1] = buf[j + 1];
			imgData.pixels[index + 2] = buf[j + 2];
		}
	}

	delete buf;
}
