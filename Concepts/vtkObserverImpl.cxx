#include <iostream>

#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkBorderWidget.h>
#include <vtkBorderRepresentation.h>

namespace
{
class myCommand : public vtkCommand
{
    public: 
    void Execute(vtkObject* caller, unsigned long eid, void* callData)
    {
        std::cout<<"This is the Observer's callback!\n"<<std::endl;
    }
};
}

int main (int argc, char* argv[])
{
    std::cout<<"Testing the observer design pattern implementation."
        <<std::endl;

    std::string dfile = argv[1];
    vtkNew<vtkDICOMImageReader> reader;
    reader->SetDirectoryName(dfile.c_str());
    reader->Update();

    vtkNew<vtkImageViewer2> imageViewer;
    vtkNew<vtkRenderWindowInteractor> interactor;
    vtkNew<vtkNamedColors> colors;
    imageViewer->SetInputData(reader->GetOutput());
    //imageViewer->SetInputConnection(reader->GetOutputPort());
    imageViewer->GetRenderWindow()->SetInteractor(interactor);
    imageViewer->GetRenderer()->SetBackground(
        colors->GetColor3d("LightBlue").GetData());
    imageViewer->GetRenderer()->ResetCamera();
    imageViewer->Render();  

    vtkNew<vtkBorderWidget> roiBox;
    vtkNew<vtkBorderRepresentation> roiRep;
    roiRep->SetPosition(0.6,0.01);
    roiRep->SetPosition2(0.6,0.85);
    roiRep->Modified();
    roiBox->SetInteractor(interactor);
    roiBox->SetRepresentation(roiRep);
    roiBox->EnabledOn();

    // If the window is resized, it will invoke the observer's callback
    auto myC = new myCommand(); 
    imageViewer->GetRenderWindow()->AddObserver(myC->WindowResizeEvent,myC);

    auto widgetC = new myCommand();
    roiBox->AddObserver(widgetC->UpdateEvent, widgetC);

    interactor->Start();

    return EXIT_SUCCESS;
}