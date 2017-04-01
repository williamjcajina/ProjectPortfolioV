#pragma once


#ifdef FBX_EXPORTER_EXPORTS
#define FBX_READER_API __declspec(dllexport)
#else
#define FBX_READER_API __declspec(dllimport)
#endif


FBX_READER_API int getnumber(const char * filename);
