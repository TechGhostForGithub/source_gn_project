# -*- coding: utf-8 -*-
import subprocess
import os
import io
import sys
import glob
import shutil
import errno
import argparse
from subprocess import call

def make_dir_exist(path):
    if not os.path.exists(path):
        try:
            os.makedirs(path)
        except OSError as exc: # Guard against race condition
            if exc.errno != errno.EEXIST:
                raise

def cp_files(from_Path, to_path, ext):
    from_Path = os.path.join(from_Path, ext)
    files = glob.iglob(from_Path)
    make_dir_exist(to_path)
    for file in files:
        if os.path.isfile(file):
            shutil.copy2(file, to_path)


def cp_interfaces(from_Path, to_path):
    cp_files(from_Path, to_path, '*.h')

def archive_interfaces(kernel_path, dispatch_path, platform):
    # set dispatch path
    include_path = os.path.join(dispatch_path, 'include')
    modules_path = os.path.join(kernel_path, 'modules')

    # clean out path
    if os.path.isdir(include_path):
        if platform == "win":
            rm_cmd = 'rd /s /q \"{}\"'.format(include_path)
            subprocess.Popen(rm_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True).communicate()
        else:
            shutil.rmtree(include_path)

    # interface
    dest = os.path.join(include_path, 'modules', 'EngineModule', 'interface')
    sour = os.path.join(modules_path, 'EngineModule', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'ClientKernel', 'interface')
    sour = os.path.join(modules_path, 'ClientKernel', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'DataCenter', 'interface')
    sour = os.path.join(modules_path, 'DataCenter', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'FileModule', 'interface')
    sour = os.path.join(modules_path, 'FileModule', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'FileTransfer', 'interface')
    sour = os.path.join(modules_path, 'FileTransfer', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'HttpKernel', 'interface')
    sour = os.path.join(modules_path, 'HttpKernel', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'ImageKernel', 'interface')
    sour = os.path.join(modules_path, 'ImageKernel', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'CommonHead', 'interface')
    sour = os.path.join(modules_path, 'CommonHead', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'MessageKernel', 'interface')
    sour = os.path.join(modules_path, 'MessageKernel', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'UserFace', 'interface')
    sour = os.path.join(modules_path, 'UserFace', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'DataReport', 'interface')
    sour = os.path.join(modules_path, 'DataReport', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'NetworkRouting', 'interface')
    sour = os.path.join(modules_path, 'NetworkRouting', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'H5OfflinePkg', 'interface')
    sour = os.path.join(modules_path, 'H5OfflinePkg', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'modules', 'LogModule')
    sour = os.path.join(modules_path, 'LogModule')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'system_wrappers', 'interface')
    sour = os.path.join(kernel_path, 'system_wrappers', 'interface')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'third_party', 'json')
    sour = os.path.join(kernel_path, 'third_party', 'json', 'include', 'json')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'third_party', 'tinyxml')
    sour = os.path.join(kernel_path, 'third_party', 'tinyxml')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'third_party', 'md5')
    sour = os.path.join(kernel_path, 'third_party', 'md5')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'base', 'sock')
    sour = os.path.join(kernel_path, 'base', 'sock')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'base', 'protocol')
    sour = os.path.join(kernel_path, 'base', 'protocol')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'base', 'http')
    sour = os.path.join(kernel_path, 'base', 'http')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'base', 'asynevent')
    sour = os.path.join(kernel_path, 'base', 'asynevent')
    cp_interfaces(sour, dest)

    dest = os.path.join(include_path, 'common_types.h')
    sour = os.path.join(kernel_path, 'common_types.h')
    shutil.copy2(sour, dest)

    dest = os.path.join(include_path, 'typedefs.h')
    sour = os.path.join(kernel_path, 'typedefs.h')
    shutil.copy2(sour, dest)

    dest = os.path.join(include_path, 'comdefs.h')
    sour = os.path.join(kernel_path, 'comdefs.h')
    shutil.copy2(sour, dest)

def archive_win(kernel_path, build_cache_path, dispatch_path, dispath_cpu):
    dest = os.path.join(dispatch_path)
    sour = os.path.join(build_cache_path)

    if os.path.exists(dest):
        rm_cmd = 'rd /s /q \"{}\"'.format(dest)
        subprocess.Popen(rm_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True).communicate()
    
    make_dir_exist(dest)
    
    cp_files(sour, dest, '*.lib')
    cp_files(sour, dest, '*.dll')
    cp_files(sour, dest, '*.pdb')

def archive_mac(kernel_path, build_cache_path, dispatch_path, dispath_cpu):
    dest = os.path.join(dispatch_path)
    sour = os.path.join(build_cache_path)
    targetlib = os.path.join(dest, "libccore_gn.a")
    
    if os.path.exists(dest):
        shutil.rmtree(dest)
    
    make_dir_exist(dest)

    # find .o files in build_cache dir
    command_find = 'find "{}" -name "*.o"'.format(sour)
    command_find_ret = subprocess.Popen(command_find, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True).communicate()[0]

    # ar .o files to "libccore_gn.a"
    command_ar_files = ""
    for line in command_find_ret.splitlines():
        command_ar_files += (line + ' ')

    command_ar = 'ar -r {} {}'.format(targetlib, command_ar_files)
    command_ar_ret = subprocess.Popen(command_ar, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True).wait()
    if command_ar_ret != 0:
        print("ar failed.")

def archive_ios(kernel_path, build_cache_path, dispatch_path, dispath_cpu):
    dest = os.path.join(dispatch_path)
    sour = os.path.join(build_cache_path)
    targetlib = os.path.join(dest, "libccore_gn.a")

    if os.path.exists(dest):
        shutil.rmtree(dest)
    
    make_dir_exist(dest)

    # find .o files in build_cache dir
    command_find = 'find "{}" -name "*.o"'.format(sour)
    command_find_ret = subprocess.Popen(command_find, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True).communicate()[0]

    # ar .o files to "libccore_gn.a"
    command_ar_files = ""
    for line in command_find_ret.splitlines():
        command_ar_files += (line + ' ')
    
    command_ar = 'ar -r {} {}'.format(targetlib, command_ar_files)
    command_ar_ret = subprocess.Popen(command_ar, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True).wait()
    if command_ar_ret != 0:
        print("ar failed.")

def archive_android(kernel_path, build_cache_path, dispatch_path, dispath_cpu):
    dest = os.path.join(dispatch_path, dispath_cpu)
    sour = os.path.join(build_cache_path)
    targetlib = os.path.join(dest, "libccore_gn.a")

    if os.path.exists(dest):
        shutil.rmtree(dest)
    
    make_dir_exist(dest)

    # find .o files in build_cache dir
    command_find = 'find "{}" -name "*.o"'.format(sour)
    command_find_ret = subprocess.Popen(command_find, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True).communicate()[0]

    # arm-linux-androideabi-ar .o files to "libccore_gn.a"
    command_ar_files = ""
    for line in command_find_ret.splitlines():
        command_ar_files += (line + ' ')
    
    command_ar = 'arm-linux-androideabi-ar -r {} {}'.format(targetlib, command_ar_files)
    command_ar_ret = subprocess.Popen(command_ar, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True, shell=True).wait()
    if command_ar_ret != 0:
        print("arm-linux-androideabi-ar failed.")

    # arm-linux-androideabi-ranlib "libccore_gn.a"
    command_ranlib = 'arm-linux-androideabi-ranlib {}'.format(targetlib)
    commaon_ranlib_ret = subprocess.Popen(command_ranlib, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True).wait()
    if commaon_ranlib_ret != 0:
        print("arm-linux-androideabi-ranlib failed.")

def main():
    parser = argparse.ArgumentParser(description='dispatch for IDE')
    parser.add_argument('--kernel', required=True)
    parser.add_argument('--outpath', required=True)
    parser.add_argument('--buildmode', required=True)
    parser.add_argument('--cachepath', required=True)
    parser.add_argument('--platform', required=True)
    parser.add_argument('--targetcpu', required=True)
    args = parser.parse_args()

    archive_interfaces(args.kernel, args.outpath, args.platform)

    if args.platform == "mac":
        archive_mac(args.kernel, args.cachepath, os.path.join(args.outpath, args.buildmode), args.targetcpu)
    if args.platform == "ios":
        archive_ios(args.kernel, args.cachepath, os.path.join(args.outpath, args.buildmode), args.targetcpu)
    if args.platform == "android":
        archive_android(args.kernel, args.cachepath, os.path.join(args.outpath, args.buildmode), args.targetcpu)
    if args.platform == "win":
        archive_win(args.kernel, args.cachepath, os.path.join(args.outpath, args.buildmode), args.targetcpu)

if __name__ == '__main__':
  sys.exit(main())