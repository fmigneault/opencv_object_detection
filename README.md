# opencv_object_detection
OpenCV samples for CNN object detection

## Installation

Clone the repository and use the provided `make_all.sh` script to compile C++ codes.  
The script will install compiled samples in `./install` of the repository clone directory.  

```
git clone https://github.com/fmigneault/opencv_object_detection
cd opencv_object_detection
sh make_all.sh
```

Then, launch the desired sample with the corresponding `sh` file.

*__Notes__*  
* Some paths might require editing in the `sh` files to specify input image or model location on your machine.
* Model files of various CNN should be acquired on their respective original source.

## Samples

* **faster_rcnn_opencv_sample** [TODO]  


* **inception_opencv_sample**  
Loads TensorFlow Inception model with OpenCV importer and recognizes an object from an input image.

* **jetson_tegra_gstreamer_camera_sample**  
Create a stream to read the Jetson Tegra on-board camera and display it with an OpenCV window.

* **ssd_inception_opencv_sample** [TODO]  


* **ssd_mobilenet_jtgc_sample**  
Uses the input stream of the Jetson Tegra on-board camera (see *jetson_tegra_gstreamer_camera_sample*) and passes it to the SSD-MobileNet object detector (see *ssd_mobilenet_opencv_sample*).

* **ssd_mobilenet_opencv_sample**  
Loads Caffe SSD-MobileNet model with OpenCV importer and obtains object region proposals from an input image or video.

* **tiny_yolo_darknet_stream_sample**  
Runs the original [Darknet Tiny YOLO](https://github.com/pjreddie/darknet.git) model for object region proposals from the Jetson Tegra on-board camera stream.

* **yolo_darknet_sample** [TODO]  


* **yolo_opencv_sample**  
Loads any of the YOLO models with OpenCV importer and obtains region proposals from an input image or video.

