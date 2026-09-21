@echo off
setlocal enabledelayedexpansion

rem Run UnipusHelperPro with a Java 25+ runtime when available.

set "JAR_NAME=UnipusHelperPro-1.0.3.jar"
set "SCRIPT_DIR=%~dp0"
set "JAR_PATH=%SCRIPT_DIR%%JAR_NAME%"
set "BACKGROUND=1"

if "%~1"=="--foreground" (
  set "BACKGROUND=0"
  shift
)

if not exist "%JAR_PATH%" (
  echo 未找到文件: %JAR_PATH%
  exit /b 1
)

set "SELECTED_JAVA="
set "DEFAULT_JAVA="

for /f "usebackq delims=" %%j in (`where java 2^>nul`) do (
  set "DEFAULT_JAVA=%%j"
  goto :after_default_search
)
:after_default_search

if defined DEFAULT_JAVA (
  call :java_major "%DEFAULT_JAVA%" DEFAULT_MAJOR
  if defined DEFAULT_MAJOR (
    if %DEFAULT_MAJOR% GEQ 25 (
      set "SELECTED_JAVA=%DEFAULT_JAVA%"
      echo 默认 Java 满足要求 (version %DEFAULT_MAJOR%).
    )
  )
)

if not defined SELECTED_JAVA (
  call :collect_javas
  for /L %%i in (1,1,%JAVA_COUNT%) do (
    call set "CANDIDATE=%%JAVA_BIN_%%i%%"
    if defined CANDIDATE (
      call :java_major "%%CANDIDATE%%" MAJOR_TMP
      if defined MAJOR_TMP (
        if !MAJOR_TMP! GEQ 25 (
          set "SELECTED_JAVA=%%CANDIDATE%%"
          echo 在 PATH 中找到符合要求的 Java: %%CANDIDATE%% (version !MAJOR_TMP!).
          goto :after_candidate_search
        )
      )
    )
  )
:after_candidate_search
)

if not defined SELECTED_JAVA (
  echo 未在 PATH 中找到 25 及以上版本的 Java。
  if %JAVA_COUNT% EQU 0 (
    echo 当前 PATH 中未发现任何 Java，可输入自定义路径。
  )
  call :prompt_for_java
)

if not defined SELECTED_JAVA (
  echo 未选择 Java，退出。
  exit /b 1
)

if %BACKGROUND% EQU 1 (
  start "" /min "%SELECTED_JAVA%" -jar "%JAR_PATH%" >nul 2>&1
  echo 已使用 %SELECTED_JAVA% 在后台启动。
) else (
  "%SELECTED_JAVA%" -jar "%JAR_PATH%"
)
exit /b 0

:collect_javas
set "JAVA_COUNT=0"
for /f "usebackq delims=" %%j in (`where java 2^>nul`) do (
  set /a JAVA_COUNT+=1
  set "JAVA_BIN_!JAVA_COUNT!=%%j"
)
exit /b 0

:prompt_for_java
set "CHOICE="
:prompt_loop
echo 请选择要使用的 Java:
if %JAVA_COUNT% GTR 0 (
  for /L %%i in (1,1,%JAVA_COUNT%) do (
    call set "BIN=%%JAVA_BIN_%%i%%"
    call :java_major "%%BIN%%" MAJOR_TMP
    if not defined MAJOR_TMP set "MAJOR_TMP=unknown"
    echo %%i) %%BIN%% ^(version: !MAJOR_TMP!^)
  )
)
set /a CUSTOM_INDEX=%JAVA_COUNT%+1
echo %CUSTOM_INDEX%) 自定义 Java 路径
set /p "CHOICE=输入序号: "
for /f "delims=0123456789" %%x in ("%CHOICE%") do set "CHOICE="
if not defined CHOICE goto :prompt_loop
if %CHOICE% LSS 1 goto :prompt_loop
if %CHOICE% EQU %CUSTOM_INDEX% (
  set "CUSTOM_PATH="
  set /p "CUSTOM_PATH=请输入 Java 可执行文件的完整路径: "
  if not exist "%CUSTOM_PATH%" (
    echo 路径无效: %CUSTOM_PATH%
    goto :prompt_loop
  )
  set "SELECTED_JAVA=%CUSTOM_PATH%"
  goto :eof
)
if %CHOICE% GTR %JAVA_COUNT% goto :prompt_loop
call set "SELECTED_JAVA=%%JAVA_BIN_%CHOICE%%%"
goto :eof

:java_major
set "JAVA_BIN=%~1"
set "OUT_VAR=%~2"
set "%OUT_VAR%="
for /f "usebackq delims=" %%v in (`powershell -NoProfile -Command "$ln=& '%JAVA_BIN%' -version 2>&1 | Select-String -Pattern 'version \"([0-9]+)(?:\.([0-9]+))?' -First 1; if($ln){$m=$ln.Matches[0].Groups; $maj=$m[1].Value; $min=$m[2].Value; if($maj -eq '1' -and $min){$maj=$min}; Write-Output $maj}"`) do (
  set "%OUT_VAR%=%%v"
)
exit /b 0
