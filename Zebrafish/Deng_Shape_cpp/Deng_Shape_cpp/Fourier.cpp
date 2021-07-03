#include "Fourier.hpp"

void Fourier1DR::compute_fft() {
	fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * len);
	fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * len);

	// Load data into real part of "in":
	float* data = image->data();
	for (uint16_t i = 0; i < len; i++) {
		in[i][0] = data[i];
		in[i][1] = 0;
	}

	fftw_plan p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p); // Repeat as needed

	// Imaginary unit:
	complex<float> j(0, 1);

	for (uint32_t i = 0; i < len; i++) {
		(*result)(i) = (complex<float>) out[i][0] + (complex<float>) out[i][1] * j;
	}

	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);
}

void Fourier2DR::compute_fft() {
	fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * len1 * len2);
	fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * len1 * len2);

	// Load data into real part of "in": (Data format is already in row-major :D)
	float* data = image->data();
	for (uint16_t i = 0; i < len1 * len2; i++) {
		in[i][0] = data[i];
		in[i][1] = 0;
	}

	fftw_plan p = fftw_plan_dft_2d(len1, len2, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p); // Repeat as needed

	// Imaginary unit:
	complex<float> j(0, 1);

	complex<float> * res = (complex<float>*) malloc(sizeof(complex<float>) * len1 * len2);

	for (uint32_t i = 0; i < len1 * len2; i++) {
		res[i] = (complex<float>) out[i][0] + (complex<float>) out[i][1] * j;
	}

	memcpy(result->data(), res, len1 * len2 * sizeof(complex<float>));
	//result->data() = res;
	
	free(res);

	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);
}

void Fourier3DR::compute_fft() {
	fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * len1 * len2 * len3);
	fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * len1 * len2 * len3);

	// Load data into real part of "in": (Data format is already in row-major :D)
	float* data = image->data();
	for (uint16_t i = 0; i < len1 * len2 * len3; i++) {
		in[i][0] = data[i];
		in[i][1] = 0;
	}

	fftw_plan p = fftw_plan_dft_3d(len1, len2, len3, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p); // Repeat as needed

	// Imaginary unit:
	complex<float> j(0, 1);

	complex<float>* res = (complex<float>*) malloc(sizeof(complex<float>) * len1 * len2 * len3);

	for (uint32_t i = 0; i < len1 * len2 * len3; i++) {
		res[i] = (complex<float>) out[i][0] + (complex<float>) out[i][1] * j;
	}

	memcpy(result->data(), res, len1 * len2 * len3 * sizeof(complex<float>));
	//result->data() = res;

	free(res);

	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);
}

/* FFTW 1D Example:
#define N 4

in[0][1] = 0;
in[1][1] = 0;
in[2][1] = 0;
in[3][1] = 0;

in[0][0] = 1;
in[1][0] = 2;
in[2][0] = 3;
in[3][0] = 4;

*/