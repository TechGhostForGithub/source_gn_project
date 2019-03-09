# -*- coding: utf-8 -*-
import subprocess
import os
import io
import sys
import glob
import shutil
import errno
import winreg
import buildtools

debug_mode = "false"
vs_ver = '2015'
build_libcurl = "false"
support_xp = "false"

print("--------------------------------------------------------------")
print("build parameters:") 
print("--------------------------------------------------------------")
print("  param1: debug/release")
print("          build mode, optionnal, default value is \"release\"")
print("--------------------------------------------------------------")

# debug mode and xp flag
if len(sys.argv) < 2:
    print("warning: need mode for build. now use release")
else:
    if sys.argv[1].lower() != "release" and sys.argv[1].lower() != "debug":
        print("error: unkonow build mode -- {}".format(sys.argv[1]))
        sys.exit(1)
		
    debug_mode = "true" if sys.argv[1].lower() == "debug" else "false"

    # vs version
    if len(sys.argv) > 2 and sys.argv[2].lower() == "xp":
        support_xp = "true"
        vs_ver = "2013"


if debug_mode == "true":
    print("build mode: debug")
else:
    print("build mode: release")

# libcurl
for i in range(len(sys.argv)):
    if sys.argv[i].lower() != "libcurl":
        continue
    build_libcurl = "true"

# kernel path
kernel_path = os.path.dirname(os.path.realpath(__file__))

# --------------------------------------------------------------------------------
# 1.init environment
# --------------------------------------------------------------------------------
#   1) init environment path
#   2) delete build_cache dir
#      mark: don't use python command to delete dir, because of python bug
#   3) create new build_cache dir
# --------------------------------------------------------------------------------
print("init environment ...")

os.chdir(kernel_path)
gn = os.path.join(kernel_path, 'gn', 'bin', 'gn.exe')
ninja = os.path.join(kernel_path, 'gn', 'bin', 'ninja.exe')
build_cache_path = os.path.join(kernel_path, 'build_cache')
log_path = os.path.join(build_cache_path, 'build.log')

# VS path
win_vc = os.path.join(buildtools.get_vs_path(vs_ver), 'VC')

# win 10 sdk
# win_sdk = os.path.dirname(buildtools.get_win_sdk_path())

# clean build_cache
if os.path.isdir(build_cache_path):
    rm_cmd = 'rd /s /q \"{}\"'.format(build_cache_path)
    subprocess.Popen(rm_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True).communicate()
    #shutil.rmtree(build_cache_path)

os.makedirs(os.path.join(build_cache_path, 'libs'))

# --------------------------------------------------------------------------------
# 2. build kernel
# --------------------------------------------------------------------------------
#   1) generate ninja file for build
#   2) build
#   3) print end log
# --------------------------------------------------------------------------------
print("build kernel ...")
gn_cmd = '\"{}\" gen \"{}\" --args=\"is_debug={} target_cpu=\\\"x86\\\" win_vc=\\\"{}\\\" win_vc_ver=\\\"{}\\\" win_xp={}\"'.format(gn, build_cache_path, debug_mode, win_vc, vs_ver, support_xp)

proc = subprocess.run(gn_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=False)
stdout = buildtools.console_to_str(proc.stdout)
stderr = buildtools.console_to_str(proc.stderr)
if 'Done.' not in stdout:
    print(stdout)
    print(stderr)
    with open(os.path.join(build_cache_path, 'build.log'), 'w') as log:
    	log.write(stdout)
    sys.exit(1)

ninja_cmd = '\"{}\" -C \"{}\"'.format(ninja, build_cache_path)
proc = subprocess.run(ninja_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=False)
stdout = buildtools.console_to_str(proc.stdout)
stderr = buildtools.console_to_str(proc.stderr)
if proc.returncode != 0:
    print(stdout)
    print(stderr)
    with open(os.path.join(build_cache_path, 'build.log'), 'w') as log:
    	log.write(stdout)
    sys.exit(1)

with open(os.path.join(build_cache_path, 'build.log'), 'w') as log:
    log.write(stdout)

# archive
# set dispatch path
if debug_mode == "false":
	dispatch_path = os.path.join(kernel_path, 'out', 'Release')
else:
	dispatch_path = os.path.join(kernel_path, 'out', 'Debug')
include_path = os.path.join(kernel_path, 'out', 'include')

print('All done. {}'.format(dispatch_path))
sys.exit(0)
