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
   MyExtractor(void) : DocExtractor() {}

private :
   void Start(void)
   {
   }

   void Word(const wstring& word)
   {
      wcout << word << endl;
   }

   void AltWord(const wstring& word)
   {
      wcout << word << " ";
   }

   void Property(const wstring& name, const wstring& value)
   {
      wcout << L"property: " << name << L"=" << value << endl;
   }
};

int wmain(int argc, wchar_t** argv)
{
   for (int i = 1; i < argc; ++i)
   {
      try
      {
         wcout << L"Document: " << argv[i] << endl;
         wcout << L"----------------------------------------" << endl;
         MyExtractor ex;
         if (!ex.LoadWordBreaker())
         {
            wcout << "failed to load word breaker" << endl;
            return 1;
         }
         if (!ex.LoadWordStemmer())
         {
            wcout << "failed to load word stemmer" << endl;
            return 1;
         }

         ex.Extract(argv[i]);
      }
      catch (exception e)
      {
         wcout << L"exception thrown: " << e.what() << endl;
      }
   }

   return 0;
}

