# ==========
# paths 
# ========== 
install_dir=/home/nvidia/opencv_object_detection/install/bin
opencv_path=/home/nvidia/opencv
models_dir=/home/nvidia/models
label_path=$models_dir/labels/imagenet_comp_graph_label_strings.txt 

# ==========
# input 
# ========== 
input_path=$opencv_path/samples/data/fruits.jpg
input_path=$opencv_path/samples/data/vtest.avi
input_path=$opencv_path/samples/data/Megamind.avi

# ========== 
# general config options 
# ========== 
output_time=1
scale_factor=1
model_file=tensorflow_inception_graph.pb

# ==========
# commands with options 
# ==========
cmd="$install_dir/tf_inception -input=$input_path -model=$models_dir/$model_file -t_conf=0.01 -c_names=$label_path -scale_factor=$scale_factor"
cmd_txt=$(eval "echo $cmd")

# ==========
# summary and call command
# ==========
echo "model:   [$model_file]"
echo "scaling: [$scale_factor]"
echo "input:   [$input_path]"
echo "cmd:     [$cmd]"
echo ""
echo "cmd:     [$cmd_txt]"
echo ""
eval $cmd

