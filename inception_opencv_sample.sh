# ==========
# paths 
# ========== 
install_dir=/home/nvidia/opencv_object_detection/install/bin
opencv_path=/home/nvidia/opencv
input_path=$opencv_path/samples/data/fruits.jpg
#input_path=$opencv_path/samples/data/vtest.avi
#input_path=$opencv_path/samples/data/Megamind.avi
models_dir=/home/nvidia/models
label_path=$models_dir/labels/imagenet_comp_graph_label_strings.txt 

# ========== 
# general config options 
# ========== 
output_time=1
scale_factor=1
model_file=tensorflow_inception_graph.pb

# ==========
# commands with options 
# ==========
#cmd="$install_dir/yolo_object_detection -video=0 -cfg=$models_dir/$model.cfg -model=$models_dir/$model.weights -class_names=$label_path -scale_factor=$scale_factor -input_size=$model_input_size"
cmd="$install_dir/tf_inception 
    -image=$input_path -model=$models_dir/$model_file 
    -c_names=$label_path -scale_factor=$scale_factor"
cmd_txt=$(eval "echo $cmd")

# ==========
# summary and call command
# ==========
echo "model:   [$model]"
echo "scaling: [$scale_factor]"
echo "input:   [$input_path]"
echo "cmd:     [$cmd]"
echo ""
echo "cmd:     [$cmd_txt]"
echo ""
eval $cmd

