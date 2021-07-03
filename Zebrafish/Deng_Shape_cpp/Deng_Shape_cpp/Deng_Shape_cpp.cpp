// Deng_Shape_cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <eigen/Eigen/Dense>
#include <eigen/unsupported/Eigen/CXX11/Tensor>

#include "Fourier.hpp"

extern "C"
{
#include <stdio.h>
#include <tiffio.h>
}

#include "Deconvolution.hpp"

#define IM_WIDTH 512
#define IM_Z_HEIGHT 40

using namespace std;

void printArray(uint16_t* array, uint16_t width)
{
	uint32_t i;
	for (i = 0; i < width; i++)
	{
		cout << array[i];
	}
	cout << endl;
}

int main()
{
	// In the future, this path will be passed into the main argument of the program:
	string path_str = "C:\\Users\\bsterling\\Desktop\\3DBioImaging\\Zebrafish\\Sample_Project\\.data_do_not_delete\\raw\\";
	cout << path_str << endl << endl;


	filesystem::path path = path_str;

	// Read the number of times we are working with here:
	uint16_t num_times = 0;
	try {
		for (auto& p : filesystem::directory_iterator(path)) ++num_times;
	}
	catch (const filesystem::filesystem_error& e) {
		// In case of exception print here (Usually that it cannot find the specified file):
		cout << e.what() << endl;
	}

	if (num_times > 0) {
		int image_width = IM_WIDTH;
		int image_Z = IM_Z_HEIGHT;

		bool time_slot_empty = 0;

		// Need to create dynamic num_z because it depends how many time-steps we have:
		// Read the number of z-heights we are working with here:
		uint16_t* num_z = new uint16_t[num_times];
		//Initialize with zeros:
		memset(num_z, 0, sizeof(uint16_t) * num_times);

		try {
			for (int t = 0; t < num_times; t++) {
				filesystem::path time_path = path_str + to_string(t + 1);
				for (auto& p : filesystem::directory_iterator(time_path)) ++num_z[t];
			}
		}
		catch (const filesystem::filesystem_error& e) {
			// In case of exception print here (Usually that it cannot find the specified file):
			cout << e.what() << endl;
			time_slot_empty = 1;
		}

		if (!time_slot_empty) {
			// Load the first time-slot for now:
			Eigen::Tensor<float, 4, Eigen::RowMajor> input(512, 512, 40, 5);

			for (int t = 0; t < num_times; t++)
				for (int z = 0; z < num_z[0]; z++) {
					//string tif_path = path_str + "1\\1.tif";
					string tif_path = path_str + to_string(t + 1) + "\\" + to_string(z + 1) + ".tif";
					char* tif_path_arr = const_cast<char*>(tif_path.c_str());

					TIFF* tif = TIFFOpen(tif_path_arr, "r");
					if (tif) {
						uint32_t imagelength;
						uint32_t config;
						tdata_t buf;
						uint32_t row;

						TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imagelength);
						TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);
						buf = _TIFFmalloc(TIFFScanlineSize(tif));
						//if (config == PLANARCONFIG_CONTIG) {
						for (row = 0; row < imagelength; row++) {
							TIFFReadScanline(tif, buf, row);
							uint8_t* data = (uint8_t*)malloc(512);
							memset(data, 0, 512);
							memcpy(data, buf, 512);
							//memcpy(img[z][row], buf, 512);
							for (int j = 0; j < 512; j++) {
								//cout << (int)data[j] << " ";
								input(row, j, z, t) = (float)data[j] / 0xFF;
							}
							/*
							if (row == 200) {
								for (int j = 0; j < 512; j++) {
									cout << (int)data[j] << " ";
								}
								cout << "endl" << endl;
							}
							*/
							free(data);
						}
						//}
						/*
						else if (config == PLANARCONFIG_SEPARATE) {
							uint16_t s, nsamples;

							TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &nsamples);
							for (s = 0; s < nsamples; s++)
								for (row = 0; row < imagelength; row++)
									TIFFReadScanline(tif, buf, row, s);
						}
						*/
						_TIFFfree(buf);
						TIFFClose(tif);
					}
				}

			// Start with Deconvolution:
			Eigen::Tensor<float, 3, Eigen::RowMajor> PSF(512, 512, 40);
			Deconvolution deconvolution(&input, &PSF, false);
			deconvolution.calculateSmallOTF();
		}
		else {
			cout << "Error: A time folder is empty" << endl;
			exit(EXIT_FAILURE);
		}
	}
	else {
		cout << "Error: Could not find any time data" << endl;
		exit(EXIT_FAILURE);
	}

	// Save to csv file:
	// const static Eigen::IOFormat::IOFormat CSVFormat(Eigen::__unnamed_enum_025f_2::FullPrecision, Eigen::__unnamed_enum_025f_1::DontAlignCols, ", ", "\n")


	return 0;
}



/* 1D FFT with FFT module:

	// Save to csv file:
	// const static Eigen::IOFormat::IOFormat CSVFormat(Eigen::__unnamed_enum_025f_2::FullPrecision, Eigen::__unnamed_enum_025f_1::DontAlignCols, ", ", "\n")
	#include <chrono>
	#define N 10000000
	Eigen::Tensor<float, 1, Eigen::RowMajor> signal(N);
	Eigen::Tensor<complex<float>, 1, Eigen::RowMajor> fft(N);

	for (int i = 0; i < N; i++) {
		signal(i) = i;
		fft(i) = 0;
	}

	Fourier1DR fourier(&signal, &fft, N);

	auto start = chrono::steady_clock::now();
	fourier.compute_fft();
	auto end = chrono::steady_clock::now();

	//for (uint16_t i = 0; i < N; i++)
		//cout << fft(i) << endl;

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
		<< " sec";

*/

/* 2D Fourier Transform Module Example:
Eigen::Tensor<float, 2, Eigen::RowMajor> m(2, 2);
Eigen::Tensor<complex<float>, 2, Eigen::RowMajor> f(2, 2);

m(0, 0) = 1;
m(0, 1) = 2;
m(1, 0) = 3;
m(1, 1) = 4;

Fourier2DR fourier2(&m, &f, 2, 2);
fourier2.compute_fft();

cout << f << endl;

for (uint8_t i = 0; i < 4; i++) {
	cout << f.data()[i] << endl;
}
*/

/* 3D Fourier Transform Module Example :
Eigen::Tensor<float, 3, Eigen::RowMajor> m(2, 2, 2);
Eigen::Tensor<complex<float>, 3, Eigen::RowMajor> f(2, 2, 2);

m(0, 0, 0) = 1;
m(0, 0, 1) = 2;
m(0, 1, 0) = 3;
m(0, 1, 1) = 4;
m(1, 0, 0) = 5;
m(1, 0, 1) = 6;
m(1, 1, 0) = 7;
m(1, 1, 1) = 8;

Fourier3DR fourier3(&m, &f, 2, 2, 2);
fourier3.compute_fft();

cout << f << endl;

for (uint8_t i = 0; i < 8; i++) {
	cout << f.data()[i] << endl;
}
*/

/*
// Convolution example:
Eigen::Tensor<float, 2, Eigen::RowMajor> input(3, 3);
Eigen::Tensor<float, 2, Eigen::RowMajor> m(2, 2);
Eigen::array<ptrdiff_t, 2> dims({ 0, 1 });

input(0, 0) = 1;
input(1, 0) = 1;
input(2, 0) = 1;
input(0, 1) = 1;
input(1, 1) = 1;
input(2, 1) = 1;
input(0, 2) = 1;
input(1, 2) = 1;
input(2, 2) = 1;

m(0, 0) = 1;
m(1, 0) = 1;
m(0, 1) = 1;
m(1, 1) = 1;

Eigen::array<pair<int, int>, 2> paddings;
paddings[0] = make_pair(1, 1);
paddings[1] = make_pair(1, 1);
Eigen::Tensor<float, 2, Eigen::RowMajor> b = m.pad(paddings);

cout << b << endl;

// Example of 2D convolution! :D
cout << b.convolve(m, dims);
*/

// Take a slice of Eigen::Tensor:
/*
Eigen::array<int, 4> offsets = { 0, 0, 0, 0 };
Eigen::array<int, 4> extents = { 512, 512, 1, 1 };

auto slice = input.slice(offsets, extents);

// C fwrite is MUCH faster than C++ stream:
FILE* fp;
fp = fopen("C:\\Users\\bsterling\\Desktop\\ugh.txt", "w");
fwrite(input.data(), sizeof(float), 52428800, fp);
fclose(fp);
*/