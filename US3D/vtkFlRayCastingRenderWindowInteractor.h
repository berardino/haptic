#include "vtkFlRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkVolume.h"
#include "vtkVolumeRayCastMIPFunction.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolumeProperty.h"
#include "vtkPiecewiseFunction.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkAxes.h"
#include "vtkPlaneSource.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeRayCastCompositeFunction.h"


class vtkFlRayCastingRenderWindowInteractor : public vtkFlRenderWindowInteractor  
{
public:
	void SetOrigin(float x0,float y0,float z0);
	void SetVoxelSize(float w,float h,float d);
	void SetVolumeSize(int w,int h,int d);
	void SetInput(vtkImageData *Input);
	vtkRenderWindow * m_RenderWindow;
	vtkRenderer * m_Renderer;
	vtkVolume * m_Volume;
	vtkVolumeRayCastMIPFunction * m_VolumeRayCastMIPFunction;

	vtkVolumeRayCastCompositeFunction *m_VolumeRayCastCompositeFunction;

	vtkVolumeRayCastMapper * m_VolumeRayCastMapper;
	vtkVolumeProperty * m_VolumeProperty;
	vtkPiecewiseFunction * m_PiecewiseFunction;
	vtkColorTransferFunction *m_ColorTransferFunction;

	vtkOutlineFilter  *m_Outline;
	vtkPolyDataMapper *m_OutlineMapper;
	vtkActor *m_OutlineActor;

	vtkAxes *m_Axes;
	vtkPolyDataMapper *m_AxesMapper;
	vtkActor *m_AxesActor;

	vtkPlaneSource *m_SectionPlane;
	vtkPolyDataMapper *m_SectionMapper;
	vtkActor *m_SectionActor;

	vtkFlRayCastingRenderWindowInteractor(int x, int y, int w, int h);
	virtual ~vtkFlRayCastingRenderWindowInteractor();

	virtual void LeftButtonPress(int x,int y);
	virtual void LeftButtonRelease(int x,int y);
	virtual void RightButtonPress(int x,int y);
	virtual void RightButtonRelease(int x,int y);

	float m_voxelW,m_voxelH,m_voxelD;
	int m_volumeW,m_volumeH,m_volumeD;


};
