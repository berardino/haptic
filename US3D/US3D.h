#include "US3DGui.h"
#include "vtkSPFReader.h"
#include "vtkImageFlip.h"
#include "vtkImageReslice.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkExtractVOI.h"
#include "vtkImageThreshold.h"
#include "vtkMarchingCubes.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkDecimatePro.h"
#include "vtkVRMLExporter.h"
#include "vtkPolyDataWriter.h"
#include "vtkPolyDataNormals.h"
#include "vtkCleanPolyData.h"



class US3D : public US3DGui  
{
public:
	US3D();
	~US3D();
public:
	void SetProgress(vtkProcessObject *process,char *label,Fl_Progress *progress);
	bool init;
	void Init();
	void ProgressCall(void *arg);
	static void Progress(void *arg);

	vtkImageGaussianSmooth * m_ImageGaussianSmoothFilter;

	vtkImageReslice * m_ResliceAxial;
	vtkImageReslice * m_ResliceSaggittal;
	vtkImageReslice * m_ResliceCoronal;

	vtkImageReslice * m_ResliceSAxial;
	vtkImageReslice * m_ResliceSSaggittal;
	vtkImageReslice * m_ResliceSCoronal;

	vtkImageFlip * m_ImageFlipFilter;
	vtkSPFReader * m_Reader;
	vtkExtractVOI *m_VOI;
	vtkImageThreshold *m_Threshold;

	vtkMarchingCubes *m_MarchingCubes;
	vtkSmoothPolyDataFilter *m_SmoothPolyDataFilter;
	vtkPolyDataConnectivityFilter *m_PolyDataConnectivityFilter; 
	vtkDecimatePro *m_Decimate;
	vtkPolyDataNormals *m_PolyDataNormals;
	vtkCleanPolyData *m_CleanPolyData;

	
	vtkVRMLExporter *m_VRMLExporter;
	vtkPolyDataWriter *m_Writer;


	int m_VOI_Xi,m_VOI_Xf,
		m_VOI_Yi,m_VOI_Yf,
		m_VOI_Zi,m_VOI_Zf;

	char *tmpString;


  virtual void Load();
  virtual void SaveVRML();
  virtual void SetBrightness();
  virtual void SetSmoothing();
  virtual void SetDecimate();
  virtual void SurfaceRender();
  virtual void RayRender();
  virtual void SetOpacity();
  virtual void SetFilteredSliceDepth();
  virtual void SetFilteredSliceWidth();
  virtual void SetFilteredSliceHeight();
  virtual void PlayStopFiltered();
  virtual void PlayStopSegmented();
  virtual void SetControlRay();
  virtual void SetControlRender();
  virtual void SetRadius(float value);
  virtual void SetStandardDeviation(float value);
  virtual void SetVOIDepthUp();
  virtual void SetVOIDepthDown();
  virtual void ExtractVOI();
  virtual void SetSegmentedSliceWidth();
  virtual void SetSegmentedSliceDepth();
  virtual void SetSegmentedSliceHeight();
  virtual void SetThresholding();
  virtual void SetShadingInterpolationMethod();
  virtual void SetFeatureAngle();
  virtual void SetVolumeResolution();
  virtual void VolumeRender();
  virtual void SaveVTK();
};

