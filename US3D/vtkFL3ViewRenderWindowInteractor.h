#include "vtkFlRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkLookupTable.h"
#include "vtkExtractVOI.h"
#include "vtkTexture.h"
#include "vtkPlaneSource.h"
#include "vtkOutlineFilter.h"

class vtkFL3ViewRenderWindowInteractor : public vtkFlRenderWindowInteractor  
{
public:
	void SetSize(int w,int h,int d);

	void SetSlide(int h,int w,int d);
	void SetInput(vtkImageData *Input);
	void Render();

	vtkRenderWindow * m_RenderWindow;
	vtkRenderer * m_Renderer;

	int h,w,d;

	vtkFL3ViewRenderWindowInteractor(int x, int y, int w, int h);
	virtual ~vtkFL3ViewRenderWindowInteractor();

	vtkPlaneSource *m_saggitalPlane;
	vtkPlaneSource *m_axialPlane;
	vtkPlaneSource *m_coronalPlane;

	vtkExtractVOI *m_saggitalSection;
	vtkExtractVOI *m_axialSection;
	vtkExtractVOI *m_coronalSection;

	vtkTexture *m_saggitalTexture;
	vtkTexture *m_axialTexture;
	vtkTexture *m_coronalTexture;

	vtkPolyDataMapper *m_saggitalMapper;
	vtkPolyDataMapper *m_axialMapper;
	vtkPolyDataMapper *m_coronalMapper;

	vtkActor *m_saggitalActor;
	vtkActor *m_axialActor;
	vtkActor *m_coronalActor;

	vtkLookupTable *bwLut;
	vtkLookupTable *hueLut;
	vtkLookupTable *satLut;

	vtkOutlineFilter  *m_OutlineSaggittal;
	vtkPolyDataMapper *m_OutlineMapperSaggittal;
	vtkActor *m_OutlineActorSaggittal;

	vtkOutlineFilter  *m_OutlineAxial;
	vtkPolyDataMapper *m_OutlineMapperAxial;
	vtkActor *m_OutlineActorAxial;

	vtkOutlineFilter  *m_OutlineCoronal;
	vtkPolyDataMapper *m_OutlineMapperCoronal;
	vtkActor *m_OutlineActorCoronal;

};