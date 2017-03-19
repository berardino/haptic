// vtkFlImageRenderWindowInteractor.cpp: implementation of the vtkFlImageRenderWindowInteractor class.
//
//////////////////////////////////////////////////////////////////////

#include "vtkFlImageRenderWindowInteractor.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

vtkFlImageRenderWindowInteractor::vtkFlImageRenderWindowInteractor(int x, int y, int w, int h) 
 : vtkFlRenderWindowInteractor(x,y,w,h)
{
	this->m_ImageViewer=vtkImageViewer::New();
	this->m_Points=vtkPoints::New();
	this->m_CellArray=vtkCellArray::New();
	this->m_Transform=vtkTransform::New();
	this->m_PolyData=vtkPolyData::New();
	this->m_TranformPolyDataFilter=vtkTransformPolyDataFilter::New();
	this->m_PolyDataMapper2D=vtkPolyDataMapper2D::New();
	this->m_Actor2D=vtkActor2D::New();


	this->m_ImageViewer->SetColorWindow(256);
	this->m_ImageViewer->SetColorLevel(128);
	this->SetRenderWindow(this->m_ImageViewer->GetRenderWindow());

	this->m_Left=this->m_Up=this->m_Right=this->m_Down=0;
	this->m_IsValidSelection=FALSE;
	this->m_MousePosX=this->m_MousePosY=0;

	this->m_Points->InsertNextPoint(0,0,0);
	this->m_Points->InsertNextPoint(0,0,0);
	this->m_Points->InsertNextPoint(0,0,0);
	this->m_Points->InsertNextPoint(0,0,0);

	this->m_CellArray->InsertNextCell(4);
	this->m_CellArray->InsertCellPoint(0);
	this->m_CellArray->InsertCellPoint(1);
	this->m_CellArray->InsertCellPoint(2);
	this->m_CellArray->InsertCellPoint(3);

	this->m_PolyData->SetPoints(this->m_Points);
	this->m_PolyData->SetPolys(this->m_CellArray);

	this->m_TranformPolyDataFilter->SetTransform(this->m_Transform);
	this->m_TranformPolyDataFilter->SetInput(this->m_PolyData);

	this->m_PolyDataMapper2D->SetInput(this->m_TranformPolyDataFilter->GetOutput());

	this->m_Actor2D->SetMapper(this->m_PolyDataMapper2D);
	this->m_Actor2D->GetProperty()->SetColor(0.7,1,0);
	this->m_Actor2D->GetProperty()->SetLineWidth(1);
	this->m_Actor2D->GetProperty()->SetDisplayLocationToForeground();
	this->m_Actor2D->GetProperty()->SetOpacity(0.2);

	this->m_ImageViewer->GetRenderer()->AddActor2D(this->m_Actor2D);

	this->VOIEnable=FALSE;
}

vtkFlImageRenderWindowInteractor::~vtkFlImageRenderWindowInteractor()
{
	this->m_ImageViewer->Delete();
	this->m_Points->Delete();
	this->m_CellArray->Delete();
	this->m_Transform->Delete();
	this->m_PolyData->Delete();
	this->m_TranformPolyDataFilter->Delete();
	this->m_PolyDataMapper2D->Delete();
	this->m_Actor2D->Delete();
}

void vtkFlImageRenderWindowInteractor::LeftButtonPress(int x, int y)
{
	if (this->VOIEnable==TRUE) 
		{
			this->m_Left=this->m_MousePosX;
			this->m_Up=this->m_MousePosY;
		}
}

void vtkFlImageRenderWindowInteractor::RightButtonPress(int x, int y)
{
}

void  vtkFlImageRenderWindowInteractor::LeftButtonRelease(int x, int y)
{
}

void vtkFlImageRenderWindowInteractor::RightButtonRelease(int x, int y)
{
	if (this->VOIEnable==TRUE)
	{
		this->m_IsValidSelection=FALSE;
		this->m_Points->SetPoint(0,0,0,0);
		this->m_Points->SetPoint(1,0,0,0);
		this->m_Points->SetPoint(2,0,0,0);
		this->m_Points->SetPoint(3,0,0,0);
		this->m_Points->Modified();
		this->Render();
	}
}

void vtkFlImageRenderWindowInteractor::MouseMove(int x, int y)
{
	this->m_MousePosX=x;
	this->m_MousePosY=this->h()-1-y;
	char *value=new char[20];
	sprintf(value,"%i",this->m_MousePosX);
	this->m_PosX->value(value);
	sprintf(value,"%i",this->m_MousePosY);
	this->m_PosY->value(value);
}

void vtkFlImageRenderWindowInteractor::SetBrightness(float Level)
{
	this->m_ImageViewer->SetColorLevel(Level);
	this->Render();
}

void vtkFlImageRenderWindowInteractor::SetInput(vtkImageData *Input)
{	
	this->m_ImageViewer->SetInput(Input);
}

void vtkFlImageRenderWindowInteractor::SetSlide(int n)
{
	this->m_ImageViewer->SetZSlice(n);
}

void vtkFlImageRenderWindowInteractor::SetFl_Output(Fl_Output *posX, Fl_Output *posY)
{
	this->m_PosX=posX;
	this->m_PosY=posY;
}

void vtkFlImageRenderWindowInteractor::OnMouseDrag()
{
	if (this->VOIEnable==TRUE)
	{
		this->m_Right=this->m_MousePosX;
		this->m_Down=this->m_MousePosY;
		this->m_IsValidSelection=TRUE;
		this->SetVOIRect(this->m_Left,this->m_Up,this->m_Right,this->m_Down);
		this->m_Saggittal->SetVOIRect(0,this->m_Up,this->m_Saggittal->m_ImageW,this->m_Down);
		this->m_Coronal->SetVOIRect(this->m_Left,this->m_Coronal->m_ImageH,this->m_Right,0);
	}
}



void vtkFlImageRenderWindowInteractor::SetSaggittalViewer(vtkFlImageRenderWindowInteractor *saggittal)
{
	this->m_Saggittal=saggittal;
}

void vtkFlImageRenderWindowInteractor::SetCoronalViewer(vtkFlImageRenderWindowInteractor *coronal)
{
	this->m_Coronal=coronal;
}

void vtkFlImageRenderWindowInteractor::EnableVOI()
{
	this->VOIEnable=TRUE;
}

void vtkFlImageRenderWindowInteractor::SetSize(int w, int h)
{
 this->m_ImageW=w;
 this->m_ImageH=h;
}

void vtkFlImageRenderWindowInteractor::SetUp(float value)
{
	this->m_Up=int(value);
	this->SetVOIRect(this->m_Left,this->m_Up,this->m_Right,this->m_Down);
}

void vtkFlImageRenderWindowInteractor::SetDown(float value)
{
	this->m_Down=int(value);
	this->SetVOIRect(this->m_Left,this->m_Up,this->m_Right,this->m_Down);
}

void vtkFlImageRenderWindowInteractor::SetLeft(float value)
{
	this->m_Left=int(value);
	this->SetVOIRect(this->m_Left,this->m_Up,this->m_Right,this->m_Down);
}

void vtkFlImageRenderWindowInteractor::SetRight(float value)
{
	this->m_Right=int(value);
	this->SetVOIRect(this->m_Left,this->m_Up,this->m_Right,this->m_Down);
}

void vtkFlImageRenderWindowInteractor::SetVOIRect(int left, int up, int right, int down)
{
	this->m_Left=left;
	this->m_Up=up;
	this->m_Right=right;
	this->m_Down=down;
	this->m_Points->SetPoint(0,left,up,0);
	this->m_Points->SetPoint(1,right,up,0);
	this->m_Points->SetPoint(2,right,down,0);
	this->m_Points->SetPoint(3,left,down,0);
	this->m_Points->Modified();
	this->Render();
}
