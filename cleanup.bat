echo 'Cleaning'
powershell.exe -NoExit -ExecutionPolicy -Bypass -Command "ls -Recurse -Include Build, Intermediate, Binaries, Saved, .vs -Exclude Saved/Config/Windows | rmdir -Force -Recurse"