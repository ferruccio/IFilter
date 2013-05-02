/*
   Copyright © 2008, Ferruccio G. Barletta

   Permission is granted to copy, distribute and modify this source code in any
   context without fee, including commercial applications, provided that 
   this copyright notice is not removed or altered.

   http://the-lazy-programmer.com

*/

#pragma once

#include <windows.h>
#include <filter.h>
#include <ntquery.h>
#include <filterr.h>
#include <propidl.h>

#include <string>
using namespace std;

class DocExtractor
{
public :
   DocExtractor(void);
   virtual ~DocExtractor(void);

   void Extract(const TCHAR* filename);

protected :
   virtual void Start(void) {}
   virtual void Text(const wstring& text) {}
   virtual void Property(const wstring& name, const wstring& value) {}
   virtual void Finish(void) {}

private :
   void ProcessValueChunk(IFilter *pFilter, STAT_CHUNK& stat);
   void ProcessTextChunk(IFilter *pFilter, STAT_CHUNK& stat);
};