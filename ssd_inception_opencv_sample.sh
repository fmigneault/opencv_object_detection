# ==========
# config options
# ==========
base_dir=$(pwd)
ssd_inception_path=$base_dir/install/bin/ssd_inception_object_detection
ssd_inception_eval="$base_dir/../opencv/samples/data/vtest.avi"
model_name="Inception-v2-SSD"
models_dir=$base_dir/../models
model_config=$models_dir/ssd_inception_v2_coco_2017_11_17.pbtxt
model_weight=$models_dir/ssd_inception_v2_coco_2017_11_17/frozen_inference_graph.pb
labels_dir=$base_dir/../models/labels
labels_file=$labels_dir/coco.names
scale_factor=1

# ==========
# command
# ==========
cmd='$ssd_inception_path 
    -video=$ssd_inception_eval -config=$model_config -weight=$model_weight 
    -labels=$labels_file -scale_factor=$scale_factor'
cmd_txt=$(eval "echo $cmd")

# ==========
# summary and call
# ==========
echo "path:   [$ssd_inception_path]"
echo "eval:   [$ssd_inception_eval]"
echo "scale:  [$scale_factor]"
echo "model:"
echo "  path:     [$models_dir]"
echo "  name:     [$model_name]"
echo "  config:   [$model_config]"
echo "  weight:   [$model_weight]"
echo ""
echo "cmd:"
echo "[$cmd_txt]"
echo ""
eval $cmd

