// US3D.cpp: implementation of the US3D class.
//
//////////////////////////////////////////////////////////////////////

#include "US3DTouch.h"
#include <FL/fl_file_chooser.H>
#include "vtkInteractorStyleTrackballCamera.h"


struct ProgressArguments
{
	US3DTouch *This;
	vtkProcessObject *Process;
	char *Label;
	Fl_Progress *progress;
};

US3DTouch::US3DTouch()
{

	

	this->m_TransformFilter=vtkTransformFilter::New(); 
	this->m_Transform=vtkTransform::New();
	this->m_Transform->GlobalWarningDisplayOff();
	this->m_TransformFilter->SetTransform(this->m_Transform);
	this->m_CumulateTransform=vtkTransform::New();

	this->tmpString=new char[200];
	this->init=false;
	this->m_DataSetReader=vtkPolyDataReader::New();
	this->SetProgress(this->m_DataSetReader,"Loading...",this->m_CProgress);
	this->m_CRender->SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());  

	XMIN = -215;
	XMAX = 206;
	YMIN = -105;
	YMAX = 161;
	ZMIN = -105;
	ZMAX = 128;

	this->m_Rotate=false;

	//Init Phantom
	this->m_rootSeparator=new gstSeparator;
	this->m_ForceControl=new CControlForce;
	this->m_Scene=new gstScene;
	this->m_Phantom=new gstPHANToM("Default PHANToM");
	this->m_Center=new gstPoint;

	this->m_AxisX=new gstVector(1,0,0);
	this->m_AxisY=new gstVector(0,1,0);
	this->m_AxisZ=new gstVector(0,0,1);

	this->m_Scene->setRoot(this->m_rootSeparator);

    if (!(this->m_Phantom) || !(this->m_Phantom->getValidConstruction())) {
		fl_alert("Unable to initialize PHANToM device");
        //exit(-1);
    }

	this->m_Center->init(0,0,0);
	this->m_ForceControl->boundByBox(this->m_Center,800,800,800);
	this->m_ForceControl->setAttenuationDistance(4);
	this->m_rootSeparator->addChild(this->m_Phantom);
	this->m_rootSeparator->addChild(this->m_ForceControl);

	this->m_ForceControl->SetOBBTree(this->m_CRender->m_OBBTree);
}

US3DTouch::~US3DTouch()
{
	delete this->tmpString;
	this->m_DataSetReader->Delete();
	delete this->m_rootSeparator;
	delete this->m_ForceControl;
	delete this->m_Scene;
	delete this->m_Phantom;
	delete this->m_Center;
}

void US3DTouch::Load() {
	
  char * filename = fl_file_chooser("VTK 3D filename","*.vtk","");

  if( !filename )
  {
    return;
  }

  this->m_Scene->stopServoLoop();

  if (!(this->init))
	{
		this->m_CRender->show();
		this->init=true;
		Fl::add_timeout(0.03,this->updateGraphicsCallBack,this);
	}

  this->m_DataSetReader->SetFileName(filename);
  this->m_DataSetReader->Update();
  this->m_TransformFilter->SetInput(this->m_DataSetReader->GetOutput());
  this->m_TransformFilter->Update();
  this->m_CRender->m_OBBTree->FreeSearchStructure();
  this->m_CRender->SetInput(this->m_TransformFilter->GetOutput());

  float f[6];
  this->m_CRender->m_Actor->GetBounds(f);

  

  this->m_CRender->m_Renderer->GetActiveCamera()->SetFocalPoint(0,0,0);
  this->m_CRender->m_Renderer->GetActiveCamera()->SetPosition(0,0,5*f[5]);
  this->m_CRender->m_Renderer->GetActiveCamera()->SetViewUp(0,1,0);
  //this->m_CRender->m_Renderer->ResetCamera();
  this->m_CRender->m_Renderer->ResetCameraClippingRange(f);

  this->m_CRender->draw();
  sprintf(this->tmpString,"%d",this->m_DataSetReader->GetOutput()->GetNumberOfCells());
  this->m_CNCells->value(this->tmpString);
  this->m_ForceControl->Dati=this->m_TransformFilter->GetOutput();
	
  
  
  this->m_ForceControl->dx=(5*f[1])/(this->XMAX-this->XMIN);
  this->m_ForceControl->dy=(5*f[3])/(this->YMAX-this->YMIN);
  this->m_ForceControl->dz=(5*f[5])/(this->ZMAX-this->ZMIN);

 /* this->m_ForceControl->dx=1;
  this->m_ForceControl->dy=1;
  this->m_ForceControl->dz=1;*/

  this->m_CServoLoop->value(1);
  this->m_CServoLoop->label("Stop servo loop");
  this->m_Scene->startServoLoop();

}


void US3DTouch::SetServoLoop()
{
  if (this->m_CServoLoop->value()==1)
  {
     this->m_Scene->startServoLoop();
	 this->m_CServoLoop->label("Stop servo loop");
  }
  else
  {
     this->m_Scene->stopServoLoop();
	 this->m_CServoLoop->label("Start servo loop");
  }
  Fl::flush();
}

void US3DTouch::Progress(void *arg)
{
	US3DTouch *This=((ProgressArguments*)arg)->This;
	This->ProgressCall(arg);
}

void US3DTouch::ProgressCall(void *arg)
{
	vtkProcessObject *Process=((ProgressArguments*)arg)->Process;
	Fl_Progress *progress=((ProgressArguments*)arg)->progress;
	char *Label=((ProgressArguments*)arg)->Label;
	float value=Process->GetProgress()*100;
	progress->value(value);
	if (value==100)
		sprintf(this->tmpString,"[%s]","Ready");
	else
		sprintf(this->tmpString,"%s [%.2f%%]",Label,value);
		
	progress->label(this->tmpString);
	Fl::flush();
}


void US3DTouch::SetProgress(vtkProcessObject *process, char *label, Fl_Progress *progress)
{
	ProgressArguments *arg=new ProgressArguments;
	arg->This=this;
	arg->Process=process;
    arg->Label=label;
	arg->progress=progress;
	process->SetProgressMethod(this->Progress,arg);
}

void US3DTouch::SetOpacity() {
	this->m_CRender->m_Actor->GetProperty()->SetOpacity(this->m_COpacity->value());
}

void US3DTouch::SetProxyRadius() {
	this->m_CRender->SetProxyRadius(this->m_CProxyRadius->value());
}

void US3DTouch::SetOBBTreeView() {
	this->m_CRender->m_OBBTreeActor[this->m_CRender->m_Level]->SetVisibility(this->m_COBBTreeView->value());
	if (this->m_COBBTreeView->value()==0)
		this->m_COBBTreeLevel->deactivate();
	else
		this->m_COBBTreeLevel->activate();		
}

void US3DTouch::SetOBBTreeSurfaceType() {
	int t=this->m_COBBTreeSurfaceType->value();
	for (int i=0;i<this->m_CRender->m_MaxLevel;i++)
	if (t==1) this->m_CRender->m_OBBTreeActor[i]->GetProperty()->SetRepresentationToSurface();
	else	  this->m_CRender->m_OBBTreeActor[i]->GetProperty()->SetRepresentationToWireframe();
}

void US3DTouch::SetOBBTreeOpacity() {
	for (int i=0;i<this->m_CRender->m_MaxLevel;i++)
		this->m_CRender->m_OBBTreeActor[i]->GetProperty()->SetOpacity(this->m_COBBTreeOpacity->value());
}

void US3DTouch::SetOBBTreeLevel() {
	this->m_CRender->SetViewOBBTreeLevel(this->m_COBBTreeLevel->value());
}

void US3DTouch::SetDamping()
{
	this->m_ForceControl->D=this->m_CDamping->value();
}
void US3DTouch::SetElastic()
{
	this->m_ForceControl->K=this->m_CElastic->value();
}

void US3DTouch::updateGraphicsCallBack(void* Object)
{
	US3DTouch *This=(US3DTouch*)Object;
	This->updateGraphics();
	Fl::add_timeout(0.03,This->updateGraphicsCallBack,This);
}

void US3DTouch::updateGraphics()
{
	float *pos=this->m_ForceControl->GetPhantomProxy();
	float *scp=this->m_ForceControl->GetSCP();

	sprintf(this->tmpString,"%.2f",pos[0]);
	this->m_CPhantomProxy_X->value(this->tmpString);
	sprintf(this->tmpString,"%.2f",pos[1]);
	this->m_CPhantomProxy_Y->value(this->tmpString);
	sprintf(this->tmpString,"%.2f",pos[2]);
	this->m_CPhantomProxy_Z->value(this->tmpString);

	sprintf(this->tmpString,"%.2f",scp[0]);
	this->m_CPhantomSCP_X->value(this->tmpString);
	sprintf(this->tmpString,"%.2f",scp[1]);
	this->m_CPhantomSCP_Y->value(this->tmpString);
	sprintf(this->tmpString,"%.2f",scp[2]);
	this->m_CPhantomSCP_Z->value(this->tmpString);

	sprintf(this->tmpString,"%.0f",this->m_ForceControl->m_Velocity[0]);
	this->m_CPhantomVelocity_X->value(this->tmpString);
	sprintf(this->tmpString,"%.0f",this->m_ForceControl->m_Velocity[1]);
	this->m_CPhantomVelocity_Y->value(this->tmpString);
	sprintf(this->tmpString,"%.0f",this->m_ForceControl->m_Velocity[2]);
	this->m_CPhantomVelocity_Z->value(this->tmpString);

	sprintf(this->tmpString,"%.3f",this->m_ForceControl->m_Force[0]);
	this->m_CPhantomForce_X->value(this->tmpString);
	sprintf(this->tmpString,"%.3f",this->m_ForceControl->m_Force[1]);
	this->m_CPhantomForce_Y->value(this->tmpString);
	sprintf(this->tmpString,"%.3f",this->m_ForceControl->m_Force[2]);
	this->m_CPhantomForce_Z->value(this->tmpString);

	sprintf(this->tmpString,"%.3f",this->m_ForceControl->m_Delta-this->m_ForceControl->m_PenetrationOffset);
	this->m_CPhantomDelta->value(this->tmpString);

	sprintf(this->tmpString,"%.3f",this->m_ForceControl->m_ForceSize);
	this->m_CPhantomForceSize->value(this->tmpString);

	sprintf(this->tmpString,"%.2f Hz",this->m_Phantom->getAverageUpdateRate());
	this->m_CUpdateRate->value(this->tmpString);
	
	if (this->m_ForceControl->GetPhantomState()) 
		this->m_CInOut->value("CONTACT");
	else
	    this->m_CInOut->value("");
	
	if (this->m_Phantom->getStylusSwitch()==TRUE)
	{
		//this->m_CRender->m_OBBTreeActor[this->m_CRender->m_Level]->SetPosition(pos);
		this->m_ForceControl->m_Wait=true;
		if (this->m_Rotate)
		{
			rx=this->m_ForceControl->GetPhantomAngleX()-this->m_Begin_RX;
			ry=this->m_ForceControl->GetPhantomAngleY()-this->m_Begin_RY;
			rz=this->m_ForceControl->GetPhantomAngleZ()-this->m_Begin_RZ;	

			tx=pos[0]-this->m_Begin_TX;
			ty=pos[1]-this->m_Begin_TY;
			tz=pos[2]-this->m_Begin_TZ;	

			//this->m_Transform->Translate(tx,ty,tz);

			if (rx>2)
				this->m_Transform->RotateWXYZ(fabs(rx),1,0,0);
			else
				if (rx<-2)
				this->m_Transform->RotateWXYZ(-fabs(rx),1,0,0);

			if (ry>2)
				this->m_Transform->RotateWXYZ(fabs(ry),0,1,0);
			else
				if (ry<-2)
				this->m_Transform->RotateWXYZ(-fabs(ry),0,1,0);

			if (rz>2)
				this->m_Transform->RotateWXYZ(-fabs(rz),0,0,1);
			else
				if (rz<-2)
				this->m_Transform->RotateWXYZ(fabs(rz),0,0,1);

			


			/*float *g=this->m_CRender->m_Actor->GetOrigin();
			this->m_CRender->m_Actor->SetOrigin(g[0]+tx,g[1]+ty,g[2]+tz);*/


			sprintf(this->tmpString,"%.2f",rx);
			this->m_CPhantomAngle_X->value(this->tmpString);
			sprintf(this->tmpString,"%.2f",ry);
			this->m_CPhantomAngle_Y->value(this->tmpString);
			sprintf(this->tmpString,"%.2f",rz);
			this->m_CPhantomAngle_Z->value(this->tmpString);

			this->m_Begin_RX=this->m_ForceControl->GetPhantomAngleX();
			this->m_Begin_RY=this->m_ForceControl->GetPhantomAngleY();
			this->m_Begin_RZ=this->m_ForceControl->GetPhantomAngleZ();		

			//float *g=this->m_ForceControl->GetPhantomProxy();

			float *g=this->m_CRender->m_Actor->GetOrigin();

			//this->m_DataSetReader->GetOutput()->GetCenter();
			//this->m_CRender->m_Actor->SetOrigin(g[0]+tx,g[1]+ty,g[2]+tz);

			this->m_Begin_TX=pos[0];
			this->m_Begin_TY=pos[1];
			this->m_Begin_TZ=pos[2];
		}
		else
		{
			
			this->m_Begin_RX=this->m_ForceControl->GetPhantomAngleX();
			this->m_Begin_RY=this->m_ForceControl->GetPhantomAngleY();
			this->m_Begin_RZ=this->m_ForceControl->GetPhantomAngleZ();

			this->m_Begin_TX=pos[0];
			this->m_Begin_TY=pos[1];
			this->m_Begin_TZ=pos[2];
			this->m_Rotate=true;
		}
	}
	else
		if (this->m_Rotate)
		{
			this->m_Rotate=false;

			this->m_CRender->m_OBBTree->FreeSearchStructure();
			this->m_CRender->m_OBBTree->BuildLocator();

			this->m_ForceControl->m_Wait=false;
		}

	this->m_CRender->m_Proxy->SetPosition(pos);
	this->m_CRender->m_SCP->SetPosition(scp);
	this->m_CRender->m_LineActor->SetPosition(0,0,0);
	this->m_CRender->m_LineSource->SetPoint1(scp);
	this->m_CRender->m_LineSource->SetPoint2(pos);

	this->m_CRender->draw();
}









