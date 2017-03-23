#pragma once

/*
 * 	macro wrappers around the msvc uuid extensions
 * 	these let us attach a uuid to a class
 * 	possibly of use in templates. info could be packed into the id,
 * 	encoded use constexpr functions
 */

struct _GUID
{
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
};

using GUID = _GUID;

extern const GUID GUID_NULL;


#define		__uniqueId(string)				__declspec(uuid(string))
#define		__classId(expression)			__uuidof(expression)


class __uniqueId("12345678-1234-1234-1234-1234567890aB") __UUID_TEST
{
};

static constexpr const GUID&  ggtest = __classId(__UUID_TEST);
