#include "Deconvolution.hpp"

// Comes from Deconvolution/calculateSmallOTF.m
void Deconvolution::calculateSmallOTF() {
	Eigen::DSizes<Eigen::DenseIndex, 3> sizePSF = OTF->dimensions();
	Eigen::DSizes<Eigen::DenseIndex, 4> imageSize = image->dimensions();

	//Hold the newest image size here:
	uint16_t newImageSize[3] = { 0, 0, 0 };

	// The saved PSF must be the same size or larger than the images:
	if (sizePSF.at(0) < imageSize.at(0) || sizePSF.at(1) < imageSize.at(1) || sizePSF.at(2) < imageSize.at(2)) {
		cout << "WARNING: The PSF is smaller than the image size in at least one dimension. All images will be cropped" << endl;
		for (uint8_t ind = 0; ind < 3; ind++)
			newImageSize[ind] = min(sizePSF.at(ind), imageSize.at(ind));
	}
	else {
		for (uint8_t ind = 0; ind < 3; ind++)
			newImageSize[ind] = imageSize.at(ind);
	}

	//Find the box of size newImageSize about the PSF origin:
	uint16_t originPSF[3] = { 0, 0, 0 };
	for (uint8_t ind = 0; ind < 3; ind++)
		originPSF[ind] = ceil((sizePSF.at(ind) + 1)/2);

	// PSF = PSF - min(PSF(:))
	float* dat = OTF->data();
	pair<float*,float*> minmax = minmax_element(dat, dat + sizePSF.at(0) * sizePSF.at(1) * sizePSF.at(2));
	*OTF -= OTF->constant(*(minmax.first));
	
	// PSF = PSF ./ max(PSF(:))
	*OTF /= OTF->constant(*(minmax.second));

	/*
	smallPSF = PSF( ...
					(originPSF(1)-ceil((newImageSize(1)-1)/2)):(originPSF(1)+floor((newImageSize(1)-1)/2)), ...
					(originPSF(2)-ceil((newImageSize(2)-1)/2)):(originPSF(2)+floor((newImageSize(2)-1)/2)), ...
					(originPSF(3)-ceil((newImageSize(3)-1)/2)):(originPSF(3)+floor((newImageSize(3)-1)/2))
				);
	*/

	Eigen::array<int, 3> offsets = { (originPSF[0] - ceil((newImageSize[0] - 1) / 2)) - 1, (originPSF[1] - ceil((newImageSize[1] - 1) / 2)) - 1, (originPSF[2] - ceil((newImageSize[2] - 1) / 2)) - 1 };
	Eigen::array<int, 3> extents = { (originPSF[0] + floor((newImageSize[0] - 1) / 2)) - 1, (originPSF[1] + floor((newImageSize[1] - 1) / 2)) - 1, (originPSF[2] + floor((newImageSize[2] - 1) / 2)) - 1 };
	//Eigen::Tensor<float,3> smallPSF = OTF->slice(offsets, extents);

	// smallOTF = fftshift(fftn(smallPSF)); clear smallPSF ;
	// smallPSF.fft();
	

	// smallOTF = abs(smallOTF);
}