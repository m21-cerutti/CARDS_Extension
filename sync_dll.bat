::Open as admin
pushd %~dp0

call python ./syncdll.py

popd
pause