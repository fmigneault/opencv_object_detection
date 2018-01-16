base_dir=$(pwd)
ssd_mobilenet_path=$base_dir/install/bin/ssd_mobilenet_object_detection
ssd_mobilenet_eval="$base_dir/../opencv/samples/data/vtest.avi"
model_name=MobileNetSSD_deploy
models_dir=$base_dir/../models
model_proto=$models_dir/$model_name.prototxt
model_caffe=$models_dir/$model_name.caffemodel
scale_factor=1

cmd='$ssd_mobilenet_path -video=$ssd_mobilenet_eval -model=$model_caffe -proto=$model_proto -scale_factor=$scale_factor'
cmd_txt=$(eval "echo $cmd")

echo "path:   [$ssd_mobilenet_path]"
echo "eval:   [$ssd_mobilenet_eval]"
echo "scale:  [$scale_factor]"
echo "model:"
echo "  path:     [$models_dir]"
echo "  name:     [$model_name]"
echo "  caffe:    [$model_caffe]"
echo "  proto:    [$model_proto]"
echo ""
echo "cmd:"
echo "[$cmd_txt]"
echo ""
eval $cmd


