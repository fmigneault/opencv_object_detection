start_dir=$(pwd)
model_dir=$start_dir/../models

yolo_dir=$start_dir/yolo_darknet
yolo_model_name=tiny-yolo-voc
yolo_label_file=$yolo_dir/cfg/voc.data
yolo_model_file=$yolo_dir/cfg/$yolo_model_name.cfg
yolo_weights=$model_dir/$yolo_model_name.weights

jetson_camera_stream='"nvcamerasrc ! video/x-raw(memory:NVMM) ! nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"'

cmd="$yolo_dir/darknet detector demo $yolo_label_file $yolo_model_file $yolo_weights $jetson_camera_stream"
cmd_txt=$(eval "echo $cmd")

echo "cmd:    [$cmd]"
echo "path:   [$yolo_dir]"
echo "eval:   [jetson camera stream]"
echo "model:"
echo "  path:     [$models_dir]"
echo "  name:     [$yolo_model_name]"
echo "  cfg:      [$yolo_model_file]"
echo "  weights:  [$yolo_weights]"
echo "  labels:   [$yolo_label_file]"
echo ""
echo "Changing directory [$yolo_dir]"
cd $yolo_dir
echo "Running sample..."
eval $cmd

