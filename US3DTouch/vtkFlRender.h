#include "vtkFlRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkPointSet.h"
#include "vtkDataSetMapper.h"
#include "vtkTextureMapToCylinder.h"
#include "vtkJPEGReader.h"
#include "vtkTexture.h"
#include "vtkSphereSource.h"
#include "vtkLineSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkOBBTree.h"


class vtkFlRender : public vtkFlRenderWindowInteractor  
{
public:
	int m_Level;
	int m_MaxLevel;
	void SetViewOBBTreeLevel(int level);
	void SetProxyRadius(float radius);
	void SetInput(vtkPointSet *Input);

	vtkRenderWindow * m_RenderWindow;
	vtkRenderer * m_Renderer;
	vtkDataSetMapper *m_DataSetMapper;
    vtkActor *m_Actor;
	vtkTextureMapToCylinder *m_TextureMapper;
	vtkJPEGReader *m_JPGReader;
	vtkTexture *m_Texture;

	vtkSphereSource *m_ProxySource;
	vtkPolyDataMapper *m_ProxyMapper;
	vtkActor *m_Proxy;

	vtkSphereSource *m_SCPSource;
	vtkPolyDataMapper *m_SCPMapper;
	vtkActor *m_SCP;

	vtkLineSource *m_LineSource;
	vtkPolyDataMapper *m_LineMapper;
	vtkActor *m_LineActor;

	float m_SphereRadius;	

   	vtkFlRender(int x, int y, int w, int h) ;
	virtual ~vtkFlRender();

	
	float centerX,centerY,centerZ;

	vtkOBBTree *m_OBBTree;	
	vtkPolyData **m_OBBTreePolyData;
	vtkPolyDataMapper **m_OBBTreePolyDataMapper;
	vtkActor **m_OBBTreeActor;

	

};
