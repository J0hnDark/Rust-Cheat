#pragma once

#include "windefs.h"

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

BOOL SupFileExists(
  _In_ LPCWSTR FilePath
);

BOOL SupWriteResourceToDisk(
  _In_     LPCWSTR FilePath,
  _In_opt_ HMODULE Module,
  _In_     LPCWSTR ResourceName,
  _In_     LPCWSTR ResourceType
);

HANDLE SupCreateFile(
  _In_ LPCWSTR FilePath,
  _In_ ACCESS_MASK DesiredAccess,
  _In_ ULONG ShareMode,
  _In_ ULONG CreateDisposition
);

BOOL SupLookupErrorMessage(
  _In_  ULONG ErrorCode,
  _Out_ LPSTR Buffer,
  _In_  ULONG BufferSize
);

LPVOID SupGetKernelBase(
  _Out_opt_ PSIZE_T KernelSize
);

#if 0
{
#endif
#ifdef __cplusplus
}
#endif
//(c) Apflmus