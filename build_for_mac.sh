#!/bin/bash
{
cd $(dirname "$0")
kernel_path=$(pwd)
cd -
} &> /dev/null # disable output

echo ---------------------------------------------------------------
echo 编译参数: 
echo ---------------------------------------------------------------
echo 参数: debug/release
echo \ \ \ \ \ \ 可选参数，编译模式默认是release
echo ---------------------------------------------------------------

echo -
echo -
echo ---------------------------------------------------------------
echo 第1步：检查编译模式debug/release...
echo ---------------------------------------------------------------
debug_mode="false"
if [ $# -le 0 ]; then
    echo "warning: unspecified build mode, now use release."
else
    mode=$(echo $1 | tr '[:upper:]' '[:lower:]')
    if [[ $mode != "release" && $mode != "debug" ]]; then
        echo "error: unkonow build mode -- $1"
        exit 1
    fi
    if [ $mode == "debug" ]; then
        debug_mode="true"
    else
        debug_mode="false"
    fi
fi
echo 编译模式为: $mode


echo -
echo -
echo ---------------------------------------------------------------
echo 第2步：清理编译临时文件...
echo ---------------------------------------------------------------
build_cache_path=$kernel_path/build_cache
rm -rf $build_cache_path


echo -
echo -
echo ---------------------------------------------------------------
echo 第3步：生成ninja编译文件..
echo ---------------------------------------------------------------
gn=$kernel_path/gn/bin/gn
ninja=$kernel_path/gn/bin/ninja
dispatch_path=$kernel_path/out

if [ $debug_mode == "true" ]; then
    dispatch_path=$kernel_path/out/Debug
else
	dispatch_path=$kernel_path/out/Release
fi
log_path=$build_cache_path/build.log

start_time="$(date -u +%s)"


# ninja files
$gn gen $build_cache_path --args="is_debug=$debug_mode target_cpu=\"x64\""
if [ $? != 0 ]; then
    echo "generate ninja failed"
    exit 1
fi


echo -
echo -
echo ---------------------------------------------------------------
echo 第4步：开始ninja编译...
echo ---------------------------------------------------------------
# build
$ninja -C $build_cache_path > $log_path
if [ $? != 0 ]; then
    echo "build failed. log: $log_path"
    exit 1
fi


echo -
echo -
echo ---------------------------------------------------------------
echo 第5步：完成！
echo ---------------------------------------------------------------
end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
echo "Elapsed time: $elapsed seconds" >> $log_path

# finish
echo $dispatch_path >> $log_path
echo "All success. $dispatch_path"

exit 0

