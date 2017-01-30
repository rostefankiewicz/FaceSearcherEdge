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
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include "windows.h"
#endif
#include <string>
#include <array>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////////


using namespace std;


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
bool CE_FindFiles(std::vector<std::string>& kFnames, const char* szPath, const char* szWildCards);
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

