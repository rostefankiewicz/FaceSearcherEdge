///////////////////////////////////////////////////////////
// Copyright: (C) 2015 CyberExtruder.com Inc.
//                     www.cyberextruder.com
//
// DISCLAIMER:
//
// THIS SOFTWARE IS PROVIDED “AS IS” AND “WITH ALL FAULTS” AND WITHOUT
// WARRANTY OF ANY KIND.
// CUSTOMER AGREES THAT THE USE OF THIS SOFTWARE IS AT CUSTOMER'S RISK
// CYBEREXTRUDER MAKES NO WARRANTY OF ANY KIND TO CUSTOMER OR ANY THIRD 
// PARTY, EXPRESS, IMPLIED OR STATUTORY, WITH RESPECT TO THE THIS 
// SOFTWARE, OPERATION OF THE SOFTWARE, OR OUTPUT OF OR RESULTS OBTAINED 
// FROM THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTY 
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OPERABILITY OR 
// NON-INFRINGEMENT AND ALL SUCH WARRANTIES ARE HEREBY EXCLUDED BY 
// CYBEREXTRUDER AND WAIVED BY CUSTOMER.
///////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include "windows.h"
#include <io.h>
#include <process.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <fcntl.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
#include "Aureus.h"
#include "AureusHeads.h"
#include "AureusAnnotations.h"
#include "cxutils.h"
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#include "../jpeg/jpeglib.h"
}
#endif
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
void PrintHeadInfo(CX_HeadInfo& hi)
{
  printf("Frame num:%d Head ID:%d size(r,c)=(%d,%d)\n", hi.m_frame_number, hi.m_head_id, hi.m_rows, hi.m_cols);
  printf("head_ok=%s ", (hi.m_head_ok ? "true" : "false"));
  if (hi.m_head_ok)
  {
    printf("bl(%.2f,%.2f) tr(%.2f,%.2f)", hi.m_head_bl_x, hi.m_head_bl_y, hi.m_head_tr_x, hi.m_head_tr_y);
  }
  printf("\n");
  printf("face_ok=%s ", (hi.m_face_ok ? "true" : "false"));
  if (hi.m_face_ok)
  {
    printf("bl(%.2f,%.2f) tr(%.2f,%.2f)", hi.m_face_bl_x, hi.m_face_bl_y, hi.m_face_tr_x, hi.m_face_tr_y);
  }
  printf("\n");

  if (hi.m_has_annotation_set)
  {
    printf("conf: %f, focus: %f, has anns=%s\n", hi.m_confidence, hi.m_focus, (hi.m_has_annotation_set ? "true" : "false"));
  }
  else
  {
    printf("has anns=%s\n", (hi.m_has_annotation_set ? "true" : "false"));
  }
}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
void PrintHeadFR(CX_HEAD p_head)
{
  char msg[1024];
  CX_RankItem rank[2];

  int n = CX_GetHeadFRRankedResults(p_head, rank, 2, msg);
  if (n > 0)
  {
    printf(" Name: %s score: %f\n", rank[0].m_person_name, rank[0].m_score);
  }
  else printf("No FR \n");
}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
void PrintHeadData(int frame_number, int head_index, CX_HEAD p_head)
{
  char msg[1204];
  CX_HeadInfo hi;
  if (!CX_GetHeadInfo(p_head, &hi, msg))
  {
    printf("Frame %d: Failed to get head %d info: %s\n", frame_number, head_index, msg);
  }
  else
  {
    PrintHeadInfo(hi);
    PrintHeadFR(p_head);
  }
}
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
void PrintHeadListData(int frame_number, CX_HEAD_LIST p_head_list, bool details)
{
  char msg[1204];
  int n_heads = CX_GetHeadListSize(p_head_list, msg);
  if (n_heads < 0) printf("Frame %d Error: %s\n", frame_number, msg);
  else
  {
    if (details)
    {
      printf("FRAME %d, Tracking %d HEADS\n", frame_number, n_heads);
      for (int i = 0; i < n_heads; i++)
      {
        CX_HEAD p_head = CX_GetHead(p_head_list, i, msg);
        if (!p_head) printf("Failed to get head %d of %d heads: %s\n", i, n_heads, msg);
        else
        {
          PrintHeadData(frame_number, i, p_head);
        }
      }
    }
    else
    {
      printf("FRAME %d, Tracking %d HEADS ", frame_number, n_heads);
      for (int i = 0; i < n_heads; i++)
      {
        CX_HEAD p_head = CX_GetHead(p_head_list, i, msg);
        if (!p_head) printf("Failed to get head %d of %d heads: %s\n", i, n_heads, msg);
        else
        {
          char msg[1204];
          CX_HeadInfo hi;
          if (CX_GetHeadInfo(p_head, &hi, msg))
          {
            printf("Head ID:%d ", hi.m_head_id);
          }
        }
      }
      printf("\n");

    }
  }

}
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
void LoadFrames(const char* dir, std::vector<CX_RAM_Image>& frames, bool print_details)
{
  std::vector<std::string> fnames;
  CX_FindFiles(fnames, dir, "*.jpg");
  printf("Loading %d images from %s\n", (int)fnames.size(), dir);
  for (int i = 0; i < (int)fnames.size(); i++)
  {
    string& fname = fnames[i];
    frames.push_back(CX_RAM_Image());
    CX_RAM_Image& im = frames[i];

    if (LoadImageFromDisk(fname.c_str(), im))
    {
      if (print_details) printf("Loaded frame %d\n", i);
    }
    else
    {
      printf("FAILED TO LOAD FRAME %d\n", i);
      frames.pop_back(); // remove last as it was not loaded
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////
void DeleteFrames(std::vector<CX_RAM_Image>& frames)
{
  for (int i = 0; i < (int)frames.size(); i++)
  {
    CX_RAM_Image& im = frames[i];
    // free mem allocate for image pixels
    DeleteImagePixels(im);
  }
}
//////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//      UTILITY FUNCTIONS FOR LOADING JPEG IMAGES USING THE JPEG LIBRARY
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
void DeleteImagePixels(CX_RAM_Image& im)
{
  if (im.mp_pixels)  delete[] im.mp_pixels;
  im.mp_pixels = NULL;
}
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
bool LoadImageFromDisk(const char* fname, CX_RAM_Image& im)
{
  // load a JPEG image
  im.mp_pixels = NULL;
  int rows, cols;
  if (!ReadJPEG(fname, &im.mp_pixels, rows, cols))
  {
    printf("Failed to load %s\n", fname);
    im.mp_pixels = NULL;
    return false;
  }
  // set relevent values
  im.m_rows = (unsigned int)rows;
  im.m_cols = (unsigned int)cols;
  im.m_type = 1; // RGBA
  im.m_origin = 1; // ReadJPEG results in images with origin bottom left

  return true;
}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
bool LoadImageFromDisk(const char* fname, CX_Image& im)
{
  // load a JPEG image
  im.mp_pixels = NULL;
  int rows, cols;
  if (!ReadJPEG(fname, &im.mp_pixels, rows, cols))
  {
    printf("Failed to load %s\n", fname);
    im.mp_pixels = NULL;
    return false;
  }
  // set relevent values
  im.m_rows = (unsigned int)rows;
  im.m_cols = (unsigned int)cols;
  im.m_type = 1; // RGBA
  im.m_origin = 1; // ReadJPEG results in images with origin bottom left

  return true;
}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// returns a byte array of RGBA data
////////////////////////////////////////////////////////////////////////////////
bool ReadJPEG( const char* pszInfile, unsigned char** ppOutImage, int& nOutRows, int& nOutCols)
{
	nOutRows = 0;
	nOutCols = 0;
  
	// open the file up for reading
	FILE* hInfile = fopen( pszInfile, "rb");
	if( NULL == hInfile)
	{
		printf("Unable to open file %s\n",pszInfile);
		return false;
	}
	
	// first check it's a real jpeg 
  // the jpeg lib does this but exist upon failur, we want something more graceful
  unsigned char c1,c2;
  fread(&c1,sizeof(char),1,hInfile);
  fread(&c2,sizeof(char),1,hInfile);
  if (c1 != 0xFF || c2 != 0xD8) 
  {
    printf("ReadJPEG() Not a JPEG file first marker was %d %d\n",(int)c1, (int)c2);
    fclose(hInfile);
		return NULL;
  }
  
  // seek to beginning of file as it correctly passed the JPEG test
  fseek(hInfile,0,SEEK_SET);

  struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

  jpeg_stdio_src(&cinfo, hInfile);

  jpeg_read_header(&cinfo, TRUE);

  jpeg_start_decompress(&cinfo);


  if (cinfo.output_components > 4)  // normal RGB
  {
    //jpeg_destroy(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    printf("ReadJPEG(). Error, file format not supported!.\n");
		fclose(hInfile);
    return NULL;
  }
	
	int nRows = cinfo.output_height;
	int nCols = cinfo.output_width;
  
	int nWidth = (int)cinfo.output_components * nCols;
  JSAMPLE* pScanLines = new JSAMPLE[nWidth];
  
	unsigned char* pOutImage = new unsigned char[cinfo.output_height * cinfo.output_width * 4];
	
  int nCurRow = 0;
  while (cinfo.output_scanline < cinfo.output_height)
  {
		// read the image one line at a time
    (void)jpeg_read_scanlines( &cinfo, (JSAMPARRAY)&pScanLines, (JDIMENSION)1);

    if( cinfo.output_components == 4)
    {
      for( int nCol = 0; nCol < nCols; nCol++)
      {
        int nIndex = ((nRows-nCurRow-1) * nCols * 4) + (nCol * 4); // origin bottom left
        //int nIndex = (nCurRow * nCols * 4) + (nCol * 4); // origin top left
        int nC = nCol * cinfo.output_components;

        pOutImage[nIndex] = pScanLines[nC];
        pOutImage[nIndex + 1] = pScanLines[nC + 1];
        pOutImage[nIndex + 2] = pScanLines[nC + 2];
        pOutImage[nIndex + 3] = pScanLines[nC + 3];
      }
    }
    else if( cinfo.output_components == 3)
    {
      for( int nCol = 0; nCol < nCols; nCol++)
      {
        int nIndex = ((nRows - nCurRow - 1) * nCols * 4) + (nCol * 4); // origin bottom left
        //int nIndex = (nCurRow * nCols * 4) + (nCol * 4); // origin top left
        int nC = nCol * cinfo.output_components;
        
				pOutImage[nIndex] = pScanLines[nC];
				pOutImage[nIndex + 1] = pScanLines[nC + 1];
				pOutImage[nIndex + 2] = pScanLines[nC + 2]; 
				pOutImage[nIndex + 3] = 255; // set alpha to 255 if missing
      }
    }
    else
    {
      for( int nCol = 0; nCol < nCols; nCol++)
      {
        int nIndex = ((nRows - nCurRow - 1) * nCols * 4) + (nCol * 4); // origin bottom left
        //int nIndex = (nCurRow * nCols * 4) + (nCol * 4); // origin top left

				pOutImage[nIndex] = pScanLines[nCol];			// gray scale, set all pixels the same
				pOutImage[nIndex + 1] = pScanLines[nCol];
				pOutImage[nIndex + 2] = pScanLines[nCol]; 
				pOutImage[nIndex + 3] = 255;							// set alpha to 255 if missing
      }
    }
    nCurRow++;
  }

  // should put this in a try exception block, but this is example code.
  jpeg_finish_decompress(&cinfo);        // finish the decompression
  jpeg_destroy_decompress(&cinfo);       // and destroy the object (free the mem)

  //__try
  //{
  //  jpeg_finish_decompress(&cinfo);        // finish the decompression
  //  jpeg_destroy_decompress(&cinfo);       // and destroy the object (free the mem)
  //}
  //__except( EXCEPTION_EXECUTE_HANDLER )  // windows definition
  //{
  //	fclose(hInfile);
  //  return false;
  //}

	fclose(hInfile);
	
	*ppOutImage = pOutImage;
	nOutRows = nRows;
	nOutCols = nCols;

  delete [] pScanLines;
  
	return true;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//      END OF JPEG UTILITY FUNCTIONS
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//      UTILITY FUNCTIONS FOR GETTING A LIST OF FILENAMES FROM A FOLDER
//      USED TO GATHER THE FILES FOR FR TESTING
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// returns file extension (if one is not present, returns empty)
// does not return the '.' character
////////////////////////////////////////////////////////////////////////////////
std::string GetExtension(std::string& s)
{
  std::string kExtension;
  size_t found = s.find_last_of('.');
  
  if (found != std::string::npos)
  {
    kExtension = s.substr(found+1);
  }
  return kExtension;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// convert all chars to lower case
////////////////////////////////////////////////////////////////////////////////
void ForceLower(std::string& s)
{
  for (int unIndex=0; unIndex<(int)s.length(); unIndex++)
  {
    if( (s[unIndex] >= 'A') && (s[unIndex] <= 'Z') )
    {
      s[unIndex] = (char)((s[unIndex] - 'A') +'a');
    }
  }
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
std::string GetPath(std::string& s)
{
  std::string p;
  size_t found = s.find_last_of("/\\");
  if (found != std::string::npos)
  {
    p = s.substr(0,found);
  }
  return p;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
std::string GetPathAndFilename(std::string& s)
{
  std::string p;
  size_t found = s.find_last_of('.');
  if (found != std::string::npos)
  {
    p = s.substr(0,found);
  }
  return p;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
std::string GetFileName(std::string& s)
{
  std::string p = s;
  size_t last_dot_found = s.find_last_of('.');
  if (last_dot_found != std::string::npos)
  {
    p = s.substr(0,last_dot_found);
  }
  size_t last_slash_found = p.find_last_of("/\\");
  if (last_slash_found != std::string::npos)
  {
    p = p.substr(last_slash_found+1);
  }

  return p;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// finds a list of files matching the 'szWildCards' string in directory 'szPath',
// eg: FindFiles(kList,"C:\\Data\\Images", *.bmp");
// would return a list of all the BMP files in C:\Data\Images
// if non are found the func returns false otherwise it returns true
////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
bool CX_FindFiles(std::vector<std::string>& kFnames, const char* szPath, const char* szWildCards)
{
  struct _finddata_t c_file;
  intptr_t hFile;
  kFnames.clear();
  std::vector<std::string> files;
  std::string kPath = std::string(szPath);// + CE_String("\\");
	if( kPath.back() != '\\')	kPath += '\\';
  std::string kPathWithWildCards = kPath + std::string(szWildCards);

  // Find first .c file in current directory 
  if( (hFile = _findfirst( kPathWithWildCards.c_str(), &c_file )) == -1L )
  {
    return false;
  }
  else
  {
    std::string kFilename = kPath + std::string(c_file.name); // first file found
    files.push_back(kFilename);
    while( _findnext( hFile, &c_file ) == 0 )
    {
      kFilename = kPath + std::string(c_file.name);
      files.push_back(kFilename);
    }
    _findclose( hFile );
  }

  std::string ef(szWildCards);
  std::string efextn=GetExtension(ef);
  for (int i=0; i<(int)files.size(); i++)
  {
    std::string fname=files[i];
    std::string extn=GetExtension(fname);
    if (extn == efextn) kFnames.push_back(fname);
  }
  return true;
}
#else
bool CX_FindFiles(std::vector<std::string>& kFnames, const char* szPath, const char* szWildCards)
{
  DIR *dp = NULL;
  struct dirent *dptr = NULL;

  std::vector<std::string> files;
  std::string kPath = std::string(szPath);// + CE_String("\\");
  //if (kPath.back() != '\\')	kPath += '\\';
  if (kPath.back() != '/')	kPath += '/';


  if (NULL == (dp = opendir(szPath)))
  {
    //printf("\n Cannot open Input directory [%s]\n", szPath);
    return false;
  }

  while (NULL != (dptr = readdir(dp)))
  {
    std::string kFilename = kPath + std::string(dptr->d_name);
    files.push_back(kFilename);

    //printf("%s\n", dptr->d_name);
  }
  // Close the directory stream
  closedir(dp);

  std::string ef(szWildCards);
  std::string efextn = GetExtension(ef);
  for (int i = 0; i<files.size(); i++)
  {
    std::string fname = files[i];
    std::string extn = GetExtension(fname);
    if (extn == efextn) kFnames.push_back(fname);
  }
  return true;
}
#endif
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
bool FileExists(const char* fname)
{
#ifdef WIN32
  int fhandle = _open(fname, _O_BINARY | _O_RDONLY, 0);
  if (fhandle == -1) return false;
  _close(fhandle);
  return true;
#else
  int fhandle = open(fname, O_RDONLY, 0);
  if (fhandle == -1) return false;
  close(fhandle);
  return true;
#endif
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
CX_Image::CX_Image()
  : mp_pixels(NULL), m_rows(0), m_cols(0), m_type(1), m_origin(1)
{
}
////////////////////////////////////////////////////////////////////////////////
CX_Image::CX_Image(cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint type, cx_uint origin)
  : mp_pixels(NULL), m_rows(0), m_cols(0), m_type(1), m_origin(1)
{
  Clone(p_pixels, rows, cols, type, origin);
}
////////////////////////////////////////////////////////////////////////////////
CX_Image::~CX_Image()
{
  Empty();
}
////////////////////////////////////////////////////////////////////////////////
CX_Image::CX_Image(const CX_Image& im)
  : mp_pixels(NULL), m_rows(0), m_cols(0), m_type(1), m_origin(1)
{
  Clone(im);
}
////////////////////////////////////////////////////////////////////////////////
// copy assignement operator
CX_Image& CX_Image::operator = (const CX_Image& im)
{
  if (this != &im)
  {
    Clone(im);
  }
  return (*this);
}
////////////////////////////////////////////////////////////////////////////////
void CX_Image::Empty()
{
  if (mp_pixels) delete[] mp_pixels;
  mp_pixels = NULL;
  m_rows = 0;
  m_cols = 0;
  m_type = 1;
  m_origin = 1;
}
////////////////////////////////////////////////////////////////////////////////
cx_uint CX_Image::Size() const
{
  cx_uint n_pixels = m_rows * m_cols;
  if (m_type == 0 || m_type == 3) return n_pixels * 3;
  if (m_type == 1 || m_type == 4) return n_pixels * 4;
  if (m_type == 2) return n_pixels;
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
void CX_Image::Clone(const CX_Image& im)
{
  Clone(im.mp_pixels, im.m_rows, im.m_cols, im.m_type, im.m_origin);
}
////////////////////////////////////////////////////////////////////////////////
void CX_Image::Clone(cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint type, cx_uint origin)
{
  if (!p_pixels) return; // can't allocate an empty image

  cx_uint size = 0;
  cx_uint n_pixels = rows * cols;
  if (type == 0 || type == 3) size = n_pixels * 3;
  if (type == 1 || type == 4) size = n_pixels * 4;
  if (type == 2) size = n_pixels;

  // not already allocated
  if (!mp_pixels)
  {
    mp_pixels = new cx_byte[size]; // allocate
  }
  // check to see if the memory is already allocated
  else if (Size() != size)
  {
    delete[] mp_pixels; // remove previous allocation as it is not the correct size
    mp_pixels = new cx_byte[size]; // allocate
  }
  m_rows = rows;
  m_cols = cols;
  m_type = type;
  m_origin = origin;

  memcpy(mp_pixels, p_pixels, size);
}
////////////////////////////////////////////////////////////////////////////////
bool CX_Image::Load(const char* fname)
{
  Empty();
  return LoadImageFromDisk(fname, *this);
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// returns the path of the executable that calls this function
////////////////////////////////////////////////////////////////////////////////
bool GetExecutablePath(string& dir)
{
#ifdef WIN32
  char buf[2048];
  unsigned int n = (unsigned int)GetModuleFileNameA(NULL, (LPCH)buf, 2048);
  if (n == 0) return false;

  string fname(buf);
  dir = GetPath(fname);
  return true;
#else
  dir = "";
  pid_t pid = getpid();
  char buf[10];
  sprintf(buf, "%d", pid);
  std::string _link = "/proc/";
  _link.append(buf);
  _link.append("/exe");
  char proc[512];
  int ch = readlink(_link.c_str(), proc, 512);
  if (ch != -1)
  {
    proc[ch] = 0;
    dir = proc;
    std::string::size_type t = dir.find_last_of("/");
    if (t == std::string::npos) return false;
    dir = dir.substr(0, t);
    return true;
  }
  return false;
#endif
}
////////////////////////////////////////////////////////////////////////////////
