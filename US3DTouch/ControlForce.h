#include <gstForceField.h>
#include "vtkOBBTree.h"
#include "vtkGenericCell.h"
#include "vtkTriangle.h"
#include  <math.h>
#include "vtkDataSet.h"



class CControlForce : public gstForceField  
{

public:
	bool GetPhantomState();
	void SetOBBTree(vtkOBBTree *OBB_Tree);
	float* GetPhantomProxy();
	float* GetPhantomLastProxy();
	float* GetSCP();

	float GetPhantomAngleX();
	float GetPhantomAngleY();
	float GetPhantomAngleZ();
	
	virtual gstVector calculateForceFieldForce(gstPHANToM* phantom);
	CControlForce();
	virtual ~CControlForce();

	gstPoint *PhantomProxy;
	gstVector *m_PhantomGimbalAngles;
	gstVector *Velocity;

	float m_PhantomProxy[3];
	float m_PhantomLastProxy[3];
	float m_SCP[3];
	float m_LastSCP[3];
	float m_Intersection[3];
	float m_N[3];
	float m_M[3];
	float m_V[3];
	float m_Force[3];
	float m_LastForce[3];
	float m_Triangle[3];
	float m_Normal[3];
	float m_Velocity[3];
	float m_LastVelocity[3];
	float m_LastNormal[3];
	float m_ForceSize;
	float m_Delta;
	float Theta;
	float K;
	float D;
	float e;
	float m_PenetrationOffset;
	bool m_PhantomState;
	bool m_Wait;
	int isContact;
	int LastContact;

	bool IN_CONTACT;
	bool OUT_CONTACT;

	float t;
	int subId;
	vtkIdType cellId;

	vtkCell *m_IntersectionCell;

	vtkOBBTree *m_OBBTree;

	vtkPoints *points;
	vtkPoints *points1;
	vtkIdList *cellIds;

	vtkDataSet *Dati;
	float a1[3],a2[3],a3[3];
	float dx,dy,dz;
};

