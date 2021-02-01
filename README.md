# Extension of CARDS project

Project done for project "MarkerLess methods for Interactive tool in Augmented Reality with RGB-Camera", student project, University of Bordeaux.

IDE recommended:  Visual Studio Community 2019.

## Installation
---

Based on tutorial from thomasmountainborn.com, see **Links**.

1. Download OpenCV 4.5.1

https://opencv.org/releases/

2. Run the .exe and extract the files to the folder of installation of OpenCV. 

3. Download and install CMake.

https://cmake.org/download/

4. Download and install last version of Visual Studio community (here 2019 16.8.4) with modules "Development Desktop in C++", "Development .NET Desktop", "Game developpment with Unity"

https://visualstudio.microsoft.com/fr/vs/

5. Run CMake. At the top, click “Browse Source”, and navigate to the directory where you unpacked OpenCV in step 2, we will call it $(OPENCV_DIR)/. Under that directory, select $(OPENCV_DIR)/sources as the source folder for CMake.

6. Create a new folder $(OPENCV_DIR)/build next to $(OPENCV_DIR)/sources. Select it as build directory for CMake.

7. With the two paths set up, go ahead and click “Configure” at the bottom. Choose  for generator Visual Studio 2019 with default native compilers. Once CMake is done, you will see a large number of red items appear in the center area of the application. These are all the build options and we let it to defaults.

8. Click “Generate”, and let CMake work.

9. Once done, now navigate to the build directory you set up in step 6, and open OpenCV.sln . You should give Visual Studio some time to initialize the projects and scan all #included files before going next – wait for the “Ready” status at the bottom of the window. If there is error, maybe you have forgotten the required components in step 4.

10. Build (F6) for both the Debug and Release x64 version of OpenCV. 

The builds will take a couple of minutes. Once the builds are done, we can finish setting everything up.

11. Verify in the build directory from step 6, you have now a bin, a lib, and a include folder. 

12. Add the $(OPENCV_DIR)/build directory to Window’s Path system variable – this just serves to tell the OS where to look for dll’s, so you don’t have to copy the dll’s to every OpenCV project you make. Go to the Control panel > System > Advanced system settings and click Environment Variable.

13. Under “System variables”, add a new entry called OPENCV_DIR, and assign the path of the build.

14. In the User variables section above the system variables, find the PATH entry, and click edit. At the end, add %OPENCV_DIR%\bin.

## Use
---

Now it will have all set up to use the project. For testing on Unity, just open the folder ./unity/ with Unity3D (Add project on Unity Hub) and go on the scene "RedCircleAroundFace".
Since the dll are already compiled, if you press "Play" it will open your default camera and execute a face tracker.

To modify or recompile the detectorCV.dll, you need to open ./detectorCV/detectorCV.sln and build the solution. Replace with the dll produce the one in ./unity/Assets/Plugins (you need to restart Unity to be effective). The development and how to add other OpenCV modules will work like any other C++ Console OpenCV project.

Also if you add Libraries in the C++ code, don't forget to copy dll from $(OPENCV_DIR)/build/Realease to Unity. To know what dll we need, we recommend using 
https://github.com/lucasg/Dependencies to analyse the detectorCV.dll and know the ones missing in the folder Plugins.

## Dependencies
---

- OpenCV 4.5.1
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

## Contact
---

Researchers :
Philippe Giraudeau <philippe.giraudeau@inria.fr>
Joan Odicio vilchez <joan.odicio-vilchez@inria.fr>

Students :
Marc Cerutti <marc.cerutti@u-bordeaux.fr>
Gabriel Lorrain <gabriel.lorrain@u-bordeaux.fr>
Johann Kadionik <johann.kadionik@etu.u-bordeaux.fr>
Kevin Durou <kevin.durou@etu.u-bordeaux.fr>