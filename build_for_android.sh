#!/bin/bash

echo ---------------------------------------------------------------
echo 第1步：检查编译参数[Debug/Release] [x86/arm64]
echo ---------------------------------------------------------------

debug_mode="false"
mode=$(echo $1 | tr '[:upper:]' '[:lower:]')
if [[ $mode != "release" && $mode != "debug" ]]; then
    echo "error: unkonow build mode -- $1"
    exit 1
fi
if [ $1 == "debug" ]; then
    debug_mode="true"
else
    debug_mode="false"
fi


if [ $debug_mode == "true" ]; then
    echo 当前编译版本为Debug版本
else
    echo 当前编译版本为Release版本
fi

target_config="x86"
if [ $2 == "x86" ]; then
    target_config="x86"
fi
if [ $2 == "arm64" ]; then
    target_config="arm64"
fi
if [ $2 == "arm" ]; then
    target_config="arm"
fi
echo 当前编译CPU指令集为$target_config

echo -
echo -
echo ---------------------------------------------------------------
echo 第2步：设置依赖库路径和输出目录
echo ---------------------------------------------------------------

{
cd $(dirname "$0")
kernel_path=$(pwd)
cd -
} &> /dev/null # disable output

ndk="/Library/android-ndk-r16b"
PATH=$PATH:$ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin
echo ndk路径：$ndk

#qt_sdk="/Users/taoshu/Qt5.9.6/5.9.6"
#echo qt路径：$qt_sdk

gn=$kernel_path/gn/bin/gn
ninja=$kernel_path/gn/bin/ninja
build_cache_path=$kernel_path/build_cache
dispatch_path=$kernel_path/out
if [ $debug_mode == "true" ]; then
    dispatch_path=$kernel_path/out/Debug/$target_config
else
    dispatch_path=$kernel_path/out/Release/$target_config
fi
log_path=$build_cache_path/build.log
start_time="$(date -u +%s)"


echo -
echo -
echo ---------------------------------------------------------------
echo 第4步：清理过期的编译临时文件
echo ---------------------------------------------------------------
rm -rf $build_cache_path


echo -
echo -
echo ---------------------------------------------------------------
echo 第5步：使用gn生成ninja文件
echo ---------------------------------------------------------------

# ninja file
$gn gen $build_cache_path --args=" \
            target_os = \"android\" \
            target_cpu = \"$target_config\" \
            use_qt_for_android = false \
            ndk = \"$ndk\" \
            qt_sdk = \"$qt_sdk\""
if [ $? != 0 ]; then
    echo "generate ninja failed"
    exit 1
fi


echo -
echo -
echo ---------------------------------------------------------------
echo 第6步：使用ninja开始编译...
echo ---------------------------------------------------------------

# build
$ninja -C $build_cache_path > $log_path
if [ $? != 0 ]; then
    exit 1
fi

end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
echo "Elapsed time: $elapsed seconds" >> $log_path


echo -
echo -
echo ---------------------------------------------------------------
echo 第7步：完成！
echo ---------------------------------------------------------------
# finish
echo -
echo -
echo $dispatch_path >> $log_path
echo "All success. $dispatch_path"

exit 0
