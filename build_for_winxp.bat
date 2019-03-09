@echo off

if "%1"=="" (
    echo "warning: need mode for build. now use release"
    python build_for_win.py "release" "xp"
) else (
    python build_for_win.py "%1" "xp" "%2"
)
if not %errorlevel% == 0 (
    pause
    exit 1
)

@echo on