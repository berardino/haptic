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

	//Init VTK

	this->m_MaxLevel=10;
	this->m_Level=0;



	this->m_OBBTreePolyData= new vtkPolyData*[this->m_MaxLevel];
	this->m_OBBTreePolyDataMapper= new vtkPolyDataMapper*[this->m_MaxLevel];
	this->m_OBBTreeActor= new vtkActor*[this->m_MaxLevel];
	

	this->m_OBBTree=vtkOBBTree::New();
	this->m_JPGReader=vtkJPEGReader::New();
	this->m_Texture=vtkTexture::New();

	this->m_Renderer = vtkRenderer::New();
	this->m_Actor=vtkActor::New();
	this->m_DataSetMapper=vtkDataSetMapper::New();
    this->m_RenderWindow = vtkRenderWindow::New();
	this->m_TextureMapper=vtkTextureMapToCylinder::New();

	this->m_ProxySource=vtkSphereSource::New();
	this->m_ProxyMapper=vtkPolyDataMapper::New();
	this->m_Proxy=vtkActor::New();

	this->m_SCPSource=vtkSphereSource::New();
	this->m_SCPMapper=vtkPolyDataMapper::New();
	this->m_SCP=vtkActor::New();

	this->m_LineSource=vtkLineSource::New();
	this->m_LineMapper=vtkPolyDataMapper::New();
	this->m_LineActor=vtkActor::New();

	this->m_LineSource->SetPoint1(0,0,0);
	this->m_LineSource->SetPoint2(0,0,0);
	this->m_LineMapper->SetInput(this->m_LineSource->GetOutput());
	this->m_LineActor->SetMapper(this->m_LineMapper);


	this->m_JPGReader->SetFileName("texture.jpg");
	this->m_Texture->SetInput(this->m_JPGReader->GetOutput());
	this->m_Texture->InterpolateOn();
	this->m_Texture->RepeatOn();
	
	this->m_RenderWindow->SetDesiredUpdateRate(30);
    this->m_RenderWindow->AddRenderer(this->m_Renderer);
	this->SetRenderWindow(this->m_RenderWindow);
	this->m_Actor->SetMapper(this->m_DataSetMapper);
	//this->m_Actor->SetTexture(this->m_Texture);
	
	this->m_ProxySource->SetThetaResolution(10);
	this->m_ProxySource->SetPhiResolution (10); 
	
	this->m_ProxyMapper->SetInput(this->m_ProxySource->GetOutput());
	this->m_Proxy->SetMapper(this->m_ProxyMapper);
	this->m_Proxy->GetProperty()->SetColor(1,0,0);

	this->m_SCPSource->SetThetaResolution(10);
	this->m_SCPSource->SetPhiResolution (10); 
	
	this->m_SCPMapper->SetInput(this->m_SCPSource->GetOutput());
	this->m_SCP->SetMapper(this->m_SCPMapper);
	this->m_SCP->GetProperty()->SetColor(0,1,0);

	for (int i=0;i<this->m_MaxLevel;i++)
	{
		this->m_OBBTreePolyData[i]=vtkPolyData::New();
		this->m_OBBTreePolyDataMapper[i]=vtkPolyDataMapper::New();
		this->m_OBBTreeActor[i]=vtkActor::New();

		this->m_OBBTreePolyDataMapper[i]->SetInput(this->m_OBBTreePolyData[i]);
		this->m_OBBTreeActor[i]->SetMapper(this->m_OBBTreePolyDataMapper[i]);
		this->m_OBBTreeActor[i]->GetProperty()->SetColor(1,1,0);
		this->m_OBBTreeActor[i]->GetProperty()->SetOpacity(0.1);

		this->m_Renderer->AddActor(this->m_OBBTreeActor[i]);

		this->m_OBBTreeActor[i]->VisibilityOff();
	}

	this->m_OBBTree->CacheCellBoundsOn();

	this->m_Renderer->AddActor(this->m_Actor);
	this->m_Renderer->AddActor(this->m_Proxy);
	this->m_Renderer->AddActor(this->m_SCP);
	this->m_Renderer->AddActor(this->m_LineActor);

	this->m_Renderer->SetBackground(0.5,0.5,0.7);
	this->m_Renderer->SetAmbient(0.7,0,0);

    this->m_Actor->GetProperty()->SetAmbient(0.2);

	

}

vtkFlRender::~vtkFlRender()
{
	this->m_OBBTree->FreeSearchStructure();
	this->m_OBBTree->Delete();
	this->m_JPGReader->Delete();
	this->m_Texture->Delete();

	this->m_Renderer->Delete();
	this->m_Actor->Delete();
	this->m_DataSetMapper->Delete();
    this->m_RenderWindow->Delete();
	this->m_TextureMapper->Delete();

	this->m_ProxySource->Delete();
	this->m_ProxyMapper->Delete();
	this->m_Proxy->Delete();

	this->m_SCPSource->Delete();
	this->m_SCPMapper->Delete();
	this->m_SCP->Delete();

	this->m_LineSource->Delete();
	this->m_LineMapper->Delete();
	this->m_LineActor->Delete();

	
	for (int i=0;i<this->m_MaxLevel;i++)
	{
		this->m_OBBTreePolyData[i]->Delete();
		this->m_OBBTreePolyDataMapper[i]->Delete();
		this->m_OBBTreeActor[i]->Delete();
	}

	delete this->m_OBBTreePolyData;
	delete this->m_OBBTreePolyDataMapper;
	delete this->m_OBBTreeActor;
}

void vtkFlRender::SetInput(vtkPointSet *Input)
{
	float f[6];

	this->m_TextureMapper->SetInput(Input);
	this->m_DataSetMapper->SetInput(this->m_TextureMapper->GetOutput());
	this->m_Actor->GetBounds(f);
	this->m_SphereRadius=2;
	this->m_ProxySource->SetRadius(2);
	this->m_SCPSource->SetRadius(this->m_SphereRadius);
	this->centerX=f[0]+(f[1]-f[0])/2;
	this->centerY=f[2]+(f[3]-f[2])/2;
	this->centerZ=f[4]+(f[5]-f[4])/2;
	this->m_ProxySource->SetCenter(0,0,0);
	this->m_SCPSource->SetCenter(0,0,0);
	this->m_Actor->SetOrigin(0,0,0);
	for (int j=0;j<this->m_MaxLevel;j++)
		this->m_OBBTreeActor[j]->SetOrigin(0,0,0);

	this->m_OBBTree->SetDataSet(Input);
	this->m_OBBTree->SetMaxLevel(this->m_MaxLevel);
	this->m_OBBTree->SetNumberOfCellsPerBucket(20);
	this->m_OBBTree->BuildLocator();

	for (int i=0;i<this->m_MaxLevel;i++)
		this->m_OBBTree->GenerateRepresentation(i,this->m_OBBTreePolyData[i]);

	/*this->m_Proxy->SetPosition(centerX,centerY,centerZ);
	this->m_SCP->SetPosition(centerX,centerY,centerZ);
	this->m_LineActor->SetPosition(centerX,centerY,centerZ);*/

	this->m_Proxy->SetPosition(0,0,0);
	this->m_SCP->SetPosition(0,0,0);
	this->m_LineActor->SetPosition(0,0,0);

	this->m_OBBTreeActor[this->m_Level]->VisibilityOn();
}

void vtkFlRender::SetProxyRadius(float radius)
{
	this->m_ProxySource->SetRadius(radius);
	this->m_SCPSource->SetRadius(radius);
	this->m_SphereRadius=radius;
}

void vtkFlRender::SetViewOBBTreeLevel(int level)
{
	this->m_OBBTreeActor[this->m_Level]->VisibilityOff();
	this->m_OBBTreeActor[level]->VisibilityOn();
	this->m_Level=level;
}
