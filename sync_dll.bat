::Open as admin
pushd %~dp0

call python ./sync_dll.py

popd
pause