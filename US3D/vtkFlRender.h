#include "vtkFlRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkDataSet.h"
#include "vtkDataSetMapper.h"
#include "vtkTextureMapToCylinder.h"
#include "vtkJPEGReader.h"
#include "vtkTexture.h"



class vtkFlRender : public vtkFlRenderWindowInteractor  
{
public:
	void SetInput(vtkDataSet *Input);
	vtkRenderWindow * m_RenderWindow;
	vtkRenderer * m_Renderer;
	vtkDataSetMapper *m_DataSetMapper;
    vtkActor *m_Actor;
	vtkJPEGReader *m_JPGReader;
	vtkTexture *m_Texture;
	
   	vtkFlRender(int x, int y, int w, int h) ;
	virtual ~vtkFlRender();

};
