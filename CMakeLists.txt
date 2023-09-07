cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(VTK_DICOM_Repertoire)

find_package(VTK COMPONENTS
    CommonColor
    CommonCore
    CommonDataModel
    IOImage
    InteractionImage
    InteractionStyle
    InteractionWidgets
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
)

if (NOT VTK_FOUND)
    message(FATAL_ERROR "Unable to find VTK build folder.")
else()
    message("Found VTK build folder!\nAvailable variables:\n")
    message("VTK_FOUND: "${VTK_FOUND})
    message("VTK_LIBRARIES: "${VTK_LIBRARIES})
    message("\n")
endif()

add_executable(dicomProcess MACOSX_BUNDLE dicomProcess.cxx )
target_link_libraries(dicomProcess PRIVATE ${VTK_LIBRARIES})

add_executable(dicomSeriesProcess MACOSX_BUNDLE dicomSeriesProcess.cxx )
target_link_libraries(dicomSeriesProcess PRIVATE ${VTK_LIBRARIES})

add_executable(dicomMultipleView MACOSX_BUNDLE dicomMultipleView.cxx)
target_link_libraries(dicomMultipleView PRIVATE ${VTK_LIBRARIES})

add_executable(dicomMultiplePlanes MACOSX_BUNDLE dicomMultiplePlanes.cxx)
target_link_libraries(dicomMultiplePlanes PRIVATE ${VTK_LIBRARIES})

# vtk_module_autoinit is needed
vtk_module_autoinit(
    TARGETS dicomProcess dicomSeriesProcess dicomMultipleView dicomMultiplePlanes
    MODULES ${VTK_LIBRARIES}
)



