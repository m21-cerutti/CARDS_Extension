# Extension of CARDS project

Project "MarkerLess methods for Interactive tool in Augmented Reality with RGB-Camera", student project, University of Bordeaux.

IDE recommended:  Visual Studio Community 2019.

## Installation OpenCV
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

7. With the two paths set up, go ahead and click “Configure” at the bottom. Choose  for generator Visual Studio 2019 with default native compilers. Once CMake is done, you will see a large number of red items appear in the center area of the application. These are all the build options and we let it to defaults, except OPENCV_EXTRA_MODULES_PATH that we set to the $(OPENCV_DIR)/sources/3.4.13-contrib/modules. I t will enable the contributions to the build.

8. Click “Generate”, and let CMake work.

9. Once done, now navigate to the build directory you set up in step 6, and open OpenCV.sln . You should give Visual Studio some time to initialize the projects and scan all #included files before going next – wait for the “Ready” status at the bottom of the window. If there is error, maybe you have forgotten the required components in step 4.

10. Build (F6) for both the Debug and Release x64 version of OpenCV. Make sure the INSTALL is set as active projet (set it in properties> StartupProject otherwise BatchBuild to build the INSTALL in Debug/Release).

The builds will take a couple of minutes. Once the builds are done, we can finish setting everything up.

11. Verify in the build directory from step 6, you have now a bin, a lib, and a include folder. 

12. Add the $(OPENCV_DIR)/build directory to Window’s Path system variable – this just serves to tell the OS where to look for dll’s, so you don’t have to copy the dll’s to every OpenCV project you make. Go to the Control panel > System > Advanced system settings and click Environment Variable.

13. Under “System variables”, add a new entry called OPENCV_DIR, and assign the path of the build.

14. In the User variables section above the system variables, find the PATH entry, and click edit. At the end, add %OPENCV_DIR%\bin.

15. Copy the include of the mudules you want from contribution. Here we will use tracking module.

## Problem to link OpenCV
---

If you have problem with link of library, maybe it's because the include folder is differrent than $(OPENCV_DIR)/build/, try to see in $(OPENCV_DIR)/build/install/include if there is the include files, or in cmake CMAKE_INSTALL_PREFIX path. It can be also a bad configuration in Cmake FOR contrib library? VERIFY THE ENABLE_PRECOMPILED_HEADERS FLAG SET TO FALSE.
It can also because you're using non free algorithms that need OPENCV_ENABLE_NONFREE flag in Cmake. Otherwise verify your path configuration in project.

## Use
---

Now it will have all set up to use the project. For testing on Unity, just open the folder ./unity/ with Unity3D (Add project on Unity Hub) and go on the scene "RedCircleAroundFace".
Since the dll are already compiled, if you press "Play" it will open your default camera and execute a face tracker.

To modify or recompile the cards_rgbtrack.dll, you need to open ./cards_rgbtrack/cards_rgbtrack.sln and build the solution. It will automatically update the library to Unity and the console programm if you haven't rename the folders, in ./console/x64/Release or Debug and ./unity/Assets/Plugins(you need to restart Unity to be effective). 

The development and how to add other OpenCV modules will work like any other C++ Console OpenCV project. In order to simplify test, add to the dll.txt the dll needed for the library, and use syncdll.py for syncrhronising the dll to unity and the console. Use the %OPENCV_DIR%\ in dll.txt to automate the copy of the opencv dll (set with environment variables).

Use https://github.com/lucasg/Dependencies to analyse the cards_rgbtrack.dll and know the ones missing in the dll.txt.

## Dependencies
---

- OpenCV 3.4.13
- OpenCV 3.4.13 contrib
- Unity 2019.4.18f1

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