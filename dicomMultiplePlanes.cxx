#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor2D.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkInteractorStyleImage.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkBorderWidget.h>
#include <vtkBorderRepresentation.h>
#include <vtkCamera.h>
#include <vtkImageActor.h>

#include <sstream>
#include <thread>
#include <exception>

namespace {

// Helper class to format slice status message.
class StatusMessage
{
public:
  static std::string Format(int slice, int maxSlice)
  {
    std::stringstream tmp;
    tmp << "Slice Number  " << slice + 1 << "/" << maxSlice + 1;
    return tmp.str();
  }
};

// Define own interaction style.
class myVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
  static myVtkInteractorStyleImage* New();
  vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage);

protected:
  vtkImageViewer2* _ImageViewer;
  vtkTextMapper* _StatusMapper;
  int _Slice;
  int _MinSlice;
  int _MaxSlice;

public:
  void SetImageViewer(vtkImageViewer2* imageViewer)
  {
    _ImageViewer = imageViewer;
    _MinSlice = imageViewer->GetSliceMin();
    _MaxSlice = imageViewer->GetSliceMax();
    _Slice = _MinSlice;
    cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice
         << std::endl;
  }

  void SetStatusMapper(vtkTextMapper* statusMapper)
  {
    _StatusMapper = statusMapper;
  }

protected:
  void MoveSliceForward()
  {
    if (_Slice < _MaxSlice)
    {
      _Slice += 1;
      cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
      _ImageViewer->SetSlice(_Slice);
      std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
      _StatusMapper->SetInput(msg.c_str());
      _ImageViewer->Render();
    }
  }

  void MoveSliceBackward()
  {
    if (_Slice > _MinSlice)
    {
      _Slice -= 1;
      cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
      _ImageViewer->SetSlice(_Slice);
      std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
      _StatusMapper->SetInput(msg.c_str());
      _ImageViewer->Render();
    }
  }

  virtual void OnKeyDown()
  {
    std::string key = this->GetInteractor()->GetKeySym();
    if (key.compare("Up") == 0)
    {
      // cout << "Up arrow key was pressed." << endl;
      MoveSliceForward();
    }
    else if (key.compare("Down") == 0)
    {
      // cout << "Down arrow key was pressed." << endl;
      MoveSliceBackward();
    }
    // forward event
    vtkInteractorStyleImage::OnKeyDown();
  }

  virtual void OnMouseWheelForward()
  {
    // std::cout << "Scrolled mouse wheel forward." << std::endl;
    if (_Slice > _MinSlice)
    {
      MoveSliceBackward();
    }    
    // don't forward events, otherwise the image will be zoomed
    // in case another interactorstyle is used (e.g. trackballstyle, ...)
    // vtkInteractorStyleImage::OnMouseWheelForward();
  }

  virtual void OnMouseWheelBackward()
  {
    // std::cout << "Scrolled mouse wheel backward." << std::endl;
    MoveSliceForward();
    // don't forward events, otherwise the image will be zoomed
    // in case another interactorstyle is used (e.g. trackballstyle, ...)
    // vtkInteractorStyleImage::OnMouseWheelBackward();
  }
};

vtkStandardNewMacro(myVtkInteractorStyleImage);

// void runInteractor(vtkNew<vtkRenderWindowInteractor>& _intRW)
// {
//     _intRW->Start();
// }

// void changePos(vtkNew<vtkBorderRepresentation>& roiRep, double& _p1)
// {
//     bool flag = true;
//     while(flag)
//     {
//         if(roiRep->GetPosition()[0]!=_p1)
//         {
//             roiRep->SetPosition(_p1,0.01);
//             roiRep->Modified();
//         }
//     }    
// }

void createSlicer(const std::string _fileName, const int _orient)
{
    vtkNew<vtkNamedColors> colors;
    // Read all the DICOM files in the specified directory.
    vtkNew<vtkDICOMImageReader> reader;
    reader->SetDirectoryName(_fileName.c_str());
    reader->Update();

    // Visualize
    vtkNew<vtkImageViewer2> imageViewer;
    imageViewer->SetInputConnection(reader->GetOutputPort());
    switch (_orient)
    {
        case 0:
        imageViewer->SetSliceOrientationToXY();
        break;

        case 1:
        imageViewer->SetSliceOrientationToXZ();
        break;

        case 2:
        imageViewer->SetSliceOrientationToYZ();
        break;

        default:
        break;
    }    

    // Slice status message.
    vtkNew<vtkTextProperty> sliceTextProp;
    sliceTextProp->SetFontFamilyToCourier();
    sliceTextProp->SetFontSize(20);
    sliceTextProp->SetVerticalJustificationToBottom();
    sliceTextProp->SetJustificationToLeft();

    vtkNew<vtkTextMapper> sliceTextMapper;
    std::string msg = StatusMessage::Format(imageViewer->GetSliceMin(),
                                            imageViewer->GetSliceMax());
    sliceTextMapper->SetInput(msg.c_str());
    sliceTextMapper->SetTextProperty(sliceTextProp);

    vtkNew<vtkActor2D> sliceTextActor;
    sliceTextActor->SetMapper(sliceTextMapper);
    sliceTextActor->SetPosition(15, 10);

    // Usage hint message.
    vtkNew<vtkTextProperty> usageTextProp;
    usageTextProp->SetFontFamilyToCourier();
    usageTextProp->SetFontSize(14);
    usageTextProp->SetVerticalJustificationToTop();
    usageTextProp->SetJustificationToLeft();

    vtkNew<vtkTextMapper> usageTextMapper;
    usageTextMapper->SetInput(
        "- Slice with mouse wheel\n  or Up/Down-Key\n- Zoom with pressed right\n "
        " mouse button while dragging");
    usageTextMapper->SetTextProperty(usageTextProp);

    vtkNew<vtkActor2D> usageTextActor;
    usageTextActor->SetMapper(usageTextMapper);
    usageTextActor->GetPositionCoordinate()
        ->SetCoordinateSystemToNormalizedDisplay();
    usageTextActor->GetPositionCoordinate()->SetValue(0.05, 0.95);

    // Create an interactor with our own style (inherit from
    // vtkInteractorStyleImage) in order to catch mousewheel and key events.  
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;

    vtkNew<myVtkInteractorStyleImage> myInteractorStyle;

    // Make imageviewer2 and sliceTextMapper visible to our interactorstyle
    // to enable slice status message updates when scrolling through the slices.
    myInteractorStyle->SetImageViewer(imageViewer);
    myInteractorStyle->SetStatusMapper(sliceTextMapper);

    imageViewer->SetupInteractor(renderWindowInteractor);
    // Make the interactor use our own interactorstyle
    // cause SetupInteractor() is defining it's own default interatorstyle
    // this must be called after SetupInteractor().
    renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
    // Sdd slice status message and usage hint message to the renderer.
    imageViewer->GetRenderer()->AddActor2D(sliceTextActor);
    imageViewer->GetRenderer()->AddActor2D(usageTextActor);


    //vtkCamera* camera = nullptr;
    // Initialize rendering and interaction.
    imageViewer->Render();  
    //camera = imageViewer->GetRenderer()->GetActiveCamera();    
    //imageViewer->GetRenderer()->GetActiveCamera()->SetViewUp(1.0,0.0,0.0);
    vtkNew<vtkBorderWidget> roiBox;
    vtkNew<vtkBorderRepresentation> roiRep;
    roiRep->SetPosition(0.6,0.01);
    roiRep->SetPosition2(0.6,0.85);
    roiRep->Modified();
    roiBox->SetInteractor(renderWindowInteractor);
    roiBox->SetRepresentation(roiRep);
    roiBox->EnabledOn();
    
    double* ssX = imageViewer->GetImageActor()->GetXRange();
    double* ssY = imageViewer->GetImageActor()->GetYRange();
    double* ssZ = imageViewer->GetImageActor()->GetZRange();

    imageViewer->GetRenderer()->ResetCamera();  
    imageViewer->GetRenderer()->SetBackground(
        colors->GetColor3d("SlateGray").GetData());    
    imageViewer->GetRenderWindow()->SetPosition(2200, 100);
    
    //std::cout<<ss[0]<<" and "<<ss[1]<<"\n"<<std::endl;

    switch (_orient)
    {
        case 0:
        imageViewer->GetRenderWindow()->SetWindowName("Axial View");
        imageViewer->GetRenderWindow()->SetSize((int)ssX[1], (int)ssY[1]);
        break;

        case 1:
        imageViewer->GetRenderWindow()->SetWindowName("Coronal View");
        break;

        case 2:
        imageViewer->GetRenderWindow()->SetWindowName("Sagittal View");
        break;

        default:
        break;
    }       
    imageViewer->Render();

    //std::thread t4(changePos, std::ref(roiRep), std::ref(_p1));    

    renderWindowInteractor->Start();
}

} // namespace

int main(int argc, char* argv[])
{  

  // Verify input arguments.
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " FolderName" << std::endl;
    return EXIT_FAILURE;
  }

  std::string folder = argv[1];  

  try{
    std::thread t1(createSlicer, folder, 0);  
    // Second Window in XZ orientation
    std::thread t2(createSlicer, folder, 1);  
    // Third Window in YZ orientation
    std::thread t3(createSlicer, folder, 2);  
    t1.join();
    t2.join();
    t3.join();
  }catch(std::exception& e)
  {
    std::cout<< e.what() << std::endl;
  }
//   std::cout<<"Testing if the code comes here.\n\n"<<std::endl;
  return EXIT_SUCCESS;
}