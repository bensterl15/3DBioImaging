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
	Fourier1DR(Eigen::Tensor<float, 1, Eigen::RowMajor>* image_, Eigen::Tensor<complex<float>, 1, Eigen::RowMajor>* result_, uint64_t len_) {
		image = image_;
		result = result_;
		len = len_;
	}
	void compute_fft();
private:
	uint64_t len;
	Eigen::Tensor<float, 1, Eigen::RowMajor>* image;
	Eigen::Tensor<complex<float>, 1, Eigen::RowMajor>* result;
};

class Fourier2DR {
public:
	Fourier2DR(Eigen::Tensor<float, 2, Eigen::RowMajor>* image_, Eigen::Tensor<complex<float>, 2, Eigen::RowMajor>* result_, uint16_t len1_, uint16_t len2_) {
		image = image_;
		result = result_;
		len1 = len1_;
		len2 = len2_;
	}
	//void compute_fft();
private:
	uint16_t len1, len2;
	Eigen::Tensor<float, 2, Eigen::RowMajor>* image;
	Eigen::Tensor<complex<float>, 2, Eigen::RowMajor>* result;
};

class Fourier3D {
public:
	Fourier3D(Eigen::Tensor<float, 3, Eigen::RowMajor>* image_, Eigen::Tensor<complex<float>, 3, Eigen::RowMajor>* result_, uint16_t len1_, uint16_t len2_, uint16_t len3_) {
		image = image_;
		result = result_;
		len1 = len1_;
		len2 = len2_;
		len3 = len3_;

		in = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * len1 * len2 * len3);
		out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * len1 * len2 * len3);

		p = fftwf_plan_dft_3d(len1, len2, len3, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	}

	Fourier3D(Eigen::Tensor<float, 3, Eigen::RowMajor>* image_, Eigen::Tensor<float, 3, Eigen::RowMajor>* result_, uint16_t len1_, uint16_t len2_, uint16_t len3_) {
		image = image_;
		real_result = result_;
		len1 = len1_;
		len2 = len2_;
		len3 = len3_;

		in = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * len1 * len2 * len3);
		out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * len1 * len2 * len3);

		p = fftwf_plan_dft_3d(len1, len2, len3, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	}

	~Fourier3D() {
		fftwf_destroy_plan(p);
		fftwf_free(in);
		fftwf_free(out);
	}

	void compute_fft();
	void compute_abs_fft();
	void fftshift();
	void ifftshift();
	void abs_fftshift();
private:
	uint16_t len1, len2, len3;
	Eigen::Tensor<float, 3, Eigen::RowMajor>* image;
	Eigen::Tensor<complex<float>, 3, Eigen::RowMajor>* result;
	Eigen::Tensor<float, 3, Eigen::RowMajor>* real_result;
	fftwf_plan p;
	fftwf_complex* in;
	fftwf_complex* out;
};
