#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;
using namespace cv;
using namespace cv::dnn;

const size_t inWidth = 300;
const size_t inHeight = 300;
const float meanVal = 127.5;

const char* params
    = "{ help           | false                                     | print usage         }"
      "{ config         | ssd_inception_v2_coco_2017_11_17.pbtxt    | model configuration }"
      "{ weight         | frozen_inference_graph.pb                 | model weights }"
      "{ labels         | coco.names                                | class name labels }"
      "{ camera_device  | 0                                         | camera device number }"
      "{ video          |                                           | video or image for detection}"
      "{ out            |                                           | path to output video file}"
      "{ min_confidence | 0.2                                       | min confidence      }"
      "{ opencl         | false                                     | enable OpenCL }"
      "{ scale_factor   | 1.0                                       | pre-scaling of video/images }"
;

std::vector<String> readClassNames(String filename);

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, params);
    parser.about("This sample uses Inception-v2 SSD.\n"
                 ".caffemodel model's file is available here: "
                 "https://github.com/opencv/opencv/wiki/TensorFlow-Object-Detection-API\n"
                 "Default network is 300x300 on COCO dataset.\n");

    if (parser.get<bool>("help") || argc == 1)
    {
        parser.printMessage();
        return 0;
    }

    String modelConfiguration = parser.get<String>("config");
    String modelWeights = parser.get<String>("weight");
    String modelLabels = parser.get<String>("labels");
    CV_Assert(!modelConfiguration.empty() && !modelWeights.empty() && !modelLabels.empty());

    std::vector<String> classNames = readClassNames(modelLabels);
    CV_Assert(classNames.size() > 0);

    //! [Initialize network]
    dnn::Net net = readNetFromTensorflow(modelConfiguration, modelWeights);
    //! [Initialize network]

    if (parser.get<bool>("opencl"))
    {
        net.setPreferableTarget(DNN_TARGET_OPENCL);
    }

    if (net.empty())
    {
        cerr << "Can't load network by using the following files: " << endl;
        cerr << "pbtxt: " << modelConfiguration << endl;
        cerr << "pb:    " << modelWeights << endl;
        cerr << "Models can be downloaded here:" << endl;
        cerr << "https://github.com/opencv/opencv/wiki/TensorFlow-Object-Detection-API" << endl;
        exit(-1);
    }
    
    double scale_factor = parser.get<double>("scale_factor");
    String display_title = "MobileNet-SSD: Detections";
    namedWindow(display_title, WINDOW_NORMAL | WINDOW_FREERATIO | CV_GUI_EXPANDED);
    resizeWindow(display_title, 800, 600);

    VideoCapture cap;
    if (!parser.has("video"))
    {
        int cameraDevice = parser.get<int>("camera_device");
        cap = VideoCapture(cameraDevice);
        if(!cap.isOpened())
        {
            cout << "Couldn't find camera: " << cameraDevice << endl;
            return -1;
        }
    }
    else
    {
        cap.open(parser.get<String>("video"));
        if(!cap.isOpened())
        {
            cout << "Couldn't open image or video: " << parser.get<String>("video") << endl;
            return -1;
        }
    }

    //Acquire input size
    Size inVideoSize((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),
                     (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));

    double fps = cap.get(CV_CAP_PROP_FPS);
    int fourcc = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));
    VideoWriter outputVideo;
    if (!parser.get<String>("out").empty()) {
        outputVideo.open(parser.get<String>("out") ,
                         (fourcc != 0 ? fourcc : VideoWriter::fourcc('M','J','P','G')),
                         (fps != 0 ? fps : 10.0), inVideoSize, true);
    }
    
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera/video or read image

        if (frame.empty())
        {
            waitKey();
            break;
        }

        if (frame.channels() == 4)
            cvtColor(frame, frame, COLOR_BGRA2BGR);
        
        resize(frame, frame, Size(), scale_factor, scale_factor, cv::INTER_NEAREST);
        
        //! [Prepare blob]
        Mat inputBlob = blobFromImage(frame, 1.0f / meanVal,
                                      Size(inWidth, inHeight),
                                      Scalar(meanVal, meanVal, meanVal),
                                      true, false); //Convert Mat to batch of images
        //! [Prepare blob]

        //! [Set input blob]
        net.setInput(inputBlob); //set the network input
        //! [Set input blob]

        //! [Make forward pass]
        Mat detection = net.forward(); //compute output
        //! [Make forward pass]

        vector<double> layersTimings;
        double freq = getTickFrequency() / 1000;
        double time = net.getPerfProfile(layersTimings) / freq;

        Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

        if (!outputVideo.isOpened())
        {
            putText(frame, format("FPS: %.2f ; time: %.2f ms", 1000.f/time, time),
                    Point(20,20), 0, 0.5, Scalar(0,0,255));
        }
        else
            cout << "Inference time, ms: " << time << endl;

        float confidenceThreshold = parser.get<float>("min_confidence");
        for(int i = 0; i < detectionMat.rows; i++)
        {
            float confidence = detectionMat.at<float>(i, 2);

            if(confidence > confidenceThreshold)
            {
                size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));

                int left = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int top = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int right = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int bottom = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

                rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 255, 0));
                String label = format("%s: %.2f", classNames[objectClass], confidence);
                int baseLine = 0;
                Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
                top = max(top, labelSize.height);
                rectangle(frame, Point(left, top - labelSize.height),
                          Point(left + labelSize.width, top + baseLine),
                          Scalar(255, 255, 255), CV_FILLED);
                putText(frame, label, Point(left, top),
                        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
            }
        }

        if (outputVideo.isOpened())
            outputVideo << frame;

        imshow(display_title, frame);
        if (waitKey(1) >= 0) break;
    }

    return 0;
} // main



std::vector<String> readClassNames(String filename)
{
    std::vector<String> classNames;

    std::ifstream fp(filename);
    if (!fp.is_open())
    {
        std::cerr << "File with classes labels not found: " << filename << std::endl;
        exit(-1);
    }

    std::string name;
    while (!fp.eof())
    {
        std::getline(fp, name);
        if (name.length())
            classNames.push_back( name );
    }

    fp.close();
    return classNames;
}
