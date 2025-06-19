#include <iostream>
#include <resize_function.h>
#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "RgaUtils.h"
#include "im2d.hpp"

using namespace cv;
using namespace std;

// Function to read YUV420 frame
bool readYUV420Frame(const string &filename, Mat &yuvImage, int width, int height) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Could not open the YUV file!" << endl;
        return false;
    }

    int frameSize = width * height * 3 / 2;
    vector<uchar> buffer(frameSize);

    file.read(reinterpret_cast<char *>(&buffer[0]), frameSize);
    if (file.gcount() != frameSize) {
        cerr << "Error: Could not read the complete frame!" << endl;
        return false;
    }

    yuvImage.create(height + height / 2, width, CV_8UC1);
    memcpy(yuvImage.data, buffer.data(), frameSize);

    file.close();
    return true;
}

int colorConvert(void *src, int srcFmt, void *dst, int dstFmt, int width, int height) {
    int ret;
    int src_len = width * height * get_bpp_from_format(srcFmt);
    void *src_ = malloc(src_len + 4096);
    void *org_src = src_;
    memset(src_, 0, src_len + 4096);
    src_ = (void *) ((((int64_t) src_ >> 12) + 1) << 12);
    memcpy(src_, src, src_len);
    int dst_len = width * height * get_bpp_from_format(dstFmt);
    void *dst_ = malloc(dst_len + 4096);
    void *org_dst = dst_;
    memset(dst_, 0, dst_len + 4096);
    dst_ = (void *) ((((int64_t) dst_ >> 12) + 1) << 12);
    rga_buffer_t rga_src = wrapbuffer_virtualaddr((void *) src_, width, height, srcFmt);
    rga_buffer_t rga_dst = wrapbuffer_virtualaddr((void *) dst_, width, height, dstFmt);

    ret = imcvtcolor(rga_src, rga_dst, rga_src.format, rga_dst.format);

    if (IM_STATUS_SUCCESS != ret) {
        printf("colorConvertAndFlip failed. Ret: %s\n", imStrError((IM_STATUS) ret));
    }

    memcpy(dst, dst_, dst_len);
    free(org_src);
    free(org_dst);

    return ret;
}

void processImage(const std::string &imagePath, int thread_num) {
    cv::Mat rgbImage;
    cv::Mat bgrImage = cv::imread(imagePath);

    if (bgrImage.empty()) {
        std::cerr << "Error: Could not read the image.\n";
        return;
    }

    cv::cvtColor(bgrImage, rgbImage, cv::COLOR_BGR2RGB);
    while (true) {
        LETTER_BOX lb;
        lb.in_width = rgbImage.cols;
        lb.in_height = rgbImage.rows;
        lb.channel = 3;
        lb.target_height = 640;
        lb.target_width = 640;
        // resize & unscale
        cv::Mat dst;
        dst.create(lb.target_height, lb.target_width, CV_8UC3);
        compute_letter_box(&lb);
        int ret = rga_letter_box_resize(rgbImage.data, dst.data, &lb);
        if (ret != 0) {
            printf("rga_letter_box_resize failed\n");
        } else {
            std::string msg = "rga_letter_box_resize done ";
            msg += std::to_string(thread_num);
            msg += "\n";
            printf(msg.c_str());
        }
    }

}

void convertYuvImage(int thread_num) {
    Mat yuvImage;
    if (!readYUV420Frame("images/2048x1536.yuv", yuvImage, 2048, 1536)) {
        return;
    }

    while (true) {
        cv::Mat dst;
        dst.create(1536, 2048, CV_8UC3);
        colorConvert(yuvImage.data, RK_FORMAT_YCbCr_420_SP, dst.data, RK_FORMAT_RGB_888, 2048, 1536);
        std::string msg = "colorConvert done ";
        msg += std::to_string(thread_num);
        msg += "\n";
        printf(msg.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    std::vector<std::thread> threads;
    std::string imagePath = "images/cai-fang-HDjVjyVHMbk-unsplash.jpg";
    // Launch threads
    for (int i = 0; i < 3; i++) {
        threads.emplace_back(processImage, imagePath, i);
        threads.emplace_back(convertYuvImage, i);
    }

    // Join threads
    for (auto &thread: threads) {
        thread.join();
    }
    return 0;
}
