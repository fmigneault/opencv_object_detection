# ==========
# paths 
# ========== 
install_dir=/home/nvidia/opencv_object_detection/install/bin
opencv_path=/home/nvidia/opencv
input_path=$opencv_path/samples/data/vtest.avi
#input_path=$opencv_path/samples/data/Megamind.avi
models_dir=/home/nvidia/models
label_path=$models_dir/labels/voc.names

# ========== 
# general config options 
# ========== 
output_time=1
scale_factor=1
#model=yolo-608x608
#model_input_size=608
#model=yolo-voc
#model_input_size=416
#model=yolo-voc-288x288
#model_input_size=288
model=tiny-yolo-voc
model_input_size=416

# ==========
# configs overwrites: 
# ==========
#   3 FPS outdoor scene, but missing bboxes
model=tiny-yolo-voc
model_input_size=416
input_path=$opencv_path/samples/data/vtest.avi
#   0.75 FPS outdoor scene, better bboxes
model=yolo-voc-288x288
model_input_size=416
input_path=$opencv_path/samples/data/vtest.avi

# ==========
# commands with options 
# ==========
#cmd="$install_dir/yolo_object_detection -video=0 -cfg=$models_dir/$model.cfg -model=$models_dir/$model.weights -class_names=$label_path -scale_factor=$scale_factor -input_size=$model_input_size"
cmd="$install_dir/yolo_object_detection 
    -source=$input_path -cfg=$models_dir/$model.cfg -model=$models_dir/$model.weights -time=$output_time
    -class_names=$label_path -scale_factor=$scale_factor -input_size=$model_input_size"
cmd_txt=$(eval "echo $cmd")

# ==========
# summary and call command
# ==========
echo "model:   [$model]"
echo "scaling: [$scale_factor]"
echo "timging: [$output_time]"
echo "input:   [$input_path]"
echo "cmd:     [$cmd]"
echo ""
echo "cmd:     [$cmd_txt]"
echo ""
eval $cmd

