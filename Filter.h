#ifndef FILTER_H
#define FILTER_H

#include <algorithm>
#include "Kernel.h"
#include "image_data.h"

enum Color {
	RED,
	GREEN,
	BLUE
};

class Filter {
protected:
	image_data& imgData;
	int from, to, x0, x1, y0, y1;
	bool checkBounds(int x, int y);
public:
	Filter(image_data& imgData, int up, int left, int down, int right);
	virtual void apply() = 0;
};

class KerFilter : public Filter {
public:
	KerFilter(image_data& imgData, int up, int left, int down, int right);
	vector<vector<int>> getMatrix(int i, Color color);
};

class Red : public Filter {
public:
	Red(image_data& imgData, int up, int left, int down, int right);
	void apply();
};

class Blur : public KerFilter {
private:
	NormKernel ker;
public:
	Blur(image_data& imgData, int up, int left, int down, int right);
	void apply();
};

class BlackWhite : public Filter {
public:
	BlackWhite(image_data& imgData, int up, int left, int down, int right);
	void apply();
};

class Edge : public KerFilter {
private:
	Kernel ker;
	BlackWhite bwFilter;
	int clamp(int);
public:
	Edge(image_data& imgData, int up, int left, int down, int right);
	void apply();
};

class Treshold : public Filter {
private:
	BlackWhite bwFilter;
	vector<int> getIntensityVector(int center);
public:
	Treshold(image_data& imgData, int up, int left, int down, int right);
	void apply();
};

#endif
