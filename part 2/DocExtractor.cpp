/*
Copyright © 2008, Ferruccio G. Barletta

Permission is granted to copy, distribute and modify this source code in any
context without fee, including commercial applications, provided that 
this copyright notice is not removed or altered.

http://the-lazy-programmer.com

*/

#include "DocExtractor.h"
#include "WordBreaker.h"

//
// ctor/dtor
//
DocExtractor::DocExtractor(void)
{
   CoInitializeEx(0, COINIT_MULTITHREADED);
}

DocExtractor::~DocExtractor(void)
{
   CoUninitialize();
}

//
// extract text/properties from a document
//
void DocExtractor::Extract(const TCHAR* filename)
{
   IFilter *pFilter = 0;
   HRESULT hr = LoadIFilter(filename, 0, (void **) &pFilter);
   if (SUCCEEDED(hr))
   {
      DWORD flags = 0;
      HRESULT hr = pFilter->Init(IFILTER_INIT_INDEXING_ONLY |
         IFILTER_INIT_APPLY_INDEX_ATTRIBUTES |
         IFILTER_INIT_APPLY_CRAWL_ATTRIBUTES |
         IFILTER_INIT_FILTER_OWNED_VALUE_OK |
         IFILTER_INIT_APPLY_OTHER_ATTRIBUTES,
         0, 0, &flags);
      if (FAILED(hr))
      {
         pFilter->Release();
         throw exception("IFilter::Init() failed");
      }

      Start();

      STAT_CHUNK stat;
      while (SUCCEEDED(hr = pFilter->GetChunk(&stat)))
      {
         if ((stat.flags & CHUNK_TEXT) != 0)
            ProcessTextChunk(pFilter, stat);

         if ((stat.flags & CHUNK_VALUE) != 0)
            ProcessValueChunk(pFilter, stat);
      }

      Finish();

      pFilter->Release();      
   }
   else
   {
      throw exception("LoadIFilter() failed");
   }
}

//
// process a value chunk
//
void DocExtractor::ProcessValueChunk(IFilter *pFilter, STAT_CHUNK& stat)
{
   wstring propName;

   // get property name
   switch (stat.attribute.psProperty.ulKind)
   {
   case PRSPEC_LPWSTR :
      propName = stat.attribute.psProperty.lpwstr;
      break;

   case PRSPEC_PROPID :
      switch (stat.attribute.psProperty.propid)
      {
      case PIDSI_TITLE :         propName = L"Title";          break;
      case PIDSI_SUBJECT :       propName = L"Subject";        break;
      case PIDSI_AUTHOR :        propName = L"Author";         break;
      case PIDSI_KEYWORDS :      propName = L"Keywords";       break;
      case PIDSI_COMMENTS :      propName = L"Comments";       break;
      case PIDSI_TEMPLATE :      propName = L"Template";       break;
      case PIDSI_LASTAUTHOR :    propName = L"LastAuthor";     break;
      case PIDSI_REVNUMBER :     propName = L"RevNumber";      break;
      case PIDSI_EDITTIME :      propName = L"EditTime";       break;
      case PIDSI_LASTPRINTED :   propName = L"LastPrinted";    break;
      case PIDSI_CREATE_DTM :    propName = L"Created";        break;
      case PIDSI_LASTSAVE_DTM :  propName = L"LastSaved";      break;
      case PIDSI_PAGECOUNT :     propName = L"PageCount";      break;
      case PIDSI_WORDCOUNT :     propName = L"WordCount";      break;
      case PIDSI_CHARCOUNT :     propName = L"CharCount";      break;
      case PIDSI_APPNAME :       propName = L"AppName";        break;
      default :                  propName = L"?";
      }
      break;
   }

   // get property value
   wstring propValue = L"";
   HRESULT hr = 0;
   PROPVARIANT *pv = 0;
   while (SUCCEEDED(hr = pFilter->GetValue(&pv)))
   {
      wstring prop;
      HRESULT hr = pFilter->GetValue(&pv);
      if (pv != 0)
      {
         switch (pv->vt)
         {
         case VT_LPWSTR :
            prop = pv->pwszVal;
            break;

         case VT_I4 :
            // TODO: convert pv->intVal to string
            break;

         case VT_FILETIME :
            // TODO: convert pv->filetime to string
            break;
         }
         CoTaskMemFree(pv);
      }

      if (propValue.length() != 0)
         propValue += L", ";
      propValue += prop;
   }

   Property(propName, propValue);
}

//
// BreakText() callback, tell it there's no more text
//
HRESULT __stdcall FillTextBuffer(TEXT_SOURCE *pTextSource)
{
   return FILTER_E_NO_MORE_TEXT;
}

//
// process a text chunk
//
void DocExtractor::ProcessTextChunk(IFilter *pFilter, STAT_CHUNK& stat)
{
   const size_t RBUFSIZE = 8 * 1024;

   bool done = false;
   while (!done)
   {
      wchar_t rbuf[RBUFSIZE];
      ULONG bufsize = RBUFSIZE;
      HRESULT hr = pFilter->GetText(&bufsize, rbuf);
      switch (hr)
      {
      case FILTER_E_NO_MORE_TEXT :
         done = true;
         break;

      case FILTER_S_LAST_TEXT :
         done = true;
         // fall through
      default :
         if (SUCCEEDED(hr) && bufsize > 0)
         {
            TEXT_SOURCE ts;
            ts.pfnFillTextBuffer = FillTextBuffer;
            ts.awcBuffer = rbuf;
            ts.iCur = 0;
            ts.iEnd = bufsize;
            MyWordSink ws(*this);
            pWordBreaker_->BreakText(&ts, &ws, 0);
         }
      }
   }
}

bool DocExtractor::LoadWordBreaker(void)
{
   const wchar_t* keyName = L"SYSTEM\\CurrentControlSet\\Control\\ContentIndex\\Language\\English_US";
   HKEY hKey = 0;
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyName, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
   {
      wchar_t wordBreakerClass[MAX_PATH];
      DWORD size = sizeof(wordBreakerClass);
      DWORD type = 0;
      LSTATUS status = RegQueryValueEx(hKey, L"WBreakerClass", 0, &type, (BYTE *) wordBreakerClass, &size);
      RegCloseKey(hKey);

      if (status == ERROR_SUCCESS || type == REG_SZ)
      {
         // locate word breaker
         CLSID clsidWordBreak;
         if (FAILED(CLSIDFromString(wordBreakerClass, &clsidWordBreak)))
            return false;

         // create word breaker
         if (FAILED(CoCreateInstance(clsidWordBreak, 0, CLSCTX_ALL, IID_IWordBreaker, (void **) &pWordBreaker_)))
            return false;

         // initialize word breaker
         BOOL bLic = 0;
         if (FAILED(pWordBreaker_->Init(FALSE, 100, &bLic)))
         {
            pWordBreaker_->Release();
            return false;
         }

         return true;
      }
   }

   return false;
}
