## SatModelViewer — ACIS SAT Viewer Sample

SatModelViewer is an MFC desktop sample that loads ACIS SAT models using the Spatial ACIS SDK and visualizes them with OpenGL.

### Features
- Load and save SAT files
- Faceted rendering of faces/edges/vertices with OpenGL
- Mouse interaction: rotate, pan, zoom
- Picking (faces/edges/points) and highlight
- Dockable panes: entity tree, JSON view, output, properties

### Requirements
- Windows + Visual Studio (2019/2022 recommended, include MFC)
- Spatial ACIS SDK (valid license required)
- OpenGL + FreeGLUT available to the app (headers/libs/DLLs)

### Build
1. Open `ModelViewer1.vcxproj` in Visual Studio
2. Configure ACIS SDK include/lib paths and link required libraries
3. Configure FreeGLUT include/lib and ensure runtime DLLs are present
4. Implement/replace license unlock in `unlock_license.cpp`
5. Build and run (Win32/x64, Debug/Release)

### Usage
- Start the app and open a `.sat` file from the menu/toolbar
- Mouse: left-drag rotate, middle-drag pan, wheel zoom, right-click pick

### Display Settings (`setting.ini`)
Create `setting.ini` next to the executable to override defaults:

```ini
[DISPLAY]
DisplayClickedPmi=0
DisplayFace=1
LineWidth=1.0
PointSize=1.0
FaceAlpha=1.0
LineAlpha=1.0
PointAlpha=1.0
ColorEdgeR=1.0
ColorEdgeG=1.0
ColorEdgeB=1.0
```

### Notes
- ACIS is commercial; binaries and licenses are not included
- Sample STL files and STL loader are provided for reference


