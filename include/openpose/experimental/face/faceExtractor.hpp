#ifndef OPENPOSE__FACE__FACE_EXTRACTOR_HPP
#define OPENPOSE__FACE__FACE_EXTRACTOR_HPP

#include <array>
#include <atomic>
#include <memory> // std::shared_ptr
#include <thread>
#include <opencv2/core/core.hpp>
#include <openpose/core/array.hpp>
#include <openpose/core/net.hpp>
#include <openpose/core/nmsCaffe.hpp>
#include <openpose/core/resizeAndMergeCaffe.hpp>
#include <openpose/pose/enumClasses.hpp>
#include "enumClasses.hpp"

namespace op
{
    namespace experimental
    {
        class FaceExtractor
        {
        public:
            explicit FaceExtractor(const std::string& modelFolder, const int gpuId, const PoseModel poseModel);

            void initializationOnThread();

            void forwardPass(const Array<float>& poseKeyPoints, const cv::Mat& cvInputData);

            Array<float> getFaceKeyPoints() const;

            double get(const FaceProperty property) const;

            void set(const FaceProperty property, const double value);

            void increase(const FaceProperty property, const double value);

        private:
            const cv::Size mNetOutputSize;
            const cv::Size mOutputSize;
            const unsigned int mNeck;
            const unsigned int mNose;
            const unsigned int mLEar;
            const unsigned int mREar;
            const unsigned int mLEye;
            const unsigned int mREye;
            std::array<std::atomic<double>, (int)FaceProperty::Size> mProperties;
            std::shared_ptr<Net> spNet;
            std::shared_ptr<ResizeAndMergeCaffe<float>> spResizeAndMergeCaffe;
            std::shared_ptr<NmsCaffe<float>> spNmsCaffe;
            Array<float> mFaceImageCrop;
            Array<float> mFaceKeyPoints;
            float mScaleFace;
            // Init with thread
            boost::shared_ptr<caffe::Blob<float>> spCaffeNetOutputBlob;
            std::shared_ptr<caffe::Blob<float>> spHeatMapsBlob;
            std::shared_ptr<caffe::Blob<float>> spPeaksBlob;
            std::thread::id mThreadId;

            void checkThread() const;

            DELETE_COPY(FaceExtractor);
        };
    }
}

#endif // OPENPOSE__FACE__FACE_EXTRACTOR_HPP
