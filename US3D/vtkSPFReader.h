
#ifndef __vtkSPFReader_h
#define __vtkSPFReader_h

#include "vtkStructuredPoints.h"

class vtkSPFReader 
{
public:
	void LoadHeader();
	void Init();
    static vtkSPFReader *New();
    void SetFileName(const char * FileName);
  	
	void Load();

	unsigned short m_volumeDepth;
	unsigned short m_volumeHeight;
	unsigned short m_volumeWidth;

	float m_voxelDepth;
	float m_voxelHeight;
	float m_voxelWidth;	

	unsigned short m_DataOffset;

	vtkStructuredPoints *m_Output;  

	const char * FileName;
protected:

	vtkSPFReader();
   ~vtkSPFReader();
	
private:

	int Error;
	
};

#endif


