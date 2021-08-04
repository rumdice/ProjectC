".\flatc.exe" --csharp -o ./ ./rumdaProtocol.fbs
IF NOT ERRORLEVEL 0 GOTO EXIT
robocopy ".\rumda\Protocol" "..\protocol" /MIR
:EXIT