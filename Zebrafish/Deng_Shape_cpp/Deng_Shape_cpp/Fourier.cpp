#include "Fourier.hpp"
#include <iostream>
#include <algorithm>
using namespace std;

// This is written for REAL-VALUED functions in the time domain
void Fourier1DR::compute_fft() {
	double* in = (double*)malloc(sizeof(double) * len);
	fftwf_complex* out = (fftwf_complex*)fftw_malloc(sizeof(fftwf_complex) * (len / 2 + 1));

	// Load data into real part of "in":
	float* data = image->data();
	for (int i = 0; i < len; i++)
	{
		in[i] = (double) data[i];
	}
	
	fftwf_plan p = fftwf_plan_dft_r2c_1d(len, data, out, FFTW_ESTIMATE);
	fftwf_execute(p); // Repeat as needed

	// Imaginary unit:
	complex<float> j(0, 1);

	for (int i = 0; i < len/2 + 1; i++) {
		(*result)(i) = (complex<float>) out[i][0] + (complex<float>) out[i][1] * j;
	}

	fftwf_destroy_plan(p);
	fftwf_free(in);
	fftwf_free(out);
}
/*
void Fourier2DR::compute_fft() {
	fftwf_complex* in = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * len1 * len2);
	fftwf_complex* out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * len1 * len2);

	// Load data into real part of "in": (Data format is already in row-major :D)
	float* data = image->data();
	for (int i = 0; i < len1 * len2; i++) {
		in[i][0] = data[i];
		in[i][1] = 0;
	}

	fftwf_plan p = fftwf_plan_dft_2d(len1, len2, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftwf_execute(p); // Repeat as needed

	// Imaginary unit:
	complex<float> j(0, 1);

	complex<float> * res = (complex<float>*) malloc(sizeof(complex<float>) * len1 * len2);

	for (int i = 0; i < len1 * len2; i++) {
		res[i] = (complex<float>) out[i][0] + (complex<float>) out[i][1] * j;
	}

	memcpy(result->data(), res, len1 * len2 * sizeof(complex<float>));
	//result->data() = res;
	
	free(res);

	fftwf_destroy_plan(p);
	fftwf_free(in);
	fftwf_free(out);
}
*/


void Fourier3D::compute_fft() {
	// Load data into real part of "in": (Data format is already in row-major :D)
	float* data = image->data();
	for (int i = 0; i < len1 * len2 * len3; i++) {
		in[i][0] = data[i];
		in[i][1] = 0;
	}

	fftwf_execute(p); // Repeat as needed

	// Imaginary unit:
	complex<float> j(0, 1);

	complex<float>* res = (complex<float>*) malloc(sizeof(complex<float>) * len1 * len2 * len3);

	for (int i = 0; i < len1 * len2 * len3; i++) {
		res[i] = (complex<float>) out[i][0] + (complex<float>) out[i][1] * j;
	}

	memcpy(result->data(), res, len1 * len2 * len3 * sizeof(complex<float>));
	//result->data() = res;

	free(res);
}

void Fourier3D::compute_abs_fft() {
	// Load data into real part of "in": (Data format is already in row-major :D)
	float* data = image->data();
	//memcpy(in[0], data, len1 * len2 * len3);
	//memset(in[0] + len1 * len2 * len3, 0, len1 * len2 * len3);

	for (int i = 0; i < len1 * len2 * len3; i++) {
		in[i][0] = data[i];
		in[i][1] = 0;
	}
	
	auto start = chrono::steady_clock::now();
	fftwf_execute(p); // Repeat as needed
	auto end = chrono::steady_clock::now();


	cout << "Elapsed time in nanoseconds: "
		<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
		<< " ns" << endl;

	cout << "Elapsed time in microseconds: "
		<< chrono::duration_cast<chrono::microseconds>(end - start).count()
		<< " µs" << endl;

	cout << "Elapsed time in milliseconds: "
		<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
		<< " ms" << endl;

	cout << "Elapsed time in seconds: "
		<< chrono::duration_cast<chrono::seconds>(end - start).count()
		<< " sec" << endl;
	//for (uint16_t i = 0; i < N; i++)
		//cout << fft(i) << endl;


	float* res = (float*) malloc(sizeof(float) * len1 * len2 * len3);

	for (int i = 0; i < len1 * len2 * len3; i++) {
		res[i] = (float) sqrt(pow(out[i][0], 2) + pow(out[i][1], 2));
	}

	memcpy(real_result->data(), res, len1 * len2 * len3 * sizeof(float));
	//result->data() = res;

	free(res);

}

void Fourier3D::fftshift() {
	Eigen::DSizes<Eigen::DenseIndex, 3> dims = result->dimensions();
	//void circshift(ty * out, const ty * in, int xdim, int ydim, int xshift, int yshift)
	// floor(size(x, dim) / 2
	int xdim = dims.at(0);
	int ydim = dims.at(1);
	int zdim = dims.at(2);

	std::complex<float>* out = result->data();
	std::complex<float>* in = (std::complex<float>*)malloc(sizeof(std::complex<float>) * xdim * ydim * zdim);
	memcpy(in, out, sizeof(std::complex<float>) * xdim * ydim * zdim);


	int xshift = floor(xdim / 2);
	int yshift = floor(ydim / 2);
	int zshift = floor(zdim / 2);
	for (int i = 0; i < xdim; i++) {
		int ii = (i + xshift) % xdim;
		if (ii < 0) ii = xdim + ii;
		for (int j = 0; j < ydim; j++) {
			int jj = (j + yshift) % ydim;
			if (jj < 0) jj = ydim + jj;
			for (int k = 0; k < zdim; k++) {
				int kk = (k + zshift) % zdim;
				if (kk < 0) kk = zdim + kk;
				out[ii * ydim * zdim + jj * zdim + kk] = in[i * ydim * zdim + j * zdim + k];
				//out[ii * ydim + jj] = in[i * ydim + j];
			}
		}
	}
	free(in);
}

void Fourier3D::ifftshift() {
	Eigen::DSizes<Eigen::DenseIndex, 3> dims = image->dimensions();
	//void circshift(ty * out, const ty * in, int xdim, int ydim, int xshift, int yshift)
	// floor(size(x, dim) / 2
	int xdim = dims.at(0);
	int ydim = dims.at(1);
	int zdim = dims.at(2);

	float* out = image->data();
	float* in = (float*)malloc(sizeof(float) * xdim * ydim * zdim);
	memcpy(in, out, sizeof(float) * xdim * ydim * zdim);


	int xshift = ceil (xdim / 2);
	int yshift = ceil(ydim / 2);
	int zshift = ceil(zdim / 2);
	for (int i = 0; i < xdim; i++) {
		int ii = (i + xshift) % xdim;
		if (ii < 0) ii = xdim + ii;
		for (int j = 0; j < ydim; j++) {
			int jj = (j + yshift) % ydim;
			if (jj < 0) jj = ydim + jj;
			for (int k = 0; k < zdim; k++) {
				int kk = (k + zshift) % zdim;
				if (kk < 0) kk = zdim + kk;
				out[ii * ydim * zdim + jj * zdim + kk] = in[i * ydim * zdim + j * zdim + k];
				//out[ii * ydim + jj] = in[i * ydim + j];
			}
		}
	}
	free(in);
}

void Fourier3D::abs_fftshift() {
	Eigen::DSizes<Eigen::DenseIndex, 3> dims = real_result->dimensions();
	//void circshift(ty * out, const ty * in, int xdim, int ydim, int xshift, int yshift)
	// floor(size(x, dim) / 2
	int xdim = dims.at(0);
	int ydim = dims.at(1);
	int zdim = dims.at(2);

	float* out = real_result->data();
	float* in = (float*)malloc(sizeof(float) * xdim * ydim * zdim);
	memcpy(in, out, sizeof(float) * xdim * ydim * zdim);


	int xshift = floor(xdim / 2);
	int yshift = floor(ydim / 2);
	int zshift = floor(zdim / 2);
	for (int i = 0; i < xdim; i++) {
		int ii = (i + xshift) % xdim;
		if (ii < 0) ii = xdim + ii;
		for (int j = 0; j < ydim; j++) {
			int jj = (j + yshift) % ydim;
			if (jj < 0) jj = ydim + jj;
			for (int k = 0; k < zdim; k++) {
				int kk = (k + zshift) % zdim;
				if (kk < 0) kk = zdim + kk;
				out[ii * ydim * zdim + jj * zdim + kk] = in[i * ydim * zdim + j * zdim + k];
				//out[ii * ydim + jj] = in[i * ydim + j];
			}
		}
	}
	free(in);
}
