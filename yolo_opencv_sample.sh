install_dir=/home/nvidia/opencv_dnn_detect/install/bin
input_path=/home/nvidia/opencv/samples/data/vtest.avi
input_path=/home/nvidia/opencv/samples/data/Megamind.avi
label_path=/home/nvidia/models/labels/voc.names
models_dir=/home/nvidia/models
#model=yolo-608x608
#model_input_size=608
#model=yolo-voc
#model_input_size=416
#model=yolo-voc-288x288
#model_input_size=288
model=tiny-yolo-voc
#model_input_size=416

model_input_size=416
scale_factor=1

#cmd="$install_dir/yolo_object_detection -video=0 -cfg=$models_dir/$model.cfg -model=$models_dir/$model.weights -class_names=$label_path -scale_factor=$scale_factor -input_size=$model_input_size"
cmd="$install_dir/yolo_object_detection -source=$input_path -cfg=$models_dir/$model.cfg -model=$models_dir/$model.weights -class_names=$label_path -scale_factor=$scale_factor -input_size=$model_input_size"
cmd_txt=$(eval "echo $cmd")

echo "model:   [$model]"
echo "scaling: [$scale_factor]"
echo "input:   [$input_path]"
echo "cmd:     [$cmd]"
echo ""
echo "cmd:     [$cmd_txt]"
echo ""
eval $cmd
