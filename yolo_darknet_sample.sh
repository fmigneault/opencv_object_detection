darknet_path=/home/nvidia/opencv_dnn_detect/yolo_darknet
#darknet_eval="detector demo"
darknet_eval="detect /home/nvidia/opencv/samples/data/vtest.avi"
darknet_data=$darknet_path/cfg/voc.data
#darknet_data=$darknet_path/cfg/coco.data
#model_name=tiny-yolo-voc
model_name=yolo.2.0
models_dir=/home/nvidia/models
model_cfg=$models_dir/$model_name.cfg
model_weigths=$models_dir/$model_name.weights

#camera_string="nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"
camera_string=-c 1

#cmd=$darknet_path/darknet detect test $darknet_path/cfg/coco.data $models_dir/$model.cfg $models_dir/$model.weights $camera_string

cmd='$darknet_path/darknet $darknet_eval $darknet_data $model_cfg $model_weigths $camera_string'
echo "cmd:    [$cmd]"
echo "path:   [$darknet_path]"
echo "eval:   [$darknet_eval]"
echo "data:   [$darknet_data]"
echo "model:"
echo "  path:     [$models_dir]"
echo "  name:     [$model_name]"
echo "  cfg:      [$model_cfg]"
echo "  weights:  [$model_weigths]"
echo ""
eval $cmd
