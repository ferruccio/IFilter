/*
   Copyright © 2008, Ferruccio G. Barletta

   Permission is granted to copy, distribute and modify this source code in any
   context without fee, including commercial applications, provided that 
   this copyright notice is not removed or altered.

   http://the-lazy-programmer.com

*/

#include "stdafx.h"
#include "DocExtractor.h"

class MyExtractor : public DocExtractor
{
public :
   MyExtractor(void) : DocExtractor() { text_ = L""; }

   const wstring& GetText(void) { return text_; }

private :

   void Start(void)
   {
      text_ = L"";
   }

   void Text(const std::wstring& text)
   {
      text_ += text;
   }

   void Property(const wstring& name, const wstring& value)
   {
      wcout << L"property: " << name << L"=" << value << endl;
   }

   wstring   text_;
};

int wmain(int argc, wchar_t** argv)
{
   for (int i = 1; i < argc; ++i)
   {
      try
      {
         wcout << L"Document: " << argv[i] << endl << L"----------------------------------------" << endl;
         MyExtractor ex;
         ex.Extract(argv[i]);
         wcout << L"Text:" << endl << L"----------------------------------------" << endl;
         wcout << ex.GetText() << endl << endl;
      }
      catch (exception e)
      {
         wcout << L"exception thrown: " << e.what() << endl;
      }
   }

	return 0;
}

