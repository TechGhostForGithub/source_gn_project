import os
import sys
import winreg
import subprocess

if sys.version_info >= (3,):
    def subprocess_encoding():
        if sys.version_info >= (3, 6):
            return "oem"
        # Prior to Python 3.6, the "oem" encoding is not available.
        # However, as long as the user hasn't redirected the stream,
        # standard IO streams are opened using the OEM encoding.
        # We take the stderr encoding, as that's presumed to be the
        # least likely for the user to have redirected.
        return sys.__stderr__.encoding

    def console_to_str(s):
        return s.decode(subprocess_encoding(), errors="replace")
else:
    def console_to_str(s):
        return s


def get_vs_path(vs_ver):
    key = r'SOFTWARE\Microsoft\VisualStudio'

    if vs_ver == "2015":
        # VS 2015
        key += r'\14.0'
    elif vs_ver == "2013":
        # VS 2013
        key += r'\12.0'
    else:
        # VS 2015
        key += r'\14.0'
    
    value = __get_value(key, 'ShellFolder')

    # current user _Config
    if len(value) < 1:
        key += r'_Config'        
        value = __get_value_current_user(key, 'ShellFolder')

    if len(value) < 1:
        print("注册表中未找到%s, 请手动指定或重新安装:(" % (vs_ver))
        sys.exit(1)

    return value

def get_win_sdk_path():
    key = r'SOFTWARE\Microsoft\Microsoft SDKs\Windows\v10.0'

    value = __get_value(key, 'InstallationFolder')
    if len(value) < 1:
        print("注册表中未找到win10 SDK, 请重新安装")
        sys.exit(1)

    return value

def __get_value(key, valuename):
    try:
        with winreg.ConnectRegistry(None, winreg.HKEY_LOCAL_MACHINE) as reg:
            with winreg.OpenKey(reg, key, 0, winreg.KEY_READ) as key:
                return winreg.QueryValueEx(key, valuename)[0]
    except PermissionError as ex:
        print("%s" % (ex), file=sys.stderr)
        print('由于UAC的限制，请用管理员模式运行此脚本')
    except FileNotFoundError as ex:
        print("%s" % (ex), file=sys.stderr)
    
    return ""

def __get_value_current_user(key, valuename):
    try:
        with winreg.ConnectRegistry(None, winreg.HKEY_CURRENT_USER) as reg:
            with winreg.OpenKey(reg, key, 0, winreg.KEY_READ) as key:
                return winreg.QueryValueEx(key, valuename)[0]
    except PermissionError as ex:
        print("%s" % (ex), file=sys.stderr)
        print('由于UAC的限制，请用管理员模式运行此脚本')
    except FileNotFoundError as ex:
        print("%s" % (ex), file=sys.stderr)
    
    return ""    

def set_gn_env(gn_folder):
    need_set_env = False
    paths = os.environ['Path'].split(';')
    if 'GN' in os.environ:
        if os.environ['GN'] != gn_folder:
            paths.remove(os.environ['GN'])
            need_set_env = True
    else:
        if gn_folder in paths:
            paths.remove(gn_folder)
        need_set_env = True

    if need_set_env:
        paths.append(str(gn_folder))
        setx_cmd = 'setx /m GN \"{}\"'.format(gn_folder)
        proc = subprocess.run(setx_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=False)
        stderr = console_to_str(proc.stderr)
        if len(stderr) > 0:
            print(stderr + '- 环境变量初始化失败，首次请以管理员模式运行.bat文件\n')
            sys.exit(1)

        setx_cmd = 'setx /m PATH \"{}\"'.format(';'.join(paths))
        proc = subprocess.run(setx_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=False)
        stderr = console_to_str(proc.stderr)
        if len(stderr) > 0:
            print(stderr + '- 环境变量初始化失败，首次请以管理员模式运行.bat文件\n')
            sys.exit(1)