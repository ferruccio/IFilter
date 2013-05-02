/*
   Copyright © 2008, Ferruccio G. Barletta

   Permission is granted to copy, distribute and modify this source code in any
   context without fee, including commercial applications, provided that 
   this copyright notice is not removed or altered.

   http://the-lazy-programmer.com

*/

#pragma once

#include <indexsrv.h>

class StemmerSink : public IWordFormSink
{
public :
   StemmerSink() : IWordFormSink()
   {
   }

protected :
   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObj)
   {
      return E_FAIL;
   }

   ULONG STDMETHODCALLTYPE AddRef(void)
   {
      return 1;
   }

   ULONG STDMETHODCALLTYPE Release(void)
   {
      return 0;
   }

   HRESULT STDMETHODCALLTYPE PutAltWord(WCHAR const *pwcInBuf, ULONG cwc)
   {
      return NOERROR;
   }

   HRESULT STDMETHODCALLTYPE PutWord(WCHAR const *pwcInBuf, ULONG cwc)
   {
      return NOERROR;
   }
};

class DocExtractor;

class MyWordStemmerSink : public StemmerSink
{
public :
   MyWordStemmerSink(DocExtractor& docex) : StemmerSink(), docex_(docex) {}

private :
   HRESULT STDMETHODCALLTYPE PutAltWord(WCHAR const *pwcInBuf, ULONG cwc);
   HRESULT STDMETHODCALLTYPE PutWord(WCHAR const *pwcInBuf, ULONG cwc);

   DocExtractor&  docex_;
};