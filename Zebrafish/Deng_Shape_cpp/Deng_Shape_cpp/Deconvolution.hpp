#pragma once
/*
% deconvolve the image
% parffor c = p.chanList
weinerEstimateList = cell(numel(p.chanList), 1);
parfor c = p.chanList
p_tmp = p;
p_tmp.chanList = c;
p_tmp = splitPerChannelParams(p_tmp, c);
switch p_tmp.deconMode
    case 'weiner'
        weinerEstimateList{ c } = weinerDeconvolveFull(MD, PSF, maxOTF, imageBlurWeiner, morphRadiusErode, morphRadiusDilate, imageMultiply, p_tmp, photoDir, outFilePaths{ 1,c });
    case 'richLucy'
        richLucyDeconvolveFull(MD, PSF, 1 / 10, p_tmp, photoDir, outFilePaths{ 1,c });
    otherwise
        error([p_tmp.deconMode ' is not an allowed decon mode']);
    end
end
*/

#include <iostream>
#include <algorithm>
#include <eigen/unsupported/Eigen/CXX11/Tensor>
#include "Slice.hpp"
#include "Fourier.hpp"

using namespace std;

class Deconvolution {
public:
    //PSF is Point Spread Function:
    Deconvolution(Eigen::Tensor<float, 4, Eigen::RowMajor> * image_, Eigen::Tensor<float, 3, Eigen::RowMajor> * PSF_, bool is_lucy_, Eigen::Tensor<float, 3, Eigen::RowMajor>* smallOTF_, Eigen::Tensor<float, 3, Eigen::RowMajor>* apodizeFilter_, uint16_t* newImageSize_) {
        image = image_;
        // Before we calculate the OTF, set the location of OTF to PSF_:
        PSF = PSF_;
        is_lucy = is_lucy_;
        smallOTF = smallOTF_;
        newImageSize = newImageSize_;
        apodizeFilter = apodizeFilter_;
    }

    void calculateSmallOTF();

    void makeApodizationFilter();

    void cropToSmallOTF();

    void weinerDeconvolve();

private:
    // The 4D image we are working with:
    Eigen::Tensor<float, 4, Eigen::RowMajor>* image;

    // OTF is optical transfer function:
    Eigen::Tensor<float, 3, Eigen::RowMajor>* PSF;

    // Apodization Filter:
    Eigen::Tensor<float, 3, Eigen::RowMajor> * apodizeFilter;

    // If filter is not Lucy, use Weiner Deconvolution:
    bool is_lucy;

    Eigen::Tensor<float, 3, Eigen::RowMajor> * smallOTF;
    uint16_t * newImageSize;
};