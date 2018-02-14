// Brief Sample of using OpenCV dnn module in real time with device capture, video and image.
// VIDEO DEMO: https://www.youtube.com/watch?v=NHtRlndE2cg

#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::dnn;

static const char* about =
"This sample uses You only look once (YOLO)-Detector (https://arxiv.org/abs/1612.08242) to detect objects on camera/video/image.\n"
"Models can be downloaded here: https://pjreddie.com/darknet/yolo/\n"
"Default network is 416x416.\n"
"Class names can be downloaded here: https://github.com/pjreddie/darknet/tree/master/data\n";

static const char* params =
"{ help           | false | print usage         }"
"{ cfg            |       | model configuration }"
"{ model          |       | model weights       }"
"{ input_size     | 416   | model input size    }"
"{ camera_device  | 0     | camera device number}"
"{ source         |       | video or image for detection}"
"{ save           |       | file name output}"
"{ fps            | 3     | frame per second }"
"{ style          | box   | box or line style draw }"
"{ min_confidence | 0.24  | min confidence      }"
"{ class_names    |       | File with class names, [PATH-TO-DARKNET]/data/coco.names }"
"{ scale_factor   | 1.0   | pre-scaling of video/images }"
"{ time           | 1     | output operation timings }";

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, params);

    if (parser.get<bool>("help"))
    {
        cout << about << endl;
        parser.printMessage();
        return 0;
    }
    
    //! [Initialize network]
    String modelConfiguration = parser.get<String>("cfg");
    String modelBinary = parser.get<String>("model");
    dnn::Net net = readNetFromDarknet(modelConfiguration, modelBinary);
    
    int model_input_dim = parser.get<int>("input_size");
    if (model_input_dim <= 0) {
        cerr << "Invalid input size: " << model_input_dim << endl;
        exit(-1);
    }
    Size model_input_size(model_input_dim, model_input_dim);

    if (net.empty())
    {
        cerr << "Can't load network by using the following files: " << endl;
        cerr << "cfg-file:     " << modelConfiguration << endl;
        cerr << "weights-file: " << modelBinary << endl;
        cerr << "Models can be downloaded here:" << endl;
        cerr << "https://pjreddie.com/darknet/yolo/" << endl;
        exit(-1);
    }

    VideoCapture cap;
    VideoWriter writer;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');
    double fps = parser.get<float>("fps");
    if (parser.get<String>("source").empty())
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
        cap.open(parser.get<String>("source"));
        if(!cap.isOpened())
        {
            cout << "Couldn't open image or video: " << parser.get<String>("video") << endl;
            return -1;
        }
    }

    if(!parser.get<String>("save").empty())
    {
        writer.open(parser.get<String>("save"), codec, fps, Size((int)cap.get(CAP_PROP_FRAME_WIDTH),(int)cap.get(CAP_PROP_FRAME_HEIGHT)), 1);
    }

    vector<String> classNamesVec;
    ifstream classNamesFile(parser.get<String>("class_names").c_str());
    if (classNamesFile.is_open())
    {
        string className = "";
        while (std::getline(classNamesFile, className))
            classNamesVec.push_back(className);
    }

    String object_roi_style = parser.get<String>("style");    
    double scale_factor = parser.get<double>("scale_factor");
    String display_title = "YOLO: Detections";
    namedWindow(display_title, WINDOW_NORMAL | WINDOW_FREERATIO | CV_GUI_EXPANDED);
    resizeWindow(display_title, 800, 600);

    Mat frame, frame_resize;
    int64 t0, t1, t2, t3, t4;
    int output_time = parser.get<int>("time");
    
    for(;;)
    {    
        if (output_time)
            t0 = getTickCount();
            
        cap >> frame; // get a new frame from camera/video or read image

        if (frame.empty())
        {
            waitKey();
            break;
        }

        if (frame.channels() == 4)
            cvtColor(frame, frame, COLOR_BGRA2BGR);
            
        resize(frame, frame, Size(), scale_factor, scale_factor, cv::INTER_NEAREST);

        // [Prepare blob] - Convert Mat to batch of images + [Set input blob] - set the network input
        Mat inputBlob = blobFromImage(frame, 1 / 255.F, model_input_size, Scalar(), true, false);
        net.setInput(inputBlob, "data");
        if (output_time)
            t1 = getTickCount();

        // [Make forward pass] - compute output
        Mat detectionMat = net.forward("detection_out");
        if (output_time)
            t2 = getTickCount();

        vector<double> layersTimings;
        double tick_freq = getTickFrequency();
        double time_ms = net.getPerfProfile(layersTimings) / tick_freq * 1000;
        
        int frame_w = frame.size().width;
        int frame_h = frame.size().height;
        putText(frame, format("FPS: %.2f ; time: %.2f ms ; %ix%i", 1000.f / time_ms, time_ms, frame_w, frame_h),
                Point(20, 20), 0, 0.5, Scalar(0, 0, 255));
        
        if (output_time)
            t3 = getTickCount();
            
        float confidenceThreshold = parser.get<float>("min_confidence");
        for (int i = 0; i < detectionMat.rows; i++)
        {
            const int probability_index = 5;
            const int probability_size = detectionMat.cols - probability_index;
            float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);

            size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;
            float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);

            if (confidence > confidenceThreshold)
            {
                float x_center = detectionMat.at<float>(i, 0) * frame.cols;
                float y_center = detectionMat.at<float>(i, 1) * frame.rows;
                float width = detectionMat.at<float>(i, 2) * frame.cols;
                float height = detectionMat.at<float>(i, 3) * frame.rows;
                Point p1(cvRound(x_center - width / 2), cvRound(y_center - height / 2));
                Point p2(cvRound(x_center + width / 2), cvRound(y_center + height / 2));
                Rect object(p1, p2);

                Scalar object_roi_color(0, 255, 0);

                if (object_roi_style == "box")
                {
                    rectangle(frame, object, object_roi_color);
                }
                else
                {
                    Point p_center(cvRound(x_center), cvRound(y_center));
                    line(frame, object.tl(), p_center, object_roi_color, 1);
                }

                String className = objectClass < classNamesVec.size() 
                                 ? classNamesVec[objectClass] 
                                 : cv::format("unknown(%d)", objectClass);
                String label = format("%s: %.2f", className.c_str(), confidence);
                int baseLine = 0;
                Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
                rectangle(frame, Rect(p1, Size(labelSize.width, labelSize.height + baseLine)),
                          object_roi_color, CV_FILLED);
                putText(frame, label, p1 + Point(0, labelSize.height),
                        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
            }
        }
        
        if (output_time) 
        {            
            t4 = getTickCount();
            cout << "===timing=== (ms)" << endl;
            cout << "[0-1]: " << (t1 - t0) / tick_freq * 1000 << endl;
            cout << "[1-2]: " << (t2 - t1) / tick_freq * 1000 << endl;
            cout << "[2-3]: " << (t3 - t2) / tick_freq * 1000 << endl;
            cout << "[3-4]: " << (t4 - t3) / tick_freq * 1000 << endl;
        }
        
        if(writer.isOpened())
            writer.write(frame);

        imshow(display_title, frame);
        if (waitKey(1) >= 0) break;
    }

    return 0;
} // main
