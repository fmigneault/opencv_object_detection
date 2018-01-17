cur_dir=$(pwd)
bld_dir=bld
cmake_bld_type=Release
cmake_install=$cur_dir/install
programs="jetson_tegra_gstreamer_camera faster_rcnn_opencv ssd_mobilenet_opencv ssd_opencv yolo_opencv"

for prog in $programs
do
    cur_bld=$cur_dir/$prog/$bld_dir
    mkdir -p $cur_bld
    cd $cur_bld
    cmake .. -DCMAKE_BUILD_TYPE=$cmake_bld_type -DCMAKE_INSTALL_PREFIX=$cmake_install
    make -j 4
    make install
done

