/*
   Copyright © 2008, Ferruccio G. Barletta

   Permission is granted to copy, distribute and modify this source code in any
   context without fee, including commercial applications, provided that 
   this copyright notice is not removed or altered.

   http://the-lazy-programmer.com

*/

#pragma once

#include <indexsrv.h>

//
// word sink for IWordBreaker
//
class WordSink : public IWordSink
{
public :
   WordSink(void) : IWordSink()
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

   HRESULT STDMETHODCALLTYPE EndAltPhrase(void)
   {
      return NOERROR;
   }

   HRESULT STDMETHODCALLTYPE PutAltWord(ULONG cwc, WCHAR const *pwcInBuf, ULONG cwcSrcLen, ULONG cwcSrcPos)
   {
      return NOERROR;
   }

   HRESULT STDMETHODCALLTYPE PutBreak(WORDREP_BREAK_TYPE breakType)
   {
      return NOERROR;
   }

   HRESULT STDMETHODCALLTYPE PutWord(ULONG cwc, WCHAR const *pwcInBuf, ULONG cwcSrcLen, ULONG cwcSrcPos)
   {
      return NOERROR;
   }

   HRESULT STDMETHODCALLTYPE StartAltPhrase(void)
   {
      return NOERROR;
   }
};

class DocExtractor;

class MyWordSink : public WordSink
{
public :
   MyWordSink(DocExtractor& docex) : WordSink(), docex_(docex) {}

private :
   HRESULT STDMETHODCALLTYPE PutAltWord(ULONG cwc, WCHAR const *pwcInBuf, ULONG cwcSrcLen, ULONG cwcSrcPos);
   HRESULT STDMETHODCALLTYPE PutWord(ULONG cwc, WCHAR const *pwcInBuf, ULONG cwcSrcLen, ULONG cwcSrcPos);

   DocExtractor&  docex_;
};