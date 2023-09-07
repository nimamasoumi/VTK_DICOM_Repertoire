#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkBorderWidget.h>
#include <vtkBorderRepresentation.h>

int main(int argc, char* argv[])
{
    vtkNew<vtkNamedColors> colors;

    // Verify input arguments
    if (argc < 2)
    {        
        std::cout << "Please specify the DICOM file."<<std::endl;
        std::cout << "Usage: " << argv[0] << " <Filename>"<< std::endl;        
        return EXIT_FAILURE;
    }
    std::string inputFilename = argv[1];

    // Read all the DICOM files in the specified directory.
    vtkNew<vtkDICOMImageReader> reader;
    reader->SetFileName(inputFilename.c_str());
    reader->Update();

    // Visualize
    vtkNew<vtkRenderWindow> renderWindow;    
    double xmins[3] = {0, 0.333, 0.666};
    double xmaxs[3] = {0.333, 0.666, 1};
    double ymins[3] = {0, 0, 0};
    double ymaxs[3] = {1, 1, 1};    
     
    vtkNew<vtkImageViewer2> imageViewer;    
    vtkNew<vtkImageViewer2> imgV2;
    vtkNew<vtkImageViewer2> imgV3;

    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    // imageViewer->SetupInteractor(renderWindowInteractor);
    // imgV2->SetupInteractor(renderWindowInteractor);        
    // imgV3->SetupInteractor(renderWindowInteractor);

    renderWindowInteractor->SetRenderWindow(renderWindow);

    vtkCamera* camera = nullptr;

    // Multiple viewports
    renderWindow->AddRenderer(imageViewer->GetRenderer());
    imageViewer->GetRenderer()->SetViewport(xmins[0], ymins[0], xmaxs[0], ymaxs[0]);
    camera = imageViewer->GetRenderer()->GetActiveCamera();    
    imageViewer->SetInputConnection(reader->GetOutputPort());
    imageViewer->GetRenderer()->SetBackground(colors->GetColor3d("Lime").GetData());
    imageViewer->GetRenderer()->ResetCamera();

    inputFilename = argv[2];
    vtkNew<vtkDICOMImageReader> reader2;
    reader2->SetFileName(inputFilename.c_str());
    reader2->Update();
    renderWindow->AddRenderer(imgV2->GetRenderer());
    imgV2->GetRenderer()->SetViewport(xmins[1], ymins[1], xmaxs[1], ymaxs[1]);
    imgV2->GetRenderer()->SetActiveCamera(camera);    
    imgV2->SetInputConnection(reader2->GetOutputPort());
    imgV2->GetRenderer()->SetBackground(colors->GetColor3d("SlateGray").GetData());
    imgV2->GetRenderer()->ResetCamera();

    renderWindow->AddRenderer(imgV3->GetRenderer());
    imgV3->GetRenderer()->SetViewport(xmins[2], ymins[2], xmaxs[2], ymaxs[2]);
    imgV3->GetRenderer()->SetActiveCamera(camera);
    imgV3->SetInputConnection(reader->GetOutputPort());
    imgV3->GetRenderer()->SetBackground(colors->GetColor3d("Navy").GetData());
    imgV3->GetRenderer()->ResetCamera();

    vtkNew<vtkBorderWidget> roiBox;
    vtkNew<vtkBorderRepresentation> roiRep;
    roiRep->SetPosition(0.2,0.01);
    roiRep->SetPosition2(0.6,0.85);
    roiRep->Modified();
    roiBox->SetInteractor(renderWindowInteractor);
    roiBox->SetRepresentation(roiRep);
    roiBox->EnabledOn();
    
    renderWindow->Render();
    renderWindow->SetSize(1200, 800);
    renderWindow->SetPosition(2200, 200);
    renderWindow->SetWindowName("Showing multiple viewports");

    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
