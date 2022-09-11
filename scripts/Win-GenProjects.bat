pushd %~dp0\..\
call vendor\pre\premake\premake5.exe vs2019
popd
PAUSE