@echo off
rem ============================================================
rem  Run every test script and save the console output to
rem  test\*.out  (run build.bat first).
rem ============================================================
setlocal
cd /d "%~dp0"

if not exist build\CampusActivity.exe (
    echo build\CampusActivity.exe not found. Run build.bat first.
    exit /b 1
)

for %%t in (T1_lecture_rule T2_competition_rule T3_polymorphic_collection T4_regression robustness) do (
    echo === Running %%t ===
    build\CampusActivity.exe < test\%%t.in > test\%%t.out 2>&1
    if errorlevel 1 (
        echo %%t FAILED
    ) else (
        echo wrote test\%%t.out
    )
)

echo.
echo All tests finished. Results are in test\*.out
endlocal
