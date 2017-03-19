// vtkFlRender.cpp: implementation of the vtkFlRender class.
//
//////////////////////////////////////////////////////////////////////

#include "vtkFlRender.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

vtkFlRender::vtkFlRender(int x, int y, int w, int h) 
 : vtkFlRenderWindowInteractor(x,y,w,h)
{
	this->m_JPGReader=vtkJPEGReader::New();
	this->m_Texture=vtkTexture::New();
	

	this->m_Renderer = vtkRenderer::New();
	this->m_Actor=vtkActor::New();
	this->m_DataSetMapper=vtkDataSetMapper::New();
    this->m_RenderWindow = vtkRenderWindow::New();

	this->m_JPGReader->SetFileName("texture.jpg");
	this->m_Texture->SetInput(this->m_JPGReader->GetOutput());
	this->m_Texture->InterpolateOn();
	this->m_Texture->RepeatOn();
	
    this->m_RenderWindow->AddRenderer(this->m_Renderer);
	this->SetRenderWindow(this->m_RenderWindow);
	this->m_Actor->SetMapper(this->m_DataSetMapper);
//	this->m_Actor->SetTexture(this->m_Texture);
	this->m_Renderer->AddActor(this->m_Actor);
	this->m_Renderer->SetBackground(0.5,0.5,0.7);
	this->m_Renderer->SetAmbient(0.7,0,0);

    this->m_Actor->GetProperty()->SetAmbient(0.2);
}

vtkFlRender::~vtkFlRender()
{
	this->m_Renderer->Delete();
	this->m_Actor->Delete();
	this->m_DataSetMapper->Delete();
    this->m_RenderWindow->Delete();
}

void vtkFlRender::SetInput(vtkDataSet *Input)
{
	this->m_DataSetMapper->SetInput(Input);	
}
