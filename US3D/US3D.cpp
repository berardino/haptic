// US3D.cpp: implementation of the US3D class.
//
//////////////////////////////////////////////////////////////////////

#include "US3D.h"
#include <FL/fl_file_chooser.H>
#include "vtkInteractorStyleTrackballCamera.h"
//#include "vtkMatrix4x4.h"
#include "vtkProcessObject.h"


char str[50];

struct ProgressArguments
{
	US3D *This;
	vtkProcessObject *Process;
	char *Label;
	Fl_Progress *progress;
};

US3D::US3D()
{
	this->init=false;
	this->m_Reader=vtkSPFReader::New();
	this->m_ImageFlipFilter=vtkImageFlip::New();
	this->m_VOI=vtkExtractVOI::New();
	this->m_Threshold=vtkImageThreshold::New();
	this->m_ResliceAxial=vtkImageReslice::New();
	this->m_ResliceSaggittal=vtkImageReslice::New();
	this->m_ResliceCoronal=vtkImageReslice::New();
	this->m_ResliceSAxial=vtkImageReslice::New();
	this->m_ResliceSSaggittal=vtkImageReslice::New();
	this->m_ResliceSCoronal=vtkImageReslice::New();
	this->m_MarchingCubes=vtkMarchingCubes::New();
	this->m_SmoothPolyDataFilter=vtkSmoothPolyDataFilter::New();
	this->m_PolyDataConnectivityFilter=vtkPolyDataConnectivityFilter::New();
	this->m_Decimate=vtkDecimatePro::New();
	this->m_VRMLExporter=vtkVRMLExporter::New();
	this->m_ImageGaussianSmoothFilter=vtkImageGaussianSmooth::New();
	this->m_PolyDataNormals=vtkPolyDataNormals::New();
	this->m_CleanPolyData=vtkCleanPolyData::New();
	this->m_Writer=vtkPolyDataWriter::New();
	this->tmpString=new char[200];


	this->m_ImageFlipFilter->SetFilteredAxis(1); 
	this->m_ImageFlipFilter->SetInput(this->m_Reader->m_Output);


	this->SetProgress(this->m_ImageGaussianSmoothFilter,"Filtering...",this->m_Progress);
	this->m_ImageGaussianSmoothFilter->SetStandardDeviation(3.5,3.5,3.5);
	this->m_ImageGaussianSmoothFilter->SetRadiusFactors(0.8,0.8,0.8);
	this->m_ImageGaussianSmoothFilter->SetDimensionality(3);
	this->m_ImageGaussianSmoothFilter->SetInput(this->m_ImageFlipFilter->GetOutput());
	
	this->m_VOI->SetInput(this->m_ImageGaussianSmoothFilter->GetOutput());

	
	this->m_Threshold->SetInValue(255);
	this->m_Threshold->ThresholdBetween(0,0);
	this->m_Threshold->SetInput(this->m_VOI->GetOutput());

	
	this->m_ResliceAxial->SetResliceAxesDirectionCosines(1,0,0,0,1,0,0,0,1);
	this->m_ResliceAxial->SetInterpolationModeToCubic();
	this->m_ResliceSaggittal->SetResliceAxesDirectionCosines(0,0,1,0,1,0,1,0,0);
	this->m_ResliceCoronal->SetResliceAxesDirectionCosines(1,0,0,0,0,1,0,1,0);
	this->m_ResliceAxial->SetInput(this->m_ImageGaussianSmoothFilter->GetOutput());
	this->m_ResliceSaggittal->SetInput(this->m_ImageGaussianSmoothFilter->GetOutput());
	this->m_ResliceCoronal->SetInput(this->m_ImageGaussianSmoothFilter->GetOutput());
	
	this->m_ResliceSAxial->SetResliceAxesDirectionCosines(1,0,0,0,1,0,0,0,1);
	this->m_ResliceSSaggittal->SetResliceAxesDirectionCosines(0,0,1,0,1,0,1,0,0);
	this->m_ResliceSCoronal->SetResliceAxesDirectionCosines(1,0,0,0,0,1,0,1,0);
	this->m_ResliceSAxial->SetInput(this->m_Threshold->GetOutput());
	this->m_ResliceSSaggittal->SetInput(this->m_Threshold->GetOutput());
	this->m_ResliceSCoronal->SetInput(this->m_Threshold->GetOutput());
	
	this->m_CDataFilteredAxial->SetFl_Output(this->m_CPosX,this->m_CPosY);
	this->m_CDataFilteredSaggittal->SetFl_Output(this->m_CPosX,this->m_CPosY);
	this->m_CDataFilteredCoronal->SetFl_Output(this->m_CPosX,this->m_CPosY);

	this->m_CDataSegmentedAxial->SetFl_Output(this->m_CSPosX,this->m_CSPosY);
	this->m_CDataSegmentedSaggittal->SetFl_Output(this->m_CSPosX,this->m_CSPosY);
	this->m_CDataSegmentedCoronal->SetFl_Output(this->m_CSPosX,this->m_CSPosY);

	this->m_CDataFilteredAxial->EnableVOI();
	this->m_CDataFilteredAxial->SetSaggittalViewer(this->m_CDataFilteredSaggittal);
	this->m_CDataFilteredAxial->SetCoronalViewer(this->m_CDataFilteredCoronal);

	

	this->m_C3ViewRender->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());  
	this->m_C3VOIViewRender->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());  
	this->m_CRender->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());  
	this->m_CRay->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());  


	this->SetProgress(this->m_MarchingCubes,"Generating surface....",this->m_CProgressRender);
	this->m_MarchingCubes->SetValue(1,255);
	this->m_MarchingCubes->SetInput(this->m_ResliceSAxial->GetOutput());
	this->m_MarchingCubes->ComputeScalarsOff();
	this->m_MarchingCubes->ComputeNormalsOff();

	this->SetProgress(this->m_Decimate,"Decimating....",this->m_CProgressRender);
	this->m_Decimate->SetFeatureAngle(45);
	this->m_Decimate->SetTargetReduction(0.5);
	this->m_Decimate->SetInput(this->m_MarchingCubes->GetOutput());

	this->SetProgress(this->m_SmoothPolyDataFilter,"Smoothing....",this->m_CProgressRender);
	this->m_SmoothPolyDataFilter->SetInput(this->m_Decimate->GetOutput());
	this->m_SmoothPolyDataFilter->SetRelaxationFactor(0.5);
    this->m_SmoothPolyDataFilter->SetNumberOfIterations(5);

	this->SetProgress(this->m_PolyDataConnectivityFilter,"Extracting max region....",this->m_CProgressRender);
	this->m_PolyDataConnectivityFilter->SetInput(this->m_SmoothPolyDataFilter->GetOutput());
	this->m_PolyDataConnectivityFilter->SetExtractionModeToLargestRegion();
	
	this->SetProgress(this->m_CleanPolyData,"Cleaning....",this->m_CProgressRender);
	this->m_CleanPolyData->SetInput(this->m_PolyDataConnectivityFilter->GetOutput());

	this->SetProgress(this->m_PolyDataNormals,"Generating normals....",this->m_CProgressRender);
	this->m_PolyDataNormals->SetInput(this->m_CleanPolyData->GetOutput());

	this->m_CRender->SetInput(this->m_PolyDataNormals->GetOutput());

	this->m_VRMLExporter->SetInput(this->m_CRender->m_RenderWindow);
}

US3D::~US3D()
{
	
	this->m_ImageFlipFilter->Delete();
	this->m_VOI->Delete();
	this->m_Threshold->Delete();
	this->m_ResliceAxial->Delete();
	this->m_ResliceSaggittal->Delete();
	this->m_ResliceCoronal->Delete();
	this->m_ResliceSAxial->Delete();
	this->m_ResliceSSaggittal->Delete();
	this->m_ResliceSCoronal->Delete();
	this->m_MarchingCubes->Delete();
	this->m_SmoothPolyDataFilter->Delete();
	this->m_PolyDataConnectivityFilter->Delete();
	this->m_Decimate->Delete();
	this->m_VRMLExporter->Delete();
}

void US3D::Load() {
	
  char * filename = fl_file_chooser("DICOM 3D filename","*.spf","");

  if( !filename )
  {
    return;
  }

  this->m_CImagesTab->activate();
  this->m_CBrightness->activate();
  this->m_CVolumeRender->activate();
  this->m_CVolumeResolution->activate();
  this->m_C3DTab->activate();

  sprintf(this->tmpString,"%s [%s]","US3D 1.3",filename);
  this->m_CWnd->label(this->tmpString);
  


  // Carico il file spf..../////////////////////////
  this->m_Reader->SetFileName(filename);
  this->m_Reader->Init();
  this->m_Reader->Load();
  /////////////////////////////////////////////////



  this->m_CVolumeResolution->maximum(this->m_Reader->m_volumeDepth*this->m_Reader->m_voxelDepth);
  // Visualizzo le informazione relative all'header del file/////////////////////////
  char *value=new char[20];

  sprintf(value,"%i",this->m_Reader->m_volumeWidth);
  this->m_CVolumeWidthText->value(value);
  sprintf(value,"%i",this->m_Reader->m_volumeHeight);
  this->m_CVolumeHeightText->value(value);
  sprintf(value,"%i",this->m_Reader->m_volumeDepth);
  this->m_CVolumeDepthText->value(value);

  sprintf(value,"%.4f",this->m_Reader->m_voxelWidth);
  this->m_CVoxelWidthText->value(value);
  sprintf(value,"%.4f",this->m_Reader->m_voxelHeight);
  this->m_CVoxelHeightText->value(value);
  sprintf(value,"%.4f",this->m_Reader->m_voxelDepth);
  this->m_CVoxelDepthText->value(value);

  sprintf(value,"%.2f mm³",this->m_Reader->m_volumeWidth*this->m_Reader->m_volumeHeight*
					   this->m_Reader->m_volumeDepth*this->m_Reader->m_voxelWidth*
					   this->m_Reader->m_voxelHeight*this->m_Reader->m_voxelDepth/1000);
  this->m_CVolumeMeasure->value(value);

  Fl::flush();
  /////////////////////////////////////////////////////////////////////////////////////

  this->Init();
  
  // Inizializzo il Volume Of Interest ////////////////////////////////////////////////
  this->m_VOI->SetVOI(0,this->m_Reader->m_volumeWidth-1,
					  0,this->m_Reader->m_volumeHeight-1,
					  0,this->m_Reader->m_volumeDepth-1);
  /////////////////////////////////////////////////////////////////////////////////////
  

  // Inizializzo le 3 slide control filtered and segmented ///////////////////////////
  this->m_CWidth->maximum(this->m_Reader->m_volumeWidth-1);	
  this->m_CHeight->maximum(this->m_Reader->m_volumeHeight-1);	
  this->m_CDepth->maximum(this->m_Reader->m_volumeDepth-1);	
  this->m_CHeight->value(0);
  this->m_CWidth->value(0);
  this->m_CDepth->value(0);

  this->m_CSWidth->maximum(this->m_Reader->m_volumeWidth-1);	
  this->m_CSHeight->maximum(this->m_Reader->m_volumeHeight-1);	
  this->m_CSDepth->maximum(this->m_Reader->m_volumeDepth-1);	
  this->m_CSHeight->value(0);
  this->m_CSWidth->value(0);
  this->m_CSDepth->value(0);
  /////////////////////////////////////////////////////////////////////////////////////

  // Inizializzo i 2 controlli roll per selezionare il VOI ////////////////////////////
  this->m_CDepthUp->maximum(this->m_Reader->m_volumeDepth-1);
  this->m_CDepthDown->maximum(this->m_Reader->m_volumeDepth-1);
  this->m_CDepthUp->value(this->m_Reader->m_volumeDepth-1);
  /////////////////////////////////////////////////////////////////////////////////////

  // Collego le immagini segmentate ai controlli di vista /////////////////////////////
  this->m_CDataSegmentedAxial->SetInput(this->m_ResliceSAxial->GetOutput());
  this->m_CDataSegmentedCoronal->SetInput(this->m_ResliceSCoronal->GetOutput());
  this->m_CDataSegmentedSaggittal->SetInput(this->m_ResliceSSaggittal->GetOutput());
  /////////////////////////////////////////////////////////////////////////////////////

  // Collego le immagini filtrate ai controlli di vista ///////////////////////////////
  this->m_CDataFilteredAxial->SetSize(this->m_Reader->m_volumeWidth,this->m_Reader->m_volumeHeight);
  this->m_CDataFilteredSaggittal->SetSize(this->m_Reader->m_volumeDepth,this->m_Reader->m_volumeHeight);
  this->m_CDataFilteredCoronal->SetSize(this->m_Reader->m_volumeWidth,this->m_Reader->m_volumeDepth);
  this->m_CDataFilteredAxial->SetInput(this->m_ResliceAxial->GetOutput());
  this->m_CDataFilteredSaggittal->SetInput(this->m_ResliceSaggittal->GetOutput());
  this->m_CDataFilteredCoronal->SetInput(this->m_ResliceCoronal->GetOutput());
  /////////////////////////////////////////////////////////////////////////////////////


  // Collego il VOI alla vista 3D.. //////////////////////////////////////////////////
  this->m_C3VOIViewRender->SetSize(this->m_Reader->m_volumeWidth,
								   this->m_Reader->m_volumeHeight,
								   this->m_Reader->m_volumeDepth);
  //this->m_C3VOIViewRender->SetInput(this->m_VOI->GetOutput());
  this->m_C3VOIViewRender->SetInput(this->m_Threshold->GetOutput());
  /////////////////////////////////////////////////////////////////////////////////////


  // Collego il VOI alla vista 3D.. //////////////////////////////////////////////////
  this->m_C3ViewRender->SetSize(this->m_Reader->m_volumeWidth,
			    				this->m_Reader->m_volumeHeight,
								this->m_Reader->m_volumeDepth);
  this->m_C3ViewRender->SetInput(this->m_ImageGaussianSmoothFilter->GetOutput());
  /////////////////////////////////////////////////////////////////////////////////////
   

  this->m_CDataFilteredAxial->SetSlide(0);
  this->m_CDataFilteredSaggittal->SetSlide(0);
  this->m_CDataFilteredCoronal->SetSlide(0);
  

  this->m_CDataFilteredAxial->draw();
  this->m_CDataFilteredSaggittal->draw();
  this->m_CDataFilteredCoronal->draw();

  this->m_C3ViewRender->m_Renderer->ResetCamera();
  this->m_C3ViewRender->draw(); 


  this->m_CRay->SetVolumeSize(this->m_Reader->m_volumeWidth,
			    			  this->m_Reader->m_volumeHeight,
							  this->m_Reader->m_volumeDepth);
  this->m_CRay->SetVoxelSize(this->m_Reader->m_voxelWidth,
			    			 this->m_Reader->m_voxelHeight,
							 this->m_Reader->m_voxelDepth);
  this->m_CRay->SetInput(this->m_ImageGaussianSmoothFilter->GetOutput());
  this->m_CRay->m_Renderer->ResetCamera();
  this->m_CRay->draw();


  this->m_CDataSegmentedAxial->draw();
  this->m_CDataSegmentedCoronal->draw();
  this->m_CDataSegmentedSaggittal->draw();
  this->m_C3VOIViewRender->m_Renderer->ResetCamera();
  this->m_C3VOIViewRender->draw(); 

  this->m_3D->hide();
}

void US3D::SaveVRML() {

	this->m_CRender->m_Renderer->GetActiveCamera()->SetFocalPoint(0,0,0);
	this->m_CRender->m_Renderer->GetActiveCamera()->SetPosition(0,0,1);
	this->m_CRender->m_Renderer->GetActiveCamera()->SetViewUp(0,1,0);
	this->m_CRender->m_Renderer->ResetCamera();

	this->m_CRender->draw();

	char * filename = fl_file_chooser("WRML 3D filename","*.wrl","");

	if( !filename )
	{
	    return;
	}

	strcat(filename,".wrl");
	this->m_VRMLExporter->SetFileName(filename);
	this->m_VRMLExporter->Write();
}

void US3D::SaveVTK() {

	this->m_CRender->draw();

	char * filename = fl_file_chooser("VTK Binary 3D filename","*.vtk","");

	if( !filename )
	{
	    return;
	}

	strcat(filename,".vtk");

	this->m_Writer->SetInput(this->m_PolyDataNormals->GetOutput());
	this->m_Writer->SetFileName(filename);
	this->m_Writer->SetFileTypeToBinary();
	this->m_Writer->Write();
}

void US3D::SetBrightness() {
	int Brightness=this->m_CBrightness->value();
    this->m_CDataFilteredAxial->SetBrightness(Brightness);
	this->m_CDataFilteredAxial->SetBrightness(Brightness);
    this->m_CDataFilteredCoronal->SetBrightness(Brightness);
    this->m_CDataFilteredSaggittal->SetBrightness(Brightness);
}

void US3D::SetSmoothing() {
	this->m_SmoothPolyDataFilter->SetRelaxationFactor(this->m_CSmoothing->value());
}

void US3D::SetDecimate() {
	this->m_Decimate->SetTargetReduction(this->m_CDecimate->value());
}


void US3D::SetFeatureAngle() {
	this->m_Decimate->SetFeatureAngle(this->m_FeatureAngle->value());
}


void US3D::SurfaceRender() {

	this->m_CRender->m_Renderer->ResetCamera();
	
	this->m_3D->show();
	this->m_CRender->show();
	this->m_CRender->draw();

	vtkPolyData *d=this->m_CleanPolyData->GetOutput();


	char *value=new char[20];

	sprintf(value,"%i",d->GetNumberOfPoints());
	this->m_CSurfaceNVertexText->value(value);

	sprintf(value,"%i",d->GetNumberOfCells());
	this->m_CSurfaceNPolygonText->value(value);

	Fl::flush();

	this->m_CSaveVRML->activate();
	this->m_CSaveVTK->activate();
}

void US3D::RayRender() {
}

void US3D::SetOpacity() {
	
	this->m_CRender->m_Actor->GetProperty()->SetOpacity(this->m_COpacity->value());
	this->m_CRender->draw();
}

void US3D::SetFilteredSliceDepth() {
	
	int w=this->m_CWidth->value();
	int h=this->m_CHeight->value();
	int d=this->m_CDepth->value();
	
    this->m_CDataFilteredAxial->SetSlide(d);
	if (this->m_CMoveSlice->value()==1)
		this->m_C3ViewRender->SetSlide(h,w,d);

	this->m_CDataFilteredAxial->Render();
}

void US3D::SetFilteredSliceWidth() {

	int w=this->m_CWidth->value();
	int h=this->m_CHeight->value();
	int d=this->m_CDepth->value();

	this->m_CDataFilteredSaggittal->SetSlide(w);
	if (this->m_CMoveSlice->value()==1)
		this->m_C3ViewRender->SetSlide(h,w,d);

	this->m_CDataFilteredSaggittal->Render();
}

void US3D::SetFilteredSliceHeight() {

	int w=this->m_CWidth->value();
	int h=this->m_CHeight->value();
	int d=this->m_CDepth->value();
	
	this->m_CDataFilteredCoronal->SetSlide(h);
	if (this->m_CMoveSlice->value()==1)
		this->m_C3ViewRender->SetSlide(h,w,d);

	this->m_CDataFilteredCoronal->Render();
}



void US3D::SetRadius(float value) {

	float x,y,z;
	if (this->m_CRadiusLock->value()==1)
	{
		x=y=z=value;
		this->m_CRadiusX->value(value);
		this->m_CRadiusY->value(value);
		this->m_CRadiusZ->value(value);
	}
	else
	{
	    x=this->m_CRadiusX->value();
		y=this->m_CRadiusY->value();
		z=this->m_CRadiusZ->value();
	}
	this->m_ImageGaussianSmoothFilter->SetRadiusFactors(x,y,z);
	this->m_CDataFilteredAxial->Render();
	this->m_CDataFilteredSaggittal->Render();
	this->m_CDataFilteredCoronal->Render();
	this->m_C3ViewRender->Render();
}

void US3D::SetStandardDeviation(float value) {

	float x,y,z;
	if (this->m_CStandardDeviationLock->value()==1)
	{
		x=y=z=value;
		this->m_CSDeviationX->value(value);
		this->m_CSDeviationY->value(value);
		this->m_CSDeviationZ->value(value);
	}
	else
	{
	    x=this->m_CSDeviationX->value();
		y=this->m_CSDeviationY->value();
		z=this->m_CSDeviationZ->value();
	}
	this->m_ImageGaussianSmoothFilter->SetStandardDeviation(x,y,z);
	this->m_CDataFilteredAxial->Render();
	this->m_CDataFilteredSaggittal->Render();
	this->m_CDataFilteredCoronal->Render();
}

void US3D::SetVOIDepthUp() {
	this->m_CDataFilteredCoronal->SetUp(this->m_CDepthUp->value());
	this->m_CDataFilteredSaggittal->SetRight(this->m_CDepthUp->value());
}

void US3D::SetVOIDepthDown() {
	this->m_CDataFilteredCoronal->SetDown(this->m_CDepthDown->value());
	this->m_CDataFilteredSaggittal->SetLeft(this->m_CDepthDown->value());
}

void US3D::ExtractVOI() {
	this->m_VOI_Xi=this->m_CDataFilteredAxial->m_Left;
	this->m_VOI_Xf=this->m_CDataFilteredAxial->m_Right;
	this->m_VOI_Yi=this->m_CDataFilteredAxial->m_Down;
	this->m_VOI_Yf=this->m_CDataFilteredAxial->m_Up;
	this->m_VOI_Zi=this->m_CDataFilteredCoronal->m_Down;
	this->m_VOI_Zf=this->m_CDataFilteredCoronal->m_Up;

	int Width=this->m_VOI_Xf-this->m_VOI_Xi+1;
	int Height=this->m_VOI_Yf-this->m_VOI_Yi+1;
	int Depth=this->m_VOI_Zf-this->m_VOI_Zi+1;

	this->m_VOI->SetVOI(this->m_VOI_Xi,this->m_VOI_Xf,
						this->m_VOI_Yi,this->m_VOI_Yf,
						this->m_VOI_Zi,this->m_VOI_Zf);

	this->m_C3VOIViewRender->SetSize(Width,Height,Depth);

	this->m_CSWidth->maximum(Width-1);	
	this->m_CSHeight->maximum(Height-1);	
	this->m_CSDepth->maximum(Depth-1);	
	this->m_CSHeight->value(0);
	this->m_CSWidth->value(0);
	this->m_CSDepth->value(0);

    this->m_CDataSegmentedAxial->SetSlide(0);
	this->m_CDataSegmentedSaggittal->SetSlide(0);
	this->m_CDataSegmentedCoronal->SetSlide(0);


	this->m_C3VOIViewRender->m_Renderer->ResetCamera();
	
	this->m_C3VOIViewRender->draw(); 
	this->m_CDataSegmentedAxial->draw();
	this->m_CDataSegmentedSaggittal->draw();
	this->m_CDataSegmentedCoronal->draw();
	

	float c[3];
	float f[3];
	
	this->m_VOI->GetOutput()->GetCenter(c);
	this->m_VOI->GetOutput()->GetOrigin(f);

	this->m_VOI->GetOutput()->SetOrigin(-c[0]+f[0],-c[1]+f[1],-c[2]+f[2]);
	

	char *value=new char[20];

	sprintf(value,"%i",Width);
	this->m_CVOIWidthText->value(value);
	sprintf(value,"%i",Height);
	this->m_CVOIHeightText->value(value);
	  sprintf(value,"%i",Depth);
	this->m_CVOIDepthText->value(value);
	
	sprintf(value,"%.2f mm³",Width*Height*Depth*this->m_Reader->m_voxelWidth*
											this->m_Reader->m_voxelHeight*
											this->m_Reader->m_voxelDepth/1000);
    this->m_CVolumeVOIMeasure->value(value);
}

void US3D::SetSegmentedSliceWidth() {

	int w=this->m_CSWidth->value();
	int h=this->m_CSHeight->value();
	int d=this->m_CSDepth->value();

	this->m_CDataSegmentedSaggittal->SetSlide(w);
	if (this->m_CMoveVOISlice->value()==1)
		this->m_C3VOIViewRender->SetSlide(h,w,d);

	this->m_CDataSegmentedSaggittal->Render();
}

void US3D::SetSegmentedSliceDepth() {

	int w=this->m_CSWidth->value();
	int h=this->m_CSHeight->value();
	int d=this->m_CSDepth->value();
	
    this->m_CDataSegmentedAxial->SetSlide(d);
	if (this->m_CMoveVOISlice->value()==1)
		this->m_C3VOIViewRender->SetSlide(h,w,d);

	this->m_CDataSegmentedAxial->Render();
}

void US3D::SetSegmentedSliceHeight() {

	int w=this->m_CSWidth->value();
	int h=this->m_CSHeight->value();
	int d=this->m_CSDepth->value();
	
	this->m_CDataSegmentedCoronal->SetSlide(h);
	if (this->m_CMoveVOISlice->value()==1)
		this->m_C3VOIViewRender->SetSlide(h,w,d);

	this->m_CDataSegmentedCoronal->Render();
}


void US3D::SetThresholding() {

	int inf=this->m_CThreshInf->value();
	int sup=this->m_CThreshSup->value();
	this->m_Threshold->ThresholdBetween(inf,sup);
	this->m_CDataSegmentedAxial->Render();
	this->m_CDataSegmentedCoronal->Render();
	this->m_CDataSegmentedSaggittal->Render();
	this->m_C3VOIViewRender->draw();
}

void US3D::SetVolumeResolution() {
	this->m_CRay->m_VolumeRayCastMapper->SetSampleDistance(int(this->m_CVolumeResolution->value()));
}

void US3D::VolumeRender() {
	this->m_CRay->draw();
}


void US3D::PlayStopFiltered() {
}

void US3D::PlayStopSegmented() {
}

void US3D::SetControlRay() {
}

void US3D::SetControlRender() {
}



void US3D::Progress(void *arg)
{
	US3D *This=((ProgressArguments*)arg)->This;
	This->ProgressCall(arg);
}

void US3D::ProgressCall(void *arg)
{
	vtkProcessObject *Process=((ProgressArguments*)arg)->Process;
	Fl_Progress *progress=((ProgressArguments*)arg)->progress;
	char *Label=((ProgressArguments*)arg)->Label;
	float value=Process->GetProgress()*100;
	progress->value(value);
	if (value==100)
		sprintf(this->tmpString,"[%s]","Ready");
	else
		sprintf(this->tmpString,"%s [%.2f%%]",Label,value);
		
	progress->label(this->tmpString);
	Fl::flush();
}

void US3D::SetShadingInterpolationMethod() {

	if (this->m_Flat->value()==1)
		this->m_CRender->m_Actor->GetProperty()->SetInterpolationToFlat();
	if (this->m_Gouraund->value()==1)
		this->m_CRender->m_Actor->GetProperty()->SetInterpolationToGouraud();
	if (this->m_Wireframe->value()==1)
		this->m_CRender->m_Actor->GetProperty()->SetRepresentationToWireframe();
	else
		this->m_CRender->m_Actor->GetProperty()->SetRepresentationToSurface();
	this->m_CRender->draw();
}



void US3D::Init()
{
	if (!(this->init)) {
		this->init=true;
		this->m_CDataFilteredAxial->show();
		this->m_CDataFilteredSaggittal->show();
		this->m_CDataFilteredCoronal->show();
	    this->m_C3ViewRender->show();
		this->m_CRay->show();
		this->m_CDataSegmentedAxial->show();
		this->m_CDataSegmentedCoronal->show();
		this->m_CDataSegmentedSaggittal->show();
		this->m_C3VOIViewRender->show();
	}
}

void US3D::SetProgress(vtkProcessObject *process, char *label, Fl_Progress *progress)
{
	ProgressArguments *arg=new ProgressArguments;
	arg->This=this;
	arg->Process=process;
    arg->Label=label;
	arg->progress=progress;
	process->SetProgressMethod(this->Progress,arg);
}

