// vtkFL3ViewRenderWindowInteractor.cpp: implementation of the vtkFL3ViewRenderWindowInteractor class.
//
//////////////////////////////////////////////////////////////////////

#include "vtkFL3ViewRenderWindowInteractor.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

vtkFL3ViewRenderWindowInteractor::vtkFL3ViewRenderWindowInteractor(int x, int y, int w, int h) 
 : vtkFlRenderWindowInteractor(x,y,w,h)
{
	this->m_Renderer=vtkRenderer::New();
	this->m_RenderWindow=vtkRenderWindow::New();
	this->bwLut=vtkLookupTable::New();
	this->hueLut=vtkLookupTable::New();
	this->satLut=vtkLookupTable::New();
	this->m_saggitalSection=vtkExtractVOI::New();
	this->m_axialSection=vtkExtractVOI::New();
	this->m_coronalSection=vtkExtractVOI::New();
	this->m_saggitalTexture=vtkTexture::New();
	this->m_axialTexture=vtkTexture::New();
	this->m_coronalTexture=vtkTexture::New();
	this->m_saggitalPlane=vtkPlaneSource::New();
	this->m_axialPlane=vtkPlaneSource::New();
	this->m_coronalPlane=vtkPlaneSource::New();
	this->m_saggitalMapper=vtkPolyDataMapper::New();
	this->m_axialMapper=vtkPolyDataMapper::New();
	this->m_coronalMapper=vtkPolyDataMapper::New();
	this->m_saggitalActor=vtkActor::New();
	this->m_axialActor=vtkActor::New();
	this->m_coronalActor=vtkActor::New();

	this->m_OutlineSaggittal=vtkOutlineFilter::New();
	this->m_OutlineMapperSaggittal=vtkPolyDataMapper::New();
	this->m_OutlineActorSaggittal=vtkActor::New();

	this->m_OutlineAxial=vtkOutlineFilter::New();
	this->m_OutlineMapperAxial=vtkPolyDataMapper::New();
	this->m_OutlineActorAxial=vtkActor::New();

	this->m_OutlineCoronal=vtkOutlineFilter::New();
	this->m_OutlineMapperCoronal=vtkPolyDataMapper::New();
	this->m_OutlineActorCoronal=vtkActor::New();

	
	this->m_OutlineMapperSaggittal->SetInput(this->m_OutlineSaggittal->GetOutput());
	this->m_OutlineActorSaggittal->SetMapper(this->m_OutlineMapperSaggittal);
	this->m_OutlineActorSaggittal->GetProperty()->SetColor(1,0,0);
	this->m_OutlineSaggittal->SetInput(this->m_saggitalPlane->GetOutput());

	this->m_OutlineMapperAxial->SetInput(this->m_OutlineAxial->GetOutput());
	this->m_OutlineActorAxial->SetMapper(this->m_OutlineMapperAxial);
	this->m_OutlineActorAxial->GetProperty()->SetColor(0,0.56,0);
	this->m_OutlineAxial->SetInput(this->m_axialPlane->GetOutput());

	this->m_OutlineMapperCoronal->SetInput(this->m_OutlineCoronal->GetOutput());
	this->m_OutlineActorCoronal->SetMapper(this->m_OutlineMapperCoronal);
	this->m_OutlineActorCoronal->GetProperty()->SetColor(1,1,0);
	this->m_OutlineCoronal->SetInput(this->m_coronalPlane->GetOutput());


	this->m_Renderer->SetAmbient(100,0,0);
	
	this->m_Renderer->SetBackground(0.1,0.3,0.3);
	this->m_RenderWindow->AddRenderer(this->m_Renderer);
	this->SetRenderWindow(this->m_RenderWindow);
	this->m_RenderWindow->LineSmoothingOn();

	this->bwLut->SetTableRange(0,255);
	this->bwLut->SetSaturationRange(0,0);
	this->bwLut->SetHueRange(0,0);
	this->bwLut->SetValueRange(0,1);
	this->bwLut->Build ();

	this->hueLut->SetTableRange(0,255);
	this->hueLut->SetSaturationRange(0,0);
	this->hueLut->SetHueRange(0,0);
	this->hueLut->SetValueRange(0,1);
	this->hueLut->Build();

	this->satLut->SetTableRange(0,255);
	this->satLut->SetSaturationRange(0,0);
	this->satLut->SetHueRange(0,0);
	this->satLut->SetValueRange(0,1);
	this->satLut->Build();

	

	this->m_saggitalTexture->SetInput(this->m_saggitalSection->GetOutput());
    this->m_saggitalTexture->InterpolateOn();
    //this->m_saggitalTexture->SetLookupTable (this->bwLut);
    //this->m_saggitalTexture->MapColorScalarsThroughLookupTableOn();

	this->m_axialTexture->SetInput(this->m_axialSection->GetOutput());
    this->m_axialTexture->InterpolateOn();
    //this->m_axialTexture->SetLookupTable (this->hueLut);
    //this->m_axialTexture->MapColorScalarsThroughLookupTableOn();

	this->m_coronalTexture->SetInput(this->m_coronalSection->GetOutput());
    this->m_coronalTexture->InterpolateOn();
    //this->m_coronalTexture->SetLookupTable (this->satLut);
    //this->m_coronalTexture->MapColorScalarsThroughLookupTableOn();


	

	this->m_saggitalPlane->SetXResolution(1);
    this->m_saggitalPlane->SetYResolution(1);
	this->m_axialPlane->SetXResolution(1);
    this->m_axialPlane->SetYResolution(1);
	this->m_coronalPlane->SetXResolution(1);
    this->m_coronalPlane->SetYResolution(1);


	this->m_saggitalMapper->SetInput(this->m_saggitalPlane->GetOutput());
	this->m_axialMapper->SetInput(this->m_axialPlane->GetOutput());
	this->m_coronalMapper->SetInput(this->m_coronalPlane->GetOutput());

	
	this->m_saggitalActor->SetMapper(this->m_saggitalMapper);
	this->m_saggitalActor->SetTexture(this->m_saggitalTexture);
	this->m_axialActor->SetMapper(this->m_axialMapper);
	this->m_axialActor->SetTexture(this->m_axialTexture);
	this->m_coronalActor->SetMapper(this->m_coronalMapper);
	this->m_coronalActor->SetTexture(this->m_coronalTexture);

	this->m_Renderer->AddActor(this->m_saggitalActor);
	this->m_Renderer->AddActor(this->m_axialActor);
	this->m_Renderer->AddActor(this->m_coronalActor);
	this->m_Renderer->AddActor(this->m_OutlineActorSaggittal);
	this->m_Renderer->AddActor(this->m_OutlineActorAxial);
	this->m_Renderer->AddActor(this->m_OutlineActorCoronal);
	this->m_Renderer->SetBackground(0.5,0.5,0.7);
}

vtkFL3ViewRenderWindowInteractor::~vtkFL3ViewRenderWindowInteractor()
{
	this->m_Renderer->Delete();
	this->m_RenderWindow->Delete();
	this->bwLut->Delete();
	this->hueLut->Delete();
	this->satLut->Delete();
	this->m_saggitalSection->Delete();
	this->m_axialSection->Delete();
	this->m_coronalSection->Delete();
	this->m_saggitalTexture->Delete();
	this->m_axialTexture->Delete();
	this->m_coronalTexture->Delete();
	this->m_saggitalPlane->Delete();
	this->m_axialPlane->Delete();
	this->m_coronalPlane->Delete();
	this->m_saggitalMapper->Delete();
	this->m_axialMapper->Delete();
	this->m_coronalMapper->Delete();
	this->m_saggitalActor->Delete();
	this->m_axialActor->Delete();
	this->m_coronalActor->Delete();
	this->m_OutlineSaggittal->Delete();
	this->m_OutlineMapperSaggittal->Delete();
	this->m_OutlineActorSaggittal->Delete();
	this->m_OutlineAxial->Delete();
	this->m_OutlineMapperAxial->Delete();
	this->m_OutlineActorAxial->Delete();
	this->m_OutlineCoronal->Delete();
	this->m_OutlineMapperCoronal->Delete();
	this->m_OutlineActorCoronal->Delete();
}



void vtkFL3ViewRenderWindowInteractor::Render()
{
	this->draw();	
}

void vtkFL3ViewRenderWindowInteractor::SetInput(vtkImageData *Input)
{
	this->m_saggitalSection->SetInput(Input);
	this->m_axialSection->SetInput(Input);
	this->m_coronalSection->SetInput(Input);  
}

void vtkFL3ViewRenderWindowInteractor::SetSlide(int h,int w,int d)
{

	this->m_saggitalSection->SetVOI(w,w,0,this->h-1,0,this->d-1);
	this->m_axialSection->SetVOI(0,this->w-1,0,this->h-1,d,d);
	this->m_coronalSection->SetVOI(0,this->w-1,h,h,0,this->d-1);

	this->m_saggitalPlane->SetOrigin(w,0,0);
    this->m_saggitalPlane->SetPoint1(w,this->h-1,0);
    this->m_saggitalPlane->SetPoint2(w,0,this->d-1);
	
	this->m_axialPlane->SetOrigin(0,0,d);
    this->m_axialPlane->SetPoint1(this->w-1,0,d);
    this->m_axialPlane->SetPoint2(0,this->h-1,d);

	this->m_coronalPlane->SetOrigin(0,h,0);
    this->m_coronalPlane->SetPoint1(this->w-1,h,0);
    this->m_coronalPlane->SetPoint2(0,h,this->d-1);

	this->m_saggitalSection->Update();
	this->m_axialSection->Update();
	this->m_coronalSection->Update();

	this->draw();
}

void vtkFL3ViewRenderWindowInteractor::SetSize(int w,int h,int d)
{

	this->w=w;
	this->h=h;
	this->d=d;

	this->m_saggitalSection->SetVOI(0,0,0,this->h,0,this->d);
	this->m_axialSection->SetVOI(0,this->w,0,this->h,0,0);
	this->m_coronalSection->SetVOI(0,this->w,0,0,0,this->d);

	this->m_saggitalPlane->SetOrigin(0.0,0.0,0.0);
    this->m_saggitalPlane->SetPoint1(0.0,this->h,0.0);
    this->m_saggitalPlane->SetPoint2(0.0,0.0,this->d);

	this->m_axialPlane->SetOrigin(0.0,0.0,0.0);
    this->m_axialPlane->SetPoint1(this->w,0.0,0.0);
    this->m_axialPlane->SetPoint2(0.0,this->h,0.0);

	this->m_coronalPlane->SetOrigin(0.0,0.0,0.0);
    this->m_coronalPlane->SetPoint1(this->w,0.0,0.0);
    this->m_coronalPlane->SetPoint2(0.0,0.0,this->d);

}

