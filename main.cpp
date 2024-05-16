#include <iostream>
#include <resize_function.h>
#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>

void processImage(const std::string& imagePath, int thread_num) {
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
        std::this_thread::sleep_for (std::chrono::milliseconds (10));
    }

}

int main() {
    std::vector<std::thread> threads;
    std::string imagePath = "images/cai-fang-HDjVjyVHMbk-unsplash.jpg";
    // Launch threads
    for (int i = 0; i <5; i++) {
        threads.emplace_back(processImage, imagePath, i);
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }
    return 0;
}
