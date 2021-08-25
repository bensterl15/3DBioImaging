#pragma once

#include <eigen/unsupported/Eigen/CXX11/Tensor>
using namespace std;

class Slice3D {
public:
	Slice3D(Eigen::Tensor<float, 3, Eigen::RowMajor>* data_, Eigen::DSizes<ptrdiff_t, 3> * offsets_, Eigen::DSizes<ptrdiff_t, 3> * extents_) {
		data = data_;
		offsets = offsets_;
		extents = extents_;

		result = (data->slice(*offsets, *extents));
	}

	Eigen::Tensor<float, 3, Eigen::RowMajor> result;

private:
	Eigen::Tensor<float, 3, Eigen::RowMajor>* data;
	Eigen::DSizes<ptrdiff_t, 3> * offsets;
	Eigen::DSizes<ptrdiff_t, 3> * extents;
};

class Slice4D {
public:
	Slice4D(Eigen::Tensor<float, 4, Eigen::RowMajor>* data_, Eigen::DSizes<ptrdiff_t, 4>* offsets_, Eigen::DSizes<ptrdiff_t, 4>* extents_) {
		data = data_;
		offsets = offsets_;
		extents = extents_;

		result = (data->slice(*offsets, *extents));
	}

	Eigen::Tensor<float, 4, Eigen::RowMajor> result;

private:
	Eigen::Tensor<float, 4, Eigen::RowMajor>* data;
	Eigen::DSizes<ptrdiff_t, 4>* offsets;
	Eigen::DSizes<ptrdiff_t, 4>* extents;
};