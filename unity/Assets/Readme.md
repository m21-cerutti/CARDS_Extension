# Unity CARDS Extension Project

## Hierarchy 
---
```
Assets
├───Animations
├───Asset  Store
│   ├───PBR Table
│   │   ├───Prefabs
│   │   └───_Models
│   │       ├───Materials
│   │       └───Table_Textures
│   └───Wood Patterns
│       ├───Materials
│       └───Textures
├───Data
├───Materials
├───Models
├───Plugins
│   └───Windows
├───Resources
├───Scenes
├───Scripts
│   ├───DebugTargets
│   ├───Editor
│   ├───Plugin
│   ├───Singleton
│   ├───Thread
│   └───Video
└───StreamingAssets
```

The most important folders for linking the DLL are unity/Ressources and the Assets/Plugins/Window folder. Norammly, it's already done when synchronising DLLs.

The StreamingAssets folder is used for video testing, it's where we put the calibration xml file.

In the Scripts/Plugin folder, there is the wrapper for using the dll functions in Unity. 

The folder Data is where ther is configuration data assets, used for different scenarios test depending on the source media.

In order to see something first on scene, open first pluginTest scene.

## Use & inputs
---

When using playmode or buid, use escape to exit the program. When selectioning ROI, use escape and reset focus again on the game window for escaping.

Otherwise it will depend on source media you use, change it on "Logic" GameObject, and click on the sciptable object VideoParameters to go on Assets/Data and see the parameters available.
In order to debug with OpenCV windows, use the Debug_cv option in it. See script documentation of VideoParameters.cs for more details on options.
For creating your own source media for testing RightClick project tab > Create > VideoParameters > Parameters or copy and rename a VideoParamaters asset.

On webcam or virtual source, there is no end but on video when finished, a warning "Empty frame" will pop up in Unity editor (also in build if autoconnect profiler is set). It permit to debug it without the Debug_cv option.

For selectionning target when a "Select Roi" windows pop up, click and drag for making the bounding boxe, and the enter for each traget. Press esape on this windows to finish it.

## Build 
---

For building, we recommend to put put the installation folder at the root of the git in BuildUnity/, because for video it permeit to not change path in VideoParameters data before build. Otherwise it work like in the editor. You can change the calibration file in CARDS_Extension_Data/StreamingAssets in build folder, but be carefull because the one in Unity will overwrite it if you rebuild in the same folder.

## Crashs
---

If Unity crash, it can be a problem with the DLL memory or an exception have occured inside. Refresh the DLL used (section Modify DLL Plugin cards_rgbtrack) and try again. If the problem persist, use instead the console in Debug mode (build the DLL also in Debug and sync it) in order to have more information about the problem.
