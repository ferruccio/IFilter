/*
   Copyright © 2008, Ferruccio G. Barletta

   Permission is granted to copy, distribute and modify this source code in any
   context without fee, including commercial applications, provided that 
   this copyright notice is not removed or altered.

   http://the-lazy-programmer.com

*/

#include "stdafx.h"
#include "WordBreaker.h"
#include "WordStemmer.h"
#include "DocExtractor.h"

HRESULT STDMETHODCALLTYPE MyWordStemmerSink::PutAltWord(WCHAR const *pwcInBuf, ULONG cwc)
{
   wstring word = L"";
   word.reserve(cwc);
   while (cwc--)
      word += *pwcInBuf++;
   docex_.AltWord(word);
   return NOERROR;
}

HRESULT STDMETHODCALLTYPE MyWordStemmerSink::PutWord(WCHAR const *pwcInBuf, ULONG cwc)
{
   wstring word = L"";
   word.reserve(cwc);
   while (cwc--)
      word += *pwcInBuf++;
   docex_.Word(word);
   return NOERROR;
}