/*
   Copyright © 2008, Ferruccio G. Barletta

   Permission is granted to copy, distribute and modify this source code in any
   context without fee, including commercial applications, provided that 
   this copyright notice is not removed or altered.

   http://the-lazy-programmer.com

*/

#include "stdafx.h"
#include "WordBreaker.h"
#include "DocExtractor.h"

HRESULT STDMETHODCALLTYPE MyWordSink::PutAltWord(ULONG cwc, WCHAR const *pwcInBuf, ULONG cwcSrcLen, ULONG cwcSrcPos)
{
   wstring word = L"";
   word.reserve(cwc);
   while (cwc--)
      word += *pwcInBuf++;
   docex_.AltWord(word);
   return NOERROR;
}

HRESULT STDMETHODCALLTYPE MyWordSink::PutWord(ULONG cwc, WCHAR const *pwcInBuf, ULONG cwcSrcLen, ULONG cwcSrcPos)
{
   wstring word = L"";
   word.reserve(cwc);
   while (cwc--)
      word += *pwcInBuf++;
   docex_.Word(word);
   return NOERROR;
}