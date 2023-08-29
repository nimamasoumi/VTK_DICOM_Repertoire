cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(VTK_DICOM_Repertoire)

find_package(VTK COMPONENTS
    CommonColor
    CommonCore
    IOImage
    InteractionImage
    InteractionStyle
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

add_executable(VTK_DICOM_Repertoire MACOSX_BUNDLE dicomProcess.cxx )
    target_link_libraries(VTK_DICOM_Repertoire PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
    TARGETS VTK_DICOM_Repertoire
    MODULES ${VTK_LIBRARIES}
)



