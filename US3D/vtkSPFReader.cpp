
#include "vtkSPFReader.h"

//------------------------------------------------------------------------------
vtkSPFReader* vtkSPFReader::New()
{
  return new vtkSPFReader;
}


vtkSPFReader::vtkSPFReader()
{
	this->m_Output=vtkStructuredPoints::New();
	this->Error=0;
}

vtkSPFReader::~vtkSPFReader()
{
}

void vtkSPFReader::Load()
{
  FILE *spf;

  vtkUnsignedCharArray *newScalars;
  int   size[3];

  int   plane_size;
  int   volume_size;
  int   z_counter;
  int   i;

  unsigned char *scan_ptr = NULL;
  unsigned char *sptr = NULL;

  this->Error=0;
  
  this->LoadHeader();  


  /*this->m_Output->SetAxisUpdateExtent(0,0, this->m_volumeWidth-1);
  this->m_Output->SetAxisUpdateExtent(1,0, this->m_volumeHeight-1);
  this->m_Output->SetAxisUpdateExtent(2,0, this->m_volumeDepth-1);*/
  //this->m_Output->SetUpdateExtent(0, this->m_volumeWidth-1, 0, this->m_volumeHeight-1, 0, this->m_volumeDepth-1);
  this->m_Output->SetWholeExtent(0, this->m_volumeWidth-1, 0, this->m_volumeHeight-1, 0, this->m_volumeDepth-1);
  //this->m_Output->SetSpacing(this->m_voxelWidth,this->m_voxelHeight,this->m_voxelDepth);
  this->m_Output->SetSpacing(1,1,1);


  (*size)=this->m_volumeWidth;
  (*(size+1))=this->m_volumeHeight;
  (*(size+2))=this->m_volumeDepth;

  m_Output->SetDimensions(size);
  
  //...letta l'header apro il file il lettura binaria
  spf=fopen(this->FileName,"rb");
  fseek(spf,this->m_DataOffset, SEEK_SET);	
	
  //Dimensione del piano immagine
  plane_size = (this->m_volumeWidth) * (this->m_volumeHeight);

  //Dimesnione del volume
  volume_size = (plane_size) * (this->m_volumeDepth);

  //Viene creato l'array di byte che conterrà tutte le immagini...
  newScalars = vtkUnsignedCharArray::New();
  //..e dimensionato opportunamente.
  newScalars->SetNumberOfTuples(volume_size);

  // Lettura delle singole immagini....
  for( z_counter=0; z_counter<this->m_volumeDepth; z_counter++ )
    {

      if ( !scan_ptr )
	     scan_ptr = new unsigned char[plane_size];
      
	  if ( fread( scan_ptr, 1, plane_size, spf ) != (unsigned int)plane_size )
		{
		  this->Error=3;
          newScalars->Delete();
          return;
        }

	  sptr = scan_ptr;

	  // se ho letto l'immagine con successo....la memorizzo nell'array..
	  unsigned char c;
      for( i=0; i<plane_size; i++ )
	  {
		c=*sptr++;
		//if (c==0) c=155;
		 newScalars->SetValue( (z_counter*plane_size + i), c );
	  }
    }

	delete [] scan_ptr;

	//Setto l'uscita del filtro con i dati letti...
	if( newScalars )
	{	
      this->m_Output->GetPointData()->SetScalars(newScalars);
      newScalars->Delete();
	}

    fclose( spf );
	this->m_Output->SetUpdateExtent(0,this->m_volumeWidth-1,
					  0,this->m_volumeHeight-1,
					  0,this->m_volumeDepth-1);
	this->m_Output->Update();
	this->m_Output->UpdateInformation();
}

void vtkSPFReader::SetFileName(const char * FileName)
{
  this->FileName=FileName;
}

void vtkSPFReader::Init()
{
  this->m_Output->PrepareForNewData(); 
  this->m_Output->Initialize();
  this->m_Output->SetScalarType(VTK_UNSIGNED_CHAR);
  this->m_Output->SetNumberOfScalarComponents(1);
  this->m_Output->SetOrigin(0,0,0);
}

void vtkSPFReader::LoadHeader()
{
  FILE *spf;
  char line[100];
  float f_temp;
  unsigned short u_temp;
  
  if ((spf = fopen(this->FileName, "r")) == NULL)
    {
    return;
    }


  fscanf( spf,"%s",line);
  if (strcmp(line,"3DVolume")!=0) 
  {
    return;	
  }
  do
	{	 
	 fscanf( spf,"%s",line);

	 if (strcmp(line,"voxelWidth")==0)
	 {
	 	fscanf( spf,"%f",&f_temp);
		this->m_voxelWidth=f_temp;
	 }

	 if (strcmp(line,"voxelHeight")==0)		
	 {
		fscanf( spf,"%f",&f_temp);
		this->m_voxelHeight=f_temp;
	 }

	 if (strcmp(line,"voxelDepth")==0)		
	 {
		fscanf( spf,"%f",&f_temp);
		this->m_voxelDepth=f_temp;
	 }

	 if (strcmp(line,"volumeWidth")==0)		
	 {
	 	fscanf( spf,"%u",&u_temp);
		this->m_volumeWidth=u_temp;
	 }

	 if (strcmp(line,"volumeHeight")==0)	
	 {
		fscanf( spf,"%u",&u_temp);
		this->m_volumeHeight=u_temp;
	 }

	 if (strcmp(line,"volumeDepth")==0)		
	 {
		fscanf( spf,"%u",&u_temp);
		this->m_volumeDepth=u_temp;
	 }

	} while (strcmp(line,"data"));
  fscanf(spf,"%x",&u_temp);
  this->m_DataOffset=u_temp;

  fclose(spf);
 
}
