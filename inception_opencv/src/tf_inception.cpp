// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2016, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.

/*
Sample of using OpenCV dnn module with Tensorflow Inception model.
*/

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace cv::dnn;

#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

const String keys =
        "{help h    || Sample app for loading Inception TensorFlow model. "
                       "The model and class names list can be downloaded here: "
                       "https://storage.googleapis.com/download.tensorflow.org/models/inception5h.zip }"
        "{model m   |tensorflow_inception_graph.pb| path to TensorFlow .pb model file }"
        "{input i   || path to image or video file }"
        "{cam_dev   | 0 | camera device index }"
        "{i_blob    | input | input blob name) }"
        "{o_blob    | softmax2 | output blob name) }"
        "{c_names c | imagenet_comp_graph_label_strings.txt | path to file with classnames for class id }"
        "{t_conf t  | 0.2 | confidence threshold of detections }"
        "{result r  || path to save output blob (optional, binary format, NCHW order) }"
        ;

const size_t inSize = 224;
const float inScaleFactor = 0.007843f;
const float meanVal = 117.0;        

void getMaxClass(const Mat &probBlob, int *classId, double *classProb);
std::vector<String> readClassNames(const char *filename);

int main(int argc, char **argv)
{
    cv::CommandLineParser parser(argc, argv, keys);

    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    String modelFile = parser.get<String>("model");
    String inputFile = parser.get<String>("input");
    String inBlobName = parser.get<String>("i_blob");
    String outBlobName = parser.get<String>("o_blob");
    float confidenceThreshold = parser.get<float>("t_conf");

    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    String classNamesFile = parser.get<String>("c_names");
    String resultFile = parser.get<String>("result");

    //! [Initialize network]
    dnn::Net net = readNetFromTensorflow(modelFile);
    //! [Initialize network]

    if (net.empty())
    {
        std::cerr << "Can't load network by using the mode file: " << std::endl;
        std::cerr << modelFile << std::endl;
        exit(-1);
    }
    
    std::vector<String> classNames;
    if (!classNamesFile.empty())
        classNames = readClassNames(classNamesFile.c_str());
    else
    {
        std::cerr << "Missing class names file" << std::endl;
        exit(-1);
    }
    if (classNames.size() < 1)
    {
        std::cerr << "Could not read class names from file: '" << classNamesFile << "'" << std::endl;
        exit(-1);
    }
    
    VideoCapture cap;
    bool is_camera = false;
    if (parser.get<String>("input").empty())
    {
        int cameraDevice = parser.get<int>("camera_device");
        cap = VideoCapture(cameraDevice);
        if(!cap.isOpened())
        {
            cout << "Couldn't find camera: " << cameraDevice << endl;
            return -1;
        }
        is_camera = true;
    }
    else
    {
        String input = parser.get<String>("input");
        cap.open(input);
        if(!cap.isOpened())
        {
            cout << "Couldn't open image or video: " << input << endl;
            return -1;
        }
    }   
    
    String display_title = "Inception: Detections";
    namedWindow(display_title, WINDOW_NORMAL | WINDOW_FREERATIO | CV_GUI_EXPANDED);
    resizeWindow(display_title, 800, 600); 

    size_t frame_count = 0;
    cv::TickMeter tm;
    Mat result, frame, image;
    ostringstream ss_label;
    for (;;) 
    {        
        //! [Prepare blob]
        cap >> frame;
        
        if (frame.empty())
        {
            waitKey(1);
            break;
        }
        
        if (frame.empty())
        {
            if (is_camera)
                std::cerr << "Can't read frame from camera device" << std::endl;
            else
                std::cerr << "Can't read frame from file: " << inputFile << std::endl;
            exit(-1);
        }
        frame_count++;
        
        //! [Prepare blob]
        if (frame.channels() == 4)
            cvtColor(frame, frame, COLOR_BGRA2BGR);
        
        //Convert frame to batch of images
        Mat inputBlob = blobFromImage(frame, inScaleFactor, Size(inSize, inSize), 
                                      Scalar(meanVal, meanVal, meanVal), true, false); 
        //! [Prepare blob]
        
        //! [Set input blob]
        net.setInput(inputBlob, inBlobName);    //set the network input
        //! [Set input blob]
        
        tm.start();

        //! [Make forward pass]
        result = net.forward(outBlobName);  //compute output
        //! [Make forward pass]

        tm.stop();
        ostringstream ss;
        vector<double> layersTimings;
        double freq = getTickFrequency() / 1000;
        double time = net.getPerfProfile(layersTimings) / freq;
        ss << "FPS: " << 1000/time << " ; time: " << time << " ms";        
        putText(frame, ss.str(), Point(20,20), 0, 0.5, Scalar(0,0,255));
        if (frame_count == 1) 
        {
            ss_label << ss.str() << " ; label: ";        
            image = frame;
        }
        
        Mat detections(result.size[2], result.size[3], CV_32F, result.ptr<float>());                
        std::cout << "===" << std::endl 
            << result.size() << std::endl
            << result.size << std::endl 
            << detections.size() << std::endl 
            << detections.rows << std::endl 
            << detections.at<float>(0, 3) * frame.cols << ", " 
            << detections.at<float>(0, 4) * frame.rows << ", " 
            << detections.at<float>(0, 5) * frame.cols << ", " 
            << detections.at<float>(0, 6) * frame.rows << ", " << std::endl;
        
        for(int i = 0; i < detections.rows; i++)
        {
            float confidence = detections.at<float>(i, 2);
            if(confidence > confidenceThreshold)
            {
                int xLeftBottom = static_cast<int>(detections.at<float>(i, 3) * frame.cols);
                int yLeftBottom = static_cast<int>(detections.at<float>(i, 4) * frame.rows);
                int xRightTop = static_cast<int>(detections.at<float>(i, 5) * frame.cols);
                int yRightTop = static_cast<int>(detections.at<float>(i, 6) * frame.rows);
                
                Rect object((int)xLeftBottom, (int)yLeftBottom,
                            (int)(xRightTop - xLeftBottom),
                            (int)(yRightTop - yLeftBottom));
                rectangle(frame, object, Scalar(0, 255, 0));
                
                std::cout << " OBJ: " << object << " | ";
                
                   
                ss.str("");
                ss << confidence;
                String conf(ss.str());                
                int classId;
                double classProb;
                getMaxClass(result, &classId, &classProb);//find the best class
                String label = classNames.at(classId);
                int baseLine = 0;
                Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
                rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
                                      Size(labelSize.width, labelSize.height + baseLine)),
                          Scalar(255, 255, 255), FILLED);
                putText(frame, label, Point(xLeftBottom, yLeftBottom),
                        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
            }                        
        }
        
        imshow(display_title, frame);
        if (waitKey(1) >= 0) break;
    }
    
    if (frame_count == 1)
    {
        if (!resultFile.empty()) 
        {
            CV_Assert(result.isContinuous());

            ofstream fout(resultFile.c_str(), ios::out | ios::binary);
            fout.write((char*)result.data, result.total() * sizeof(float));
            fout.close();
        }

        std::cout << "Output blob shape " 
                  << result.size[0] << " x " 
                  << result.size[1] << " x " 
                  << result.size[2] << " x " 
                  << result.size[3] << std::endl;
        std::cout << "Inference time, ms: " << tm.getTimeMilli() << std::endl;

        int classId;
        double classProb;
        getMaxClass(result, &classId, &classProb);//find the best class
        String label = classNames.at(classId);
        ss_label << label;
        putText(image, ss_label.str(), Point(20,20), 0, 0.5, Scalar(0,0,255));
        imshow(display_title, image);
        
        //! [Print results]
        std::cout << "Best class: #" << classId << " '" << label << "'" << std::endl;
        std::cout << "Probability: " << classProb * 100 << "%" << std::endl;
        
        waitKey();
    }
        
    return 0;
} //main


/* Find best class for the blob (i. e. class with maximal probability) */
void getMaxClass(const Mat &probBlob, int *classId, double *classProb)
{
    Mat probMat = probBlob.reshape(1, 1); //reshape the blob to 1x1000 matrix
    Point classNumber;

    minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
    *classId = classNumber.x;
}

std::vector<String> readClassNames(const char *filename)
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
