# VTK DICOM Repertoire using C++
Programming conceptual C++ codes using VTK libraries for lernears and enthusiasts. 
The focus is on reading, writing, and processing DICOM data. 

![alt-text](https://github.com/nimamasoumi/VTK_DICOM_Repertoire/blob/main/dcm.png?raw=true)

## Usage and run the program ##

- Step 1:
  Follow the instructions in https://cmake.org/install/ to install CMake
  
- Step 2:
  Follow the instructions in https://vtk.org/download/ to download and install VTK
  
- Step 3:
  Go to the VTK_DICOM_Repertoire source code directory:
  
  `cd <Your_Directories>/VTK_DICOM_Repertoire`

- Step 4:
  Make a build directory in the parent directory:
  
  `mkdir ../VTK_DICOM_Repertoire-build`

- Step 5:
  Configure the files using CMake and make sure there are no errors:
  
  `cmake ../VTK_DICOM_Repertoire`

- Step 6:
  Compile and build the project:
  
  `make`

- Step 7:
  Run one of the executables, for instance:
  
  `dicomSeriesProcess <Path_To_A_DICOM_Directory>`

## Remarks ##

I am expanding my VTK and DICOM knowledge everyday. I create codes to showcase the concept in a comprehensible example. Following is the list of topics that I generated the exmaple codes using VTK:

1. Read the DICOM files
2. Graphically show the DICOM files
3. Import a DICOM directory with several image slices
4. Show different slices, zoom, and pan the images
5. Display multiple viewports 
6. Show an interative border widget superimposed on images
7. Show 2D slices of 3D image from axial, coronal, and sagittal views
8. Create a custom observer callback using VTK implementation of observer/command design pattern