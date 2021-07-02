#pragma once

#include <complex>
#include <eigen/unsupported/Eigen/CXX11/Tensor>

using namespace std;

extern "C" 
{
#include <fftw3.h>
}

class Fourier1DR {
public:
	Fourier1DR(Eigen::Tensor<float, 1, Eigen::RowMajor>* image_, Eigen::Tensor<complex<float>, 1, Eigen::RowMajor>* result_, uint8_t len_) {
		image = image_;
		result = result_;
		len = len_;
	}
	void compute_fft();
private:
	uint8_t len;
	Eigen::Tensor<float, 1, Eigen::RowMajor>* image;
	Eigen::Tensor<complex<float>, 1, Eigen::RowMajor>* result;
};

class Fourier2DR {
public:
	Fourier2DR(Eigen::Tensor<float, 2, Eigen::RowMajor>* image_, Eigen::Tensor<complex<float>, 2, Eigen::RowMajor>* result_, uint8_t len1_, uint8_t len2_) {
		image = image_;
		result = result_;
		len1 = len1_;
		len2 = len2_;
	}
	void compute_fft();
private:
	uint8_t len1, len2;
	Eigen::Tensor<float, 2, Eigen::RowMajor>* image;
	Eigen::Tensor<complex<float>, 2, Eigen::RowMajor>* result;
};

class Fourier3DR {
public:
	Fourier3DR(Eigen::Tensor<float, 3, Eigen::RowMajor>* image_, Eigen::Tensor<complex<float>, 3, Eigen::RowMajor>* result_, uint8_t len1_, uint8_t len2_, uint8_t len3_) {
		image = image_;
		result = result_;
		len1 = len1_;
		len2 = len2_;
		len3 = len3_;
	}
	void compute_fft();
private:
	uint8_t len1, len2, len3;
	Eigen::Tensor<float, 3, Eigen::RowMajor>* image;
	Eigen::Tensor<complex<float>, 3, Eigen::RowMajor>* result;
};