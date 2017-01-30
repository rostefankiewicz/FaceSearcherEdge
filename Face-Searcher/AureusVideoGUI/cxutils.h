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
#pragma once
//////////////////////////////////////////////////////////////////////////////////////
// Include the SDK utils
#include "CX_Aureus.h"
#include "Aureus.h"
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include "windows.h"
#else
#include <string.h>
#endif
#include <string>
#include <array>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////////


using namespace std;


/////////////////////////////////////////////////////////////////////
#define CX_MIN(x,y)     (x < y ? x : y)
#define CX_MAX(x,y)     (x > y ? x : y)
#define CX_MAX3(x,y,z)     (x > y ? CX_MAX(x,z) : CX_MAX(y,z))
#define CX_MIN3(x,y,z)     (x < y ? CX_MIN(x,z) : CX_MIN(y,z))
#define CX_CLIP(v,minv,maxv) ( CX_MAX( CX_MIN(v,maxv), minv ) )
#define CX_ABS(x) (x < 0 ? -x : x)
#define CX_SIGN(x) (x < 0 ? -1 : 1)
#define CX_POSITIVE(x) (x < 0 ? 0 : 1)
inline cx_int cx_round(const cx_real rX) { return ((cx_int)(rX < 0 ? (rX - 0.5f) : (rX + 0.5f))); };
inline cx_int cx_abs(const cx_int X) { return CX_ABS(X); };
inline cx_real cx_fabs(const cx_real X) { return CX_ABS(X); };
inline cx_real cx_sin(const cx_real rX) { return (cx_real)sin((double)rX); };
inline cx_real cx_cos(const cx_real rX) { return (cx_real)cos((double)rX); };
inline cx_real cx_ceil(const cx_real rX) { return (cx_real)ceil((double)rX); };
inline cx_real cx_floor(const cx_real rX) { return (cx_real)floor((double)rX); };
#define CX_PI  ((cx_real)3.14159265358979323846264338327950288419716939937511)
#define CX_DEGREESTORADIANS 1.7453292519943295769236907684883e-2
#define CX_RADIANSTODEGREES 57.295779513082320876798154814114
inline cx_real cx_deg2rad(const cx_real rX) { return (cx_real)(rX*CX_DEGREESTORADIANS); };
inline cx_real cx_rad2deg(const cx_real rX) { return (cx_real)(rX*CX_RADIANSTODEGREES); };
inline cx_real cx_sqr(const cx_real rX) { return rX*rX; };
/////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// various utility functions
void PrintHeadInfo(CX_HeadInfo& hi);
void PrintHeadData(int frame_number, int head_index, CX_HEAD p_head);
void PrintHeadListData(int frame_number, CX_HEAD_LIST p_head_list, bool details = true);
void LoadFrames(const char* dir, std::vector<CX_RAM_Image>& frames, bool print_details = true);
void DeleteFrames(std::vector<CX_RAM_Image>& frames);
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//      UTILITY FUNCTIONS FOR LOADING JPEG IMAGES USING THE JPEG LIBRARY
//
////////////////////////////////////////////////////////////////////////////////
// returns a byte array of RGBA data
////////////////////////////////////////////////////////////////////////////////
bool ReadJPEG( const char* pszInfile, unsigned char** ppOutImage, int& nOutRows, int& nOutCols);
bool LoadImageFromDisk(const char* fname, CX_RAM_Image& im);
void DeleteImagePixels(CX_RAM_Image& im);
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//      UTILITY FUNCTIONS FOR GETTING A LIST OF FILENAMES FROM A FOLDER
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// returns file extension (if one is not present, returns empty)
// does not return the '.' character
////////////////////////////////////////////////////////////////////////////////
std::string GetExtension(std::string& s);          // returns the part after the last '.'
std::string GetPath(std::string& s);               // returns the part before the last '/' or '\\'
std::string GetPathAndFilename(std::string& s);    // returns the part before the last '.'
std::string GetFileName(std::string& s);           // returns the part between the last '/' or '\\' and the last '.'
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// checks whether a file exists
bool FileExists(const char* fname);
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// convert all chars to lower case
////////////////////////////////////////////////////////////////////////////////
void ForceLower(std::string& s);
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// finds a list of files matching the 'szWildCards' string in directory 'szPath',
// eg: FindFiles(kList,"C:\\Data\\Images", *.bmp");
// would return a list of all the BMP files in C:\Data\Images
// if non are found the func returns false otherwise it returns true
////////////////////////////////////////////////////////////////////////////////
bool CX_FindFiles(std::vector<std::string>& kFnames, const char* szPath, const char* szWildCards);
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// returns the path of the executable that calls this function
////////////////////////////////////////////////////////////////////////////////
bool GetExecutablePath(string& dir);
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// A templated Sort Insert function, 
// The templated type must have the '<' operator defined
// Sorts so that kList(0) contains the smallest value and kList(kList.m_elems-1)
// has the highest. Returns the index at which the value was inserted.
////////////////////////////////////////////////////////////////////////////////
template <class T>
int SortInsert(vector<T>& kList, const T& tElement)
{
  bool found = false;
  int index = 0;
  for (typename vector<T>::iterator it = kList.begin(); it<kList.end(); it++, index++)
  {
    if (tElement<kList[index])
    {
      kList.insert(it, tElement);
      found = true;
      break;
    }
  }
  if (!found)
  {
    kList.push_back(tElement);
    index = (int)kList.size() - 1;
  }
  return index;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// A templated Sort Insert function, 
// The templated type must have the '<' operator defined
// Sorts so that kList(0) contains the smallest value and kList(kList.m_elems-1)
// has the highest. Returns the index at which the value was inserted.
// This function however limits the number of list items to be compared to, hence
// if n_max = (say) 10, then only the first 10 elements in the list will be compared
// this makes rank sorting (up to rank 10) hugely faster as we are only comparing
// 10 elements rather than (potentially) hundreds of thousands
////////////////////////////////////////////////////////////////////////////////
template <class T>
int SortInsertLimited(vector<T>& kList, const T& tElement, int n_max)
{
  int n = n_max;
  if (n>kList.size()) n = (int)kList.size();

  typename vector<T>::iterator it = kList.begin();
  for (int i = 0; i<n; i++, it++)
  {
    if (tElement < kList[i])
      //if (tElement < *it) 
    {
      //kList.insert(i,tElement);
      kList.insert(it, tElement);
      return (int)i;
    }
  }
  if (kList.size() >= n_max) return -1;
  kList.push_back(tElement);
  return (int)kList.size() - 1;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// some simple point classes
class CX_Vector3D
{
  public:
    cx_real m_data[3];
    const cx_real& x() const { return m_data[0]; }; 
    const cx_real& y() const { return m_data[1]; };
    const cx_real& z() const { return m_data[2]; };
    cx_real& x() { return m_data[0]; };             
    cx_real& y() { return m_data[1]; };             
    cx_real& z() { return m_data[2]; };            

    void operator -= (const CX_Vector3D &kVec){ for (cx_uint i = 0; i < 3; i++) m_data[i] -= kVec.m_data[i]; };
    void operator += (const CX_Vector3D &kVec){ for (cx_uint i = 0; i < 3; i++) m_data[i] += kVec.m_data[i]; };;
    CX_Vector3D operator - (const CX_Vector3D &kVec) { return CX_Vector3D(m_data[0] - kVec.m_data[0], m_data[1] - kVec.m_data[1], m_data[2] - kVec.m_data[2]); };
    CX_Vector3D operator + (const CX_Vector3D &kVec) { return CX_Vector3D(m_data[0] + kVec.m_data[0], m_data[1] + kVec.m_data[1], m_data[2] + kVec.m_data[2]); };

    void Clip(cx_real rLow, cx_real rHigh){ for (cx_uint i = 0; i < 3; i++) CX_CLIP(m_data[i],rLow,rHigh); }
    void MultiplyElements(const CX_Vector3D &kVec){ for (cx_uint i = 0; i < 3; i++) m_data[i] *= kVec.m_data[i]; };
    void DivideElements(const CX_Vector3D &kVec){ for (cx_uint i = 0; i < 3; i++) m_data[i] /= kVec.m_data[i]; };
    cx_real Mag() { return sqrt((m_data[0] * m_data[0]) + (m_data[1] * m_data[1]) + (m_data[2] * m_data[2])); };

    void operator *= (const cx_real& r){ for (cx_uint i = 0; i < 3; i++) m_data[i] *= r; };
    void operator /= (const cx_real& r){ for (cx_uint i = 0; i < 3; i++) m_data[i] /= r; };
    void operator += (const cx_real& r){ for (cx_uint i = 0; i < 3; i++) m_data[i] += r; };
    void operator -= (const cx_real& r){ for (cx_uint i = 0; i < 3; i++) m_data[i] -= r; };

    void operator()(cx_real x, cx_real y, cx_real z){ m_data[0] = x; m_data[1] = y; m_data[2] = z; };
    CX_Vector3D(cx_real x, cx_real y, cx_real z){ m_data[0] = x; m_data[1] = y; m_data[2] = z; };
    CX_Vector3D(){ memset(m_data, 0, 3 * sizeof(cx_real)); };
};
////////////////////////////////////////////////////////////////////////////////
inline CX_Vector3D Cross3D(const CX_Vector3D& kVec1, const CX_Vector3D& kVec2)
{
  return CX_Vector3D((kVec1.m_data[1] * kVec2.m_data[2]) - (kVec1.m_data[2] * kVec2.m_data[1]),
    (kVec1.m_data[2] * kVec2.m_data[0]) - (kVec1.m_data[0] * kVec2.m_data[2]),
    (kVec1.m_data[0] * kVec2.m_data[1]) - (kVec1.m_data[1] * kVec2.m_data[0]));

}
////////////////////////////////////////////////////////////////////////////////
inline cx_real Distance(const CX_Vector3D& kVec1, const CX_Vector3D& kVec2)
{
  return sqrt(cx_sqr(kVec1.m_data[0] - kVec2.m_data[0]) +
    cx_sqr(kVec1.m_data[1] - kVec2.m_data[1]) +
    cx_sqr(kVec1.m_data[2] - kVec2.m_data[2]));
}
////////////////////////////////////////////////////////////////////////////////
class CX_Vector2D
{
public:
  cx_real m_data[2];
  const cx_real& x() const { return m_data[0]; };
  const cx_real& y() const { return m_data[1]; };
  cx_real& x() { return m_data[0]; };
  cx_real& y() { return m_data[1]; };

  void operator -= (const CX_Vector2D &kVec){ for (cx_uint i = 0; i < 2; i++) m_data[i] -= kVec.m_data[i]; };
  void operator += (const CX_Vector2D &kVec){ for (cx_uint i = 0; i < 2; i++) m_data[i] += kVec.m_data[i]; };;
  CX_Vector2D operator - (const CX_Vector2D &kVec) { return CX_Vector2D(m_data[0] - kVec.m_data[0], m_data[1] - kVec.m_data[1]); };
  CX_Vector2D operator + (const CX_Vector2D &kVec) { return CX_Vector2D(m_data[0] + kVec.m_data[0], m_data[1] + kVec.m_data[1]); };

  void Clip(cx_real rLow, cx_real rHigh){ for (cx_uint i = 0; i < 2; i++) CX_CLIP(m_data[i], rLow, rHigh); }
  void MultiplyElements(const CX_Vector2D &kVec){ for (cx_uint i = 0; i < 2; i++) m_data[i] *= kVec.m_data[i]; };
  void DivideElements(const CX_Vector2D &kVec){ for (cx_uint i = 0; i < 2; i++) m_data[i] /= kVec.m_data[i]; };

  void operator *= (const cx_real& r){ for (cx_uint i = 0; i < 2; i++) m_data[i] *= r; };
  void operator /= (const cx_real& r){ for (cx_uint i = 0; i < 2; i++) m_data[i] /= r; };
  void operator += (const cx_real& r){ for (cx_uint i = 0; i < 2; i++) m_data[i] += r; };
  void operator -= (const cx_real& r){ for (cx_uint i = 0; i < 2; i++) m_data[i] -= r; };

  void operator()(cx_real x, cx_real y){ m_data[0] = x; m_data[1] = y; }; 
  CX_Vector2D(cx_real x, cx_real y){ m_data[0] = x; m_data[1] = y; };
  CX_Vector2D(){ memset(m_data, 0, 2 * sizeof(cx_real)); };
};
////////////////////////////////////////////////////////////////////////////////
class cx_coord
{
  public:
    cx_int r, c;

    void operator -= (const cx_coord &p){ r -= p.r; c -= p.c; };
    void operator += (const cx_coord &p){ r += p.r; c += p.c; };;
    cx_coord operator - (const cx_coord &p) { return cx_coord(r - p.r, c - p.c); };
    cx_coord operator + (const cx_coord &p) { return cx_coord(r + p.r, c + p.c); };

    void operator()(cx_int ri, cx_int ci){ r = ri; c = ci; };
    cx_coord(cx_int ri, cx_int ci){ r = ri; c = ci; };
    cx_coord(){};
    ~cx_coord(){};
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// a simple class for an image
////////////////////////////////////////////////////////////////////////////////
class CX_Image
{
  public:
    CX_Image(cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint type, cx_uint origin);
    CX_Image();
    ~CX_Image();

    // copy constructor 
    CX_Image(const CX_Image& im);

    // copy assignement operator
    CX_Image& operator = (const CX_Image& im);

    bool Load(const char* fname);
    void Clone(const CX_Image& im);
    void Clone(cx_byte* p_pixels, cx_uint rows, cx_uint cols, cx_uint type, cx_uint origin);
    void Empty();
    cx_uint Size() const;
    CX_Vector2D RealSize(){ return CX_Vector2D((cx_real)m_cols, (cx_real)m_rows); };

    cx_byte*    mp_pixels;
    cx_uint     m_rows;
    cx_uint     m_cols;
    cx_uint     m_type;    // 0=RGB, 1=RGBA, 2=Gray, 3=BGR, 4=BGRA
    cx_uint     m_origin;  // 0=top left,  1=bottom left
};
////////////////////////////////////////////////////////////////////////////////
