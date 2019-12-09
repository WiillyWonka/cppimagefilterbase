#pragma once
#include <vector>

using namespace std;


class Kernel {
private:
	vector<vector<int>> matrix;
public:
	Kernel() {}
	Kernel(vector<vector<int>> matrix) : matrix(matrix) {}

	int apply(vector<vector<int>> field) {
		int sum = 0;
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[i].size(); j++) {
				sum += matrix[i][j] * field[i][j];
			}
		}

		return sum;
	}
};

class NormKernel : Kernel {
	int wsum;
public:
	NormKernel() : Kernel(), wsum(0) {}
	NormKernel(vector<vector<int>> matrix) : Kernel(matrix), wsum(0) {
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[i].size(); j++) {
				wsum += matrix[i][j];
			}
		}
	}

	int apply(vector<vector<int>> field) {
		return Kernel::apply(field) / wsum;
	}
};
