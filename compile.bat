@ECHO off

REM Set SDK path
SET SDK_PATH=C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6
SET PATH=%SDK_PATH%\bin;%PATH%

REM Navigate to the project directory
CD C:\Users\User\Documents\GitHub\fx-CG50-Stuff

REM Run the Prizm SDK build process with Prizm's make (from SDK)
%SDK_PATH%\bin\make
PAUSE
