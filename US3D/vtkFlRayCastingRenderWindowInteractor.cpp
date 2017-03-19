// vtkFlRayCastingRenderWindowInteractor.cpp: implementation of the vtkFlRayCastingRenderWindowInteractor class.
//
//////////////////////////////////////////////////////////////////////

#include "vtkFlRayCastingRenderWindowInteractor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

vtkFlRayCastingRenderWindowInteractor::vtkFlRayCastingRenderWindowInteractor(int x, int y, int w, int h) 
 : vtkFlRenderWindowInteractor(x,y,w,h)
{
	this->m_PiecewiseFunction = vtkPiecewiseFunction::New();
	this->m_VolumeProperty = vtkVolumeProperty::New();
	this->m_VolumeRayCastMapper = vtkVolumeRayCastMapper::New();
	this->m_VolumeRayCastMIPFunction = vtkVolumeRayCastMIPFunction::New();
	this->m_Volume = vtkVolume::New();
	this->m_Renderer = vtkRenderer::New();
	this->m_RenderWindow = vtkRenderWindow::New();
	this->m_VolumeRayCastCompositeFunction=vtkVolumeRayCastCompositeFunction::New();
	this->m_ColorTransferFunction=vtkColorTransferFunction::New();

	this->m_Outline=vtkOutlineFilter::New();
	this->m_OutlineMapper=vtkPolyDataMapper::New();
	this->m_OutlineActor=vtkActor::New();

	this->m_Axes=vtkAxes::New();
	this->m_AxesMapper=vtkPolyDataMapper::New();
	this->m_AxesActor=vtkActor::New();

	this->m_SectionPlane=vtkPlaneSource::New();
	this->m_SectionMapper=vtkPolyDataMapper::New();
	this->m_SectionActor=vtkActor::New();

	/*this->m_PiecewiseFunction->AddPoint(0,0.15);
	this->m_PiecewiseFunction->AddPoint(255,0.4);*/
	//this->m_PiecewiseFunction->AddSegment(0,0.1,255,0.2);

	for (float i=0;i<=255;i++)
		this->m_ColorTransferFunction->AddRGBPoint(int(i),i/255,i/255,i/255);

	this->m_Renderer->SetAmbient(1,1,1);
	this->m_Renderer->SetBackground(0.5,0.5,0.7);
	/*this->m_VolumeProperty->ShadeOn();
    this->m_VolumeProperty->SetShade(0.1);*/
    //this->m_VolumeProperty->SetAmbient(1);
    /*this->m_VolumeProperty->SetDiffuse(1);
    this->m_VolumeProperty->SetSpecular(1);
    this->m_VolumeProperty->SetSpecularPower(50);*/


	//this->m_VolumeProperty->SetScalarOpacity(this->m_PiecewiseFunction);
	this->m_VolumeProperty->SetColor(this->m_ColorTransferFunction);

	//this->m_VolumeRayCastMapper->SetSampleDistance(5);
	this->m_VolumeRayCastMapper->SetVolumeRayCastFunction(this->m_VolumeRayCastMIPFunction);
//	this->m_VolumeRayCastCompositeFunction->SetCompositeMethodToClassifyFirst();
//	this->m_VolumeRayCastMapper->SetVolumeRayCastFunction(this->m_VolumeRayCastCompositeFunction);
		
	this->m_Volume->SetMapper(this->m_VolumeRayCastMapper);
	this->m_Volume->SetProperty(this->m_VolumeProperty);

    this->m_RenderWindow->AddRenderer(this->m_Renderer);

	this->SetRenderWindow(this->m_RenderWindow);
	this->m_RenderWindow->LineSmoothingOn();

	this->m_Renderer->AddProp(this->m_Volume);	

	
	this->m_OutlineMapper->SetInput(this->m_Outline->GetOutput());
	this->m_OutlineActor->SetMapper(this->m_OutlineMapper);
	
	this->m_Renderer->AddProp(this->m_OutlineActor);

	this->m_Axes->SymmetricOn();
	this->m_AxesMapper->SetInput(this->m_Axes->GetOutput());
	this->m_AxesActor->SetMapper(this->m_AxesMapper);
	this->m_Renderer->AddProp(this->m_AxesActor);
	

	/*this->m_SectionPlane->SetXResolution(1);
    this->m_SectionPlane->SetYResolution(1);

	this->m_SectionMapper->SetInput(this->m_SectionPlane->GetOutput());
	this->m_SectionActor->SetMapper(this->m_SectionMapper);

	this->m_SectionPlane->SetOrigin(50,0,0);
    this->m_SectionPlane->SetPoint1(50,50,0);
    this->m_SectionPlane->SetPoint2(50,0,50);

	this->m_SectionActor->GetProperty()->SetOpacity(0.1);
	this->m_SectionActor->GetProperty()->SetColor(1,0,0);

//	this->m_Renderer->AddActor(this->m_SectionActor);*/


}

vtkFlRayCastingRenderWindowInteractor::~vtkFlRayCastingRenderWindowInteractor()
{
	this->m_PiecewiseFunction->Delete();
	this->m_VolumeProperty->Delete();
	this->m_VolumeRayCastMapper->Delete();
	this->m_VolumeRayCastMIPFunction->Delete();
	this->m_Volume->Delete();
	this->m_Renderer->Delete();
	this->m_RenderWindow->Delete();
	this->m_Outline->Delete();
	this->m_OutlineMapper->Delete();
	this->m_OutlineActor->Delete();
	this->m_Axes->Delete();
	this->m_AxesMapper->Delete();
	this->m_AxesActor->Delete();
	this->m_SectionPlane->Delete();
	this->m_SectionMapper->Delete();
	this->m_SectionActor->Delete();
}

void vtkFlRayCastingRenderWindowInteractor::SetInput(vtkImageData *Input)
{
	this->m_VolumeRayCastMapper->SetInput(Input);
	this->m_Outline->SetInput(Input);
	float w=(float(this->m_volumeW)*this->m_voxelW)/2;
	float h=(float(this->m_volumeH)*this->m_voxelH)/2;
	float d=(float(this->m_volumeD)*this->m_voxelD)/2;
	this->m_Axes->SetScaleFactor(w*1.5);
	this->m_Axes->SetOrigin(w,h,d);
}



void vtkFlRayCastingRenderWindowInteractor::SetVolumeSize(int w, int h, int d)
{
	this->m_volumeW=w;
	this->m_volumeH=h;
	this->m_volumeD=d;
	this->m_Axes->SetScaleFactor(w*1.5);
}

void vtkFlRayCastingRenderWindowInteractor::SetVoxelSize(float w, float h, float d)
{
	this->m_voxelW=w;
	this->m_voxelH=h;
	this->m_voxelD=d;
}

void  vtkFlRayCastingRenderWindowInteractor::LeftButtonRelease(int x, int y)
{
	this->m_RenderWindow->SetDesiredUpdateRate(0);
	this->draw();
}

void  vtkFlRayCastingRenderWindowInteractor::LeftButtonPress(int x, int y)
{
	this->m_RenderWindow->SetDesiredUpdateRate(25);
}

void vtkFlRayCastingRenderWindowInteractor::RightButtonPress(int x, int y)
{
	this->m_RenderWindow->SetDesiredUpdateRate(25);
}

void  vtkFlRayCastingRenderWindowInteractor::RightButtonRelease(int x, int y)
{
	this->m_RenderWindow->SetDesiredUpdateRate(0);
	this->draw();
}

void vtkFlRayCastingRenderWindowInteractor::SetOrigin(float x0, float y0, float z0)
{
	float wt=((float(this->m_volumeW)*this->m_voxelW)/2);
	float ht=((float(this->m_volumeH)*this->m_voxelH)/2);
	float dt=((float(this->m_volumeD)*this->m_voxelD)/2);
	this->m_Axes->SetScaleFactor((wt)*1.5);
}
