// ControlForce.cpp: implementation of the CControlForce class.
//
//////////////////////////////////////////////////////////////////////

#include "ControlForce.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CControlForce::CControlForce()
{
	this->PhantomProxy=new gstPoint();
	this->m_PhantomGimbalAngles=new gstVector();
	this->Velocity=new gstVector();
	this->m_IntersectionCell=vtkGenericCell::New();
	this->cellId=-1;
	this->points=vtkPoints::New();
	this->points1=vtkPoints::New();
	this->cellIds=vtkIdList::New();
	IN_CONTACT=true;
	OUT_CONTACT=false;

	this->PhantomProxy->init(0,0,0);
	this->m_PhantomState=OUT_CONTACT;
	this->LastContact=0;

	this->m_PhantomProxy[0]=0;
	this->m_PhantomProxy[1]=0;
	this->m_PhantomProxy[2]=0;

	this->m_PhantomLastProxy[0]=0;
	this->m_PhantomLastProxy[1]=0;
	this->m_PhantomLastProxy[2]=0;

	this->m_SCP[0]=0;
	this->m_SCP[1]=0;
	this->m_SCP[2]=0;

	this->m_N[0]=0;
	this->m_N[1]=0;
	this->m_N[2]=0;

	this->m_V[0]=0;
	this->m_V[1]=0;
	this->m_V[2]=0;

	this->m_ForceSize=0;

	this->m_Force[0]=0;
	this->m_Force[1]=0;
	this->m_Force[2]=0;


	this->m_Velocity[0]=0;
	this->m_Velocity[1]=0;
	this->m_Velocity[2]=0;

	this->m_LastNormal[0]=0;
	this->m_LastNormal[1]=0;
	this->m_LastNormal[2]=0;

	this->m_Normal[0]=0;
	this->m_Normal[1]=0;
	this->m_Normal[2]=0;

	this->m_LastForce[0]=0;
	this->m_LastForce[1]=0;
	this->m_LastForce[2]=0;

	this->K=500;
	this->D=2;
	this->e=0.1;
	this->m_PenetrationOffset=1;
	this->m_Delta=0;
	
	this->m_Wait=false;
}

CControlForce::~CControlForce()
{
	delete this->PhantomProxy;
	delete this->m_PhantomGimbalAngles;
	delete this->Velocity;
	this->points->Delete();
	this->points1->Delete();
	this->cellIds->Delete();
	this->m_IntersectionCell->Delete();
}

gstVector CControlForce::calculateForceFieldForce(gstPHANToM *phantom)
{
	phantom->getPosition_WC(*this->PhantomProxy);
	(*this->m_PhantomGimbalAngles)=phantom->getGimbalAngles();
	(*this->Velocity)=phantom->getVelocity();

	this->m_PhantomProxy[0]=this->PhantomProxy->x()*this->dx;
	this->m_PhantomProxy[1]=this->PhantomProxy->y()*this->dy;
	this->m_PhantomProxy[2]=this->PhantomProxy->z()*this->dz;

	
	this->m_LastVelocity[0]=this->m_Velocity[0];
	this->m_LastVelocity[1]=this->m_Velocity[1];
	this->m_LastVelocity[2]=this->m_Velocity[2];
	
	this->m_Velocity[0]=this->Velocity->x();
	this->m_Velocity[1]=this->Velocity->y();
	this->m_Velocity[2]=this->Velocity->z();

	if (!this->m_Wait)
	{

	this->m_N[0]=this->m_PhantomProxy[0]+(this->m_SCP[0]-this->m_PhantomLastProxy[0])*5;
	this->m_N[1]=this->m_PhantomProxy[1]+(this->m_SCP[1]-this->m_PhantomLastProxy[1])*5;
	this->m_N[2]=this->m_PhantomProxy[2]+(this->m_SCP[2]-this->m_PhantomLastProxy[2])*5;
	isContact=this->m_OBBTree->IntersectWithLine(this->m_PhantomProxy,this->m_N,points,cellIds);

	if (isContact==-1) //c'è intersezione
	{
		if (this->m_PhantomState==IN_CONTACT) // era dentro..si e spostato 
		{	  								 // ed è rimasto dentro....	

			this->m_LastSCP[0]=this->m_SCP[0];
			this->m_LastSCP[1]=this->m_SCP[1];
			this->m_LastSCP[2]=this->m_SCP[2];

			this->m_Intersection[0]=points->GetPoint(0)[0];
			this->m_Intersection[1]=points->GetPoint(0)[1];
			this->m_Intersection[2]=points->GetPoint(0)[2];

			this->m_IntersectionCell=Dati->GetCell(cellIds->GetId(0));

			this->a1[0]=this->m_IntersectionCell->GetPoints()->GetPoint(0)[0];
			this->a1[1]=this->m_IntersectionCell->GetPoints()->GetPoint(0)[1];
			this->a1[2]=this->m_IntersectionCell->GetPoints()->GetPoint(0)[2];

			this->a2[0]=this->m_IntersectionCell->GetPoints()->GetPoint(1)[0];
			this->a2[1]=this->m_IntersectionCell->GetPoints()->GetPoint(1)[1];
			this->a2[2]=this->m_IntersectionCell->GetPoints()->GetPoint(1)[2];

			this->a3[0]=this->m_IntersectionCell->GetPoints()->GetPoint(2)[0];
			this->a3[1]=this->m_IntersectionCell->GetPoints()->GetPoint(2)[1];
			this->a3[2]=this->m_IntersectionCell->GetPoints()->GetPoint(2)[2];


			this->m_LastNormal[0]=this->m_Normal[0];
			this->m_LastNormal[1]=this->m_Normal[1];
			this->m_LastNormal[2]=this->m_Normal[2];

			vtkTriangle::ComputeNormal(a1,a2,a3,this->m_Normal);

			this->m_Normal[0]=(this->m_Normal[0]+3*this->m_LastNormal[0])/4;
			this->m_Normal[1]=(this->m_Normal[1]+3*this->m_LastNormal[1])/4;
			this->m_Normal[2]=(this->m_Normal[2]+3*this->m_LastNormal[2])/4;

			vtkMath::Normalize(this->m_Normal);
			
			this->m_N[0]=(this->m_Intersection[0]-this->m_PhantomProxy[0]);
			this->m_N[1]=(this->m_Intersection[1]-this->m_PhantomProxy[1]);
			this->m_N[2]=(this->m_Intersection[2]-this->m_PhantomProxy[2]);

			this->m_SCP[0]=this->m_Intersection[0]+this->m_Normal[0];
			this->m_SCP[1]=this->m_Intersection[1]+this->m_Normal[1];
			this->m_SCP[2]=this->m_Intersection[2]+this->m_Normal[2];
		}
		else								  // era fuori ed è entrato...
		{					
			this->m_LastSCP[0]=this->m_SCP[0];
			this->m_LastSCP[1]=this->m_SCP[1];
			this->m_LastSCP[2]=this->m_SCP[2];

			this->m_Intersection[0]=points->GetPoint(0)[0];
			this->m_Intersection[1]=points->GetPoint(0)[1];
			this->m_Intersection[2]=points->GetPoint(0)[2];	
			
			this->m_IntersectionCell=Dati->GetCell(cellIds->GetId(0));

			this->a1[0]=this->m_IntersectionCell->GetPoints()->GetPoint(0)[0];
			this->a1[1]=this->m_IntersectionCell->GetPoints()->GetPoint(0)[1];
			this->a1[2]=this->m_IntersectionCell->GetPoints()->GetPoint(0)[2];

			this->a2[0]=this->m_IntersectionCell->GetPoints()->GetPoint(1)[0];
			this->a2[1]=this->m_IntersectionCell->GetPoints()->GetPoint(1)[1];
			this->a2[2]=this->m_IntersectionCell->GetPoints()->GetPoint(1)[2];

			this->a3[0]=this->m_IntersectionCell->GetPoints()->GetPoint(2)[0];
			this->a3[1]=this->m_IntersectionCell->GetPoints()->GetPoint(2)[1];
			this->a3[2]=this->m_IntersectionCell->GetPoints()->GetPoint(2)[2];

			this->m_LastNormal[0]=this->m_Normal[0];
			this->m_LastNormal[1]=this->m_Normal[1];
			this->m_LastNormal[2]=this->m_Normal[2];

			vtkTriangle::ComputeNormal(a1,a2,a3,this->m_Normal);

			this->m_Normal[0]=(this->m_Normal[0]+3*this->m_LastNormal[0])/4;
			this->m_Normal[1]=(this->m_Normal[1]+3*this->m_LastNormal[1])/4;
			this->m_Normal[2]=(this->m_Normal[2]+3*this->m_LastNormal[2])/4;

			vtkMath::Normalize(this->m_Normal);

			this->m_N[0]=(this->m_Intersection[0]-this->m_PhantomProxy[0]);
			this->m_N[1]=(this->m_Intersection[1]-this->m_PhantomProxy[1]);
			this->m_N[2]=(this->m_Intersection[2]-this->m_PhantomProxy[2]);

			this->m_SCP[0]=this->m_Intersection[0]+this->m_Normal[0];
			this->m_SCP[1]=this->m_Intersection[1]+this->m_Normal[1];
			this->m_SCP[2]=this->m_Intersection[2]+this->m_Normal[2];

		}

		this->m_PhantomState=IN_CONTACT;
		//calcolo la forza...

		this->m_V[0]=(this->m_SCP[0]-this->m_PhantomProxy[0]);
		this->m_V[1]=(this->m_SCP[1]-this->m_PhantomProxy[1]);
		this->m_V[2]=(this->m_SCP[2]-this->m_PhantomProxy[2]);

		this->m_Delta=vtkMath::Norm(this->m_V,3);

		float dl=fabs(this->m_Delta-this->m_PenetrationOffset)*K/1000;

		this->m_LastForce[0]=this->m_Force[0];
		this->m_LastForce[1]=this->m_Force[1];
		this->m_LastForce[2]=this->m_Force[2];
		
		this->m_Force[0]=this->m_Normal[0]*dl-D*(this->m_Velocity[0]/1000);
		this->m_Force[1]=this->m_Normal[1]*dl-D*(this->m_Velocity[1]/1000);
		this->m_Force[2]=this->m_Normal[2]*dl-D*(this->m_Velocity[2]/1000);

		this->m_Force[0]=(this->m_Force[0]+2*this->m_LastForce[0])/3;
		this->m_Force[1]=(this->m_Force[1]+2*this->m_LastForce[1])/3;
		this->m_Force[2]=(this->m_Force[2]+2*this->m_LastForce[2])/3;
		
		this->m_ForceSize=vtkMath::Norm(this->m_Force,3);	

		//al max 3 Newton
		if (this->m_ForceSize>=5)
		{
			for (int i=0;i<2;i++)
				this->m_Force[i]=(this->m_Force[i]/this->m_ForceSize)*5;
			this->m_ForceSize=5;
		}

			if (this->m_Delta<=this->m_PenetrationOffset)
			{
				this->m_Force[0]=0;
				this->m_Force[1]=0;
				this->m_Force[2]=0;
			}
	}
	else	// non c'è intersezione
	{
		this->m_PhantomState=OUT_CONTACT;

		this->m_N[0]=(this->m_PhantomLastProxy[0]-this->m_PhantomProxy[0]);
		this->m_N[1]=(this->m_PhantomLastProxy[1]-this->m_PhantomProxy[1]);
		this->m_N[2]=(this->m_PhantomLastProxy[2]-this->m_PhantomProxy[2]);

		float G=this->e/vtkMath::Norm(this->m_N,3);

		this->m_SCP[0]=this->m_PhantomProxy[0]+this->m_N[0]*G;
		this->m_SCP[1]=this->m_PhantomProxy[1]+this->m_N[1]*G;
		this->m_SCP[2]=this->m_PhantomProxy[2]+this->m_N[2]*G;
	
		this->m_Force[0]=0;
		this->m_Force[1]=0;
		this->m_Force[2]=0;

		this->m_Delta=0;
		this->m_ForceSize=0;
		this->LastContact=0;
			
		this->m_LastSCP[0]=0;
		this->m_LastSCP[1]=0;
		this->m_LastSCP[2]=0;

		this->m_LastNormal[0]=0;
		this->m_LastNormal[1]=0;
		this->m_LastNormal[2]=0;

		this->m_LastForce[0]=0;
		this->m_LastForce[1]=0;
		this->m_LastForce[2]=0;

		this->m_Normal[0]=0;
		this->m_Normal[1]=0;
		this->m_Normal[2]=0;
	}

	this->m_PhantomLastProxy[0]=this->m_PhantomProxy[0];
	this->m_PhantomLastProxy[1]=this->m_PhantomProxy[1];
	this->m_PhantomLastProxy[2]=this->m_PhantomProxy[2];

	this->LastContact=isContact;

    return gstVector(this->m_Force[0],this->m_Force[1],this->m_Force[2]);
	}
	else
	{
		return gstVector(0,0,0);
	}
}

float* CControlForce::GetPhantomProxy()
{
	return this->m_PhantomProxy;
}

float* CControlForce::GetPhantomLastProxy()
{
	return this->m_PhantomLastProxy;
}

float* CControlForce::GetSCP()
{
	return this->m_SCP;
}

float CControlForce::GetPhantomAngleZ()
{
	return ((this->m_PhantomGimbalAngles->z()/M_PI)*180);//-26;
}

float CControlForce::GetPhantomAngleY()
{
	return ((this->m_PhantomGimbalAngles->x()/M_PI)*180);//-39;
}

float CControlForce::GetPhantomAngleX()
{
	return ((this->m_PhantomGimbalAngles->y()/M_PI)*180);//-126;
}

void CControlForce::SetOBBTree(vtkOBBTree *OBB_Tree)
{
	this->m_OBBTree=OBB_Tree;
}

bool CControlForce::GetPhantomState()
{
	return this->m_PhantomState;
}
