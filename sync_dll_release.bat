::Open as admin for release dll synchronisation
pushd %~dp0

call python ./sync_dll.py

popd
pause