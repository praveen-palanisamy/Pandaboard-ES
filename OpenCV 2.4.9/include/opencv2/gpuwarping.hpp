/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_GPUWARPING_HPP__
#define __OPENCV_GPUWARPING_HPP__

#ifndef __cplusplus
#  error gpuwarping.hpp header must be compiled as C++
#endif

#include "opencv2/core/gpu.hpp"
#include "opencv2/imgproc.hpp"

namespace cv { namespace gpu {

//! DST[x,y] = SRC[xmap[x,y],ymap[x,y]]
//! supports only CV_32FC1 map type
CV_EXPORTS void remap(const GpuMat& src, GpuMat& dst, const GpuMat& xmap, const GpuMat& ymap,
                      int interpolation, int borderMode = BORDER_CONSTANT, Scalar borderValue = Scalar(),
                      Stream& stream = Stream::Null());

//! resizes the image
//! Supports INTER_NEAREST, INTER_LINEAR, INTER_CUBIC, INTER_AREA
CV_EXPORTS void resize(const GpuMat& src, GpuMat& dst, Size dsize, double fx=0, double fy=0, int interpolation = INTER_LINEAR, Stream& stream = Stream::Null());

//! warps the image using affine transformation
//! Supports INTER_NEAREST, INTER_LINEAR, INTER_CUBIC
CV_EXPORTS void warpAffine(const GpuMat& src, GpuMat& dst, const Mat& M, Size dsize, int flags = INTER_LINEAR,
    int borderMode = BORDER_CONSTANT, Scalar borderValue = Scalar(), Stream& stream = Stream::Null());

CV_EXPORTS void buildWarpAffineMaps(const Mat& M, bool inverse, Size dsize, GpuMat& xmap, GpuMat& ymap, Stream& stream = Stream::Null());

//! warps the image using perspective transformation
//! Supports INTER_NEAREST, INTER_LINEAR, INTER_CUBIC
CV_EXPORTS void warpPerspective(const GpuMat& src, GpuMat& dst, const Mat& M, Size dsize, int flags = INTER_LINEAR,
    int borderMode = BORDER_CONSTANT, Scalar borderValue = Scalar(), Stream& stream = Stream::Null());

CV_EXPORTS void buildWarpPerspectiveMaps(const Mat& M, bool inverse, Size dsize, GpuMat& xmap, GpuMat& ymap, Stream& stream = Stream::Null());

//! builds plane warping maps
CV_EXPORTS void buildWarpPlaneMaps(Size src_size, Rect dst_roi, const Mat &K, const Mat& R, const Mat &T, float scale,
                                   GpuMat& map_x, GpuMat& map_y, Stream& stream = Stream::Null());

//! builds cylindrical warping maps
CV_EXPORTS void buildWarpCylindricalMaps(Size src_size, Rect dst_roi, const Mat &K, const Mat& R, float scale,
                                         GpuMat& map_x, GpuMat& map_y, Stream& stream = Stream::Null());

//! builds spherical warping maps
CV_EXPORTS void buildWarpSphericalMaps(Size src_size, Rect dst_roi, const Mat &K, const Mat& R, float scale,
                                       GpuMat& map_x, GpuMat& map_y, Stream& stream = Stream::Null());

//! rotates an image around the origin (0,0) and then shifts it
//! supports INTER_NEAREST, INTER_LINEAR, INTER_CUBIC
//! supports 1, 3 or 4 channels images with CV_8U, CV_16U or CV_32F depth
CV_EXPORTS void rotate(const GpuMat& src, GpuMat& dst, Size dsize, double angle, double xShift = 0, double yShift = 0,
                       int interpolation = INTER_LINEAR, Stream& stream = Stream::Null());

//! smoothes the source image and downsamples it
CV_EXPORTS void pyrDown(const GpuMat& src, GpuMat& dst, Stream& stream = Stream::Null());

//! upsamples the source image and then smoothes it
CV_EXPORTS void pyrUp(const GpuMat& src, GpuMat& dst, Stream& stream = Stream::Null());

class CV_EXPORTS ImagePyramid
{
public:
    inline ImagePyramid() : nLayers_(0) {}
    inline ImagePyramid(const GpuMat& img, int nLayers, Stream& stream = Stream::Null())
    {
        build(img, nLayers, stream);
    }

    void build(const GpuMat& img, int nLayers, Stream& stream = Stream::Null());

    void getLayer(GpuMat& outImg, Size outRoi, Stream& stream = Stream::Null()) const;

    inline void release()
    {
        layer0_.release();
        pyramid_.clear();
        nLayers_ = 0;
    }

private:
    GpuMat layer0_;
    std::vector<GpuMat> pyramid_;
    int nLayers_;
};

}} // namespace cv { namespace gpu {

#endif /* __OPENCV_GPUWARPING_HPP__ */
