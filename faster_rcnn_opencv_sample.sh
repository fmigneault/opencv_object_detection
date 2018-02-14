base_dir=$(pwd)
frcnn_path=$base_dir/install/bin/faster_rcnn_object_detection
frcnn_eval="$base_dir/../opencv/samples/data/vtest.avi"
model_name=MobileNetfrcnn_deploy
models_dir=$base_dir/../models
model_proto=$models_dir/$model_name.prototxt
model_caffe=$models_dir/$model_name.caffemodel
scale_factor=1

cmd='$frcnn_path -video=$frcnn_eval -model=$model_caffe -proto=$model_proto -scale_factor=$scale_factor'
cmd_txt=$(eval "echo $cmd")

echo "path:   [$frcnn_path]"
echo "eval:   [$frcnn_eval]"
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


