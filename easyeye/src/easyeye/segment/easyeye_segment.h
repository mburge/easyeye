/*
 * easyeye_segment.h
 *
 *  Created on: Jul 11, 2013
 *      Author: mchaberski
 */

#ifndef EASYEYE_SEGMENT_H_
#define EASYEYE_SEGMENT_H_

#include <vector>
#include "../common/easyeye_imaging.h"
#include "../common/easyeye_serial.h"
#include "../common/easyeye_utils.h"
#include "../common/easyeye_types.h"
#include <Masek.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "../common/easyeye_diagnostics.h"
#include "../common/easyeye_config.h"

namespace easyeye
{

class Segmentation
{
public:
	Segmentation();
	virtual ~Segmentation();
    Segmentation(const Segmentation& other);
    typedef Result::ResultType Status;
    Status status;
	BoundaryPair boundary_pair;
    std::vector<cv::Point2i> iris_boundary;
    std::vector<cv::Point2i> pupil_boundary;
    cv::SparseMat extrema_noise;
    void Describe(std::ostream& out) const;
    bool Equals(const Segmentation& other) const;
    bool Equals(const Segmentation& other, int radius_delta) const;
    const EyelidsLocation& eyelids_location() const;
    void set_eyelids_location(EyelidsLocation* eyelids_location);
    bool IsEyelidsLocationPresent() const;
private:
    EyelidsLocation* eyelids_location_;
};

class SegmenterConfig : public Config
{
public:
    SegmenterConfig();
    PupilFinderConfig pupil_finder_config;
    IrisFinderConfig iris_finder_config;
    EyelidFinderConfig eyelid_finder_config;
    ExtremaNoiseFinderConfig extrema_noise_finder_config;
};

class Segmenter : public DiagnosticsCreator
{
public:
	Segmenter();
    void SegmentEyeImage(cv::Mat& eyeImg, Segmentation& seg);
	const static int EYE_IMAGE_WIDTH_SCALE_THRESHOLD = 600;
	const static int EYE_IMAGE_HEIGHT_SCALE_THRESHOLD = 440;
protected:
	SegmenterConfig config_;
};

namespace serial
{

class SegmentationAdapter : public serial::Adapter 
{
    public:
        bool FromJson(const Json::Value& src, void* dst);
        void ToJson(void* src, Json::Value& dst);
};

std::string Serialize(const Segmentation& data);
bool Deserialize(const std::string& json, Segmentation& data);

}

class Contours
{
public:
    static cv::RotatedRect fitEllipse(std::vector<cv::Point2i>& points);
    static cv::RotatedRect fitEllipse(std::vector<cv::Point2i>::iterator points_begin, std::vector<cv::Point2i>::iterator points_end);
    static void AddAll(std::vector<cv::Point>& from, std::vector<cv::Point>& to);
private:
    Contours();
};


}


#endif /* EASYEYE_SEGMENT_H_ */
