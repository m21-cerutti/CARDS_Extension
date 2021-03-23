# Extension of CARDS project

Project "MarkerLess methods for Interactive tool in Augmented Reality with RGB-Camera", student project, University of Bordeaux.

OS : Windows

IDE recommended:  Visual Studio Community 2019.

## Hierarchy
---

It's only afer have installed OpenCV, Python, and Unity you can begin to modify and launch the differents projects. Please follow the instructions of the next sections if you haven't installed dependencies yet.

There is three independant folder projects and a folder for camera calibration.

**cards_rgbtrack** is the main folder for dll plugin.

**console** is the folder for debuging and have more information about DLL workflow test.
It permit also to write XML annotation for videos.

**unity** is like this name the Unity3D project. 

**videos_tests** is the folder containing video and XML annotation for testing.

**camera_calibration** is the folder for calibrating the camera.

**open_logs scripts** is utilities scripts for accesing more quickly to logs and datas about tracking performances and precision.

Instructions are on this file and on ./unity/Assets/Readme.md for special configurations and problems on Unity.

## Installation
### Unity3D
---

The project need Unity 2019.4.18f1. For intalling it, follow Unity instructions. No particular module is needed, except the Standalone or Windows build support for building a executable with Unity.

https://unity3d.com/fr/get-unity/download/archive


### OpenCV
---

Based on tutorial from thomasmountainborn.com, see **Links**.

1. Prepare a folder for installing opencv library and contribution and we will call it $(OPENCV_DIR)/ during this tutorial. Make inside a folder, $(OPENCV_DIR)/sources/, where we will put the opencv sources in 3.4.13 folder (or whatever version youy use), and do the same with contribution (3.4.13-contrib). Normally you will have $(OPENCV_DIR)/sources/3.4.13 and $(OPENCV_DIR)/sources/3.4.13-contrib.

2.  Download OpenCV 3.4.13 sources. Extract the files sources to the folder $(OPENCV_DIR)/sources/3.4.13, for contribution extract it to the contrib folder $(OPENCV_DIR)/sources/3.4.13-contrib.

https://opencv.org/releases/

https://github.com/opencv/opencv_contrib/releases

3. Download and install CMake.

https://cmake.org/download/

4. Download and install last version of Visual Studio community (here 2019 16.8.4) with modules "Development Desktop in C++", "Development .NET Desktop", "Game developpment with Unity"

https://visualstudio.microsoft.com/fr/vs/

5. Run CMake. At the top, click “Browse Source”, and navigate to the directory where you unpacked OpenCV in step 2, select $(OPENCV_DIR)/sources/3.4.13 as the source folder for CMake.

6. Create a new folder $(OPENCV_DIR)/build next to $(OPENCV_DIR)/sources. Select it as build directory for CMake.

7. With the two paths set up, go ahead and click “Configure” at the bottom. Choose  for generator Visual Studio 2019 with default native compilers. Once CMake is done, you will see a large number of red items appear in the center area of the application. These are all the build options.

8. Change the option OPENCV_EXTRA_MODULES_PATH that we set to the $(OPENCV_DIR)/sources/3.4.13-contrib/modules. It will enable the contributions to the build. Sometimes also, CMAKE_INSTALL_PREFIX is set to a subfolder $(OPENCV_DIR)/build/install, change it to the parent folder build.

8. Click “Generate”, and let CMake work.

9. Once done, now navigate to the build directory you set up in step 6, and open OpenCV.sln . You should give Visual Studio some time to initialize the projects and scan all #included files before going next – wait for the “Ready” status at the bottom of the window. If there is error, maybe you have forgotten the required components in step 4.

10. Build (F6) for both the Debug and Release x64 version of OpenCV. Make sure the INSTALL is set as active projet (set it in properties>StartupProject) otherwise BatchBuild to build the INSTALL in Debug/Release mode.

The builds will take a couple of minutes. Once the builds are done, we can finish setting everything up.

11. Verify in the build directory from step 6, you have now a bin, a lib, and a include folder. 

12. Add the $(OPENCV_DIR)/build directory to Window’s Path system variable – this just serves to tell the OS where to look for dll’s, so you don’t have to copy the dll’s to every OpenCV project you make. Go to the Control panel > System > Advanced system settings and click Environment Variable.

13. Under “System variables”, add a new entry called OPENCV_DIR, and assign the path of the build.

14. In the User variables section above the system variables, find the PATH entry, and click edit. At the end, add %OPENCV_DIR%\bin.

15. Copy the include of the mudules you want from contribution. Here we will use tracking module.

### Problems to link OpenCV
---

If you have problem with link of library, maybe it's because the include folder location is differrent than $(OPENCV_DIR)/build/, try to see in $(OPENCV_DIR)/build/install/include if there is the include files, and if it's true, verify the cmake option CMAKE_INSTALL_PREFIX (step 8).

It can be also a bad configuration in Cmake for contrib library, verify the ENABLE_PRECOMPILED_HEADERS FLAG SET TO FALSE.

An other could be non free algorithms have been implemented that need OPENCV_ENABLE_NONFREE flag in Cmake. 

Otherwise verify your path configuration in project.

### Python 3
---

The project need Python 3 installed for utilities script like DLL synchronisation or camera calibration. Please make sure you can run python scripts in folders. If not, dowload the latest version.

https://www.python.org/downloads/

## Modify DLL Plugin cards_rgbtrack
---

DLL files are handled in this project by symlink on Windows, but Git automatically copy all DLL if registered and replace the simlink files, that's mostly true for Unity developpement that need DLL to start. But if you want to update the DLL in console or Unity, make sure the synchronisation's scripts work. 
Use the "sync_dll_release.bat" (or debug if you want) in admin mode, and a "SYNC DONE" must appear to the console. If not, verify the problematic folder or file, if it's in OpenCV verify your installation, otherwise it can be file permission or an error if the file already exist.

You can modify the DLL you use and the target path for symlink in "ddl_*.txt" and "folder_*.txt file".

Otherwise, to modify the source code, open with Visual Studio cards_rgbtrack.sln project, and it will be the same as develloping C++ CLI DLL. The development and how to add other OpenCV modules will work like any other C++ Console project. 

To test it on console or unity, don't forget to synchronise it (one time is enough, if no git process have altered symlink files). If a DLL is missing on console or Unity, use https://github.com/lucasg/Dependencies to analyse the cards_rgbtrack.dll and know the ones missing compare to the ones reffered in the dll_*.txt.

##  Quick use Unity
---

Now it will have all set up to use the project. For testing on Unity, just open the folder ./unity/ with Unity3D (Add project on Unity Hub) and go on the scene "pluginTest".
Since the dll are already compiled, if you press "Play" it will launch the testing scenerio set in "Logic" GameObject, in SingleThread script.
You can change it with  current others scenarios ind Assets/Data or make your own.

See more information on Unity Readme in ./unity/Assets.

## Use console
---

In order to use the console, build before the DLL, and open the console.sln project. Since we haven't implemented command line system for choosing tests or write xml annotation, uncomment what you want in the main function in console.cpp file.

For opening video or change output folder for XML files, you will need to change the code.

For xml annotation, select on Select ROI windows the objects you want but be carefull to do it on the same order, in XML anotation but also when comparing in Unity. A debug on the current id you set is displayed on Console. Use Escape to skip a ID. Change the code for the number of ids to use.

## Camera calibration
---

Put all the images for calibration in camera_calibration/img_calibration folder, if you haven't, you can take images with the create_data_calib.py python script (s for saving image, q or escape for quit, space to skip delay). 

You have the cherckerboard file used in our test in the same folder, print it on A4 paper, or change the calibration.py script accordingly of your method of calibration, and paramaeters (size of square, distance to camera, etc.)

Then rename the images that have the chekerboard on table with "_table" suffix before extension that will handle special computing (use in pose estimation, set a reference to size objects). Launch then the calibration process and it will compute a XML  calibration file. Copy to StreamingAsset folder in Unity or at the root of console.

## Dependencies
---

- OpenCV 3.4.13
- OpenCV 3.4.13 contrib
- Unity 2019.4.18f1
- Python 3

## Links
---

- Forum fo integrate OpenCV with Native plugins in Unity

https://forum.unity.com/threads/tutorial-using-c-opencv-within-unity.459434/

- Tutorial from thomasmountainborn.com
https://thomasmountainborn.com/2016/09/11/unity-and-opencv-part-one-install/
http://thomasmountainborn.com/2016/09/12/unity-and-opencv-part-two-project-setup/
http://thomasmountainborn.com/2017/03/05/unity-and-opencv-part-three-passing-detection-data-to-unity/

- Base project 

https://github.com/iseta/UnityAndOpenCV

- Install contrib

https://cv-tricks.com/how-to/installation-of-opencv-4-1-0-in-windows-10-from-source/
https://docs.opencv.org/3.4.13/d3/d52/tutorial_windows_install.html

## Contact
---

Researchers :
---
- Philippe Giraudeau <philippe.giraudeau@inria.fr>
- Joan Odicio vilchez <joan.odicio-vilchez@inria.fr>

Students :
---
- Marc Cerutti <marc.cerutti@u-bordeaux.fr>
- Gabriel Lorrain <gabriel.lorrain@u-bordeaux.fr>
- Johann Kadionik <johann.kadionik@etu.u-bordeaux.fr>
- Kevin Durou <kevin.durou@etu.u-bordeaux.fr>