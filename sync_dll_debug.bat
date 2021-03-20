::Open as admin for debug dll synchronisation
pushd %~dp0

call python ./sync_dll.py debug

popd
pause