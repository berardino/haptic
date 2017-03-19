#include "US3DTouchGui.h"
#include "vtkPolyDataReader.h"
#include "vtkInteractorStyleTrackballCamera.h"

#include <FL/fl_ask.H>

#include <gstScene.h>
#include <gstPHANToM.h>
#include "ControlForce.h"


#include "vtkCubeSource.h"
#include "vtkSphereSource.h"

#include "vtkTransformFilter.h"
#include "vtkTransform.h"



class US3DTouch : public US3DTouchGui  
{
public:
	US3DTouch();
	~US3DTouch();
public:
	void SetProgress(vtkProcessObject *process,char *label,Fl_Progress *progress);
	void ProgressCall(void *arg);
	static void Progress(void *arg);

	void updateGraphics();
	static void updateGraphicsCallBack(void*);


	bool init;
	char *tmpString;
	vtkPolyDataReader *m_DataSetReader;

	virtual void Load();
	virtual void SetServoLoop();
	virtual void SetOpacity();
	virtual void SetProxyRadius();
	virtual void SetOBBTreeView();
	virtual void SetOBBTreeSurfaceType();
	virtual void SetOBBTreeOpacity();
	virtual void SetOBBTreeLevel();
	virtual void SetDamping();
	virtual void SetElastic();
	
	float XMIN;
	float XMAX;
	float YMIN;
	float YMAX;
	float ZMIN;
	float ZMAX;

	float dx,dy,dz;

	gstSeparator *m_rootSeparator;
	CControlForce *m_ForceControl;
	gstScene *m_Scene;
	gstPHANToM *m_Phantom;
	gstPoint *m_Center;

	gstVector *m_AxisX,*m_AxisY,*m_AxisZ;

	bool m_Rotate;
	float m_Begin_RX,m_Begin_RY,m_Begin_RZ,
		  m_Begin_TX,m_Begin_TY,m_Begin_TZ;

	float rx,ry,rz,
		  tx,ty,tz;

	vtkTransformFilter *m_TransformFilter;
	vtkTransform *m_Transform;
	vtkTransform *m_CumulateTransform;

};

