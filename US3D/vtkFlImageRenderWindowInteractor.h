#include "vtkFlRenderWindowInteractor.h"
#include "vtkImageViewer.h"
#include "vtkImageData.h"
#include <FL/Fl_Output.H>

#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkPlaneSource.h"

#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkTransform.h"
#include "vtkPolyData.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkActor2D.h"



class vtkFlImageRenderWindowInteractor : public vtkFlRenderWindowInteractor
{
public:
	void SetVOIRect(int left,int up,int right,int down);
	void SetRight(float value);
	void SetLeft(float value);
	void SetDown(float value);
	void SetUp(float value);
	void SetSize(int w,int h);
	void EnableVOI();
	BOOL VOIEnable;
	void SetCoronalViewer(vtkFlImageRenderWindowInteractor *coronal);
	void SetSaggittalViewer(vtkFlImageRenderWindowInteractor *saggittal);
	int m_Left,m_Up,m_Right,m_Down;
	int m_MousePosX;
	int m_MousePosY;
	BOOL m_IsValidSelection;
	Fl_Output * m_PosX;
	Fl_Output * m_PosY;
	int m_ImageH,m_ImageW;
	


	vtkFlImageRenderWindowInteractor *m_Saggittal;
	vtkFlImageRenderWindowInteractor *m_Coronal;

	vtkImageViewer * m_ImageViewer;

	vtkPoints *m_Points;
	vtkCellArray *m_CellArray;
	vtkTransform *m_Transform;
	vtkPolyData *m_PolyData;
	vtkTransformPolyDataFilter *m_TranformPolyDataFilter;
	vtkPolyDataMapper2D *m_PolyDataMapper2D;
	vtkActor2D *m_Actor2D;



	vtkFlImageRenderWindowInteractor(int x, int y, int w, int h);
	virtual ~vtkFlImageRenderWindowInteractor();

	void SetFl_Output(Fl_Output *posX,Fl_Output *posY);
	void SetSlide(int n);
	void SetInput(vtkImageData *Input);
	void SetBrightness(float Level);
	virtual void OnMouseDrag();
	virtual void MouseMove(int x,int y);
	virtual void LeftButtonPress(int x,int y);
	virtual void LeftButtonRelease(int x,int y);
	virtual void RightButtonPress(int x,int y);
	virtual void RightButtonRelease(int x,int y);

};

