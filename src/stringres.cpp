/*
 * stringres - Print string resource from Windows library by index.
 *
 * Copyright (C) 2017  Drew Chapin
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Windows.h>
#include <tchar.h>

void error( LPTSTR msg, ... )
{
	va_list args = NULL;
	va_start(args,msg);
	DWORD dwError = GetLastError();
	LPVOID str = NULL;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_STRING,
		NULL,dwError,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&str,0,&args);
	_ftprintf_s(stderr,TEXT("Error %i: %s"),dwError,str);
	LocalFree(str);
	va_end(args);
}

int _tmain( int argc, LPTSTR argv[] )
{

	if( argc != 3 )
	{
		_tprintf_s(TEXT("stringres FILE INDEX\n"));
		_tprintf_s(TEXT("    FILE     Path to the library to read the string from.\n"));
		_tprintf_s(TEXT("    INDEX    Index number of the string to read.\n"));
		return 1;
	}

	HMODULE hdll = LoadLibrary(argv[1]);
	if( hdll != NULL )
	{
		LPTSTR res = NULL;
		size_t size = LoadString(hdll,_tstoi((LPCTSTR)argv[2]),(LPTSTR)&res,0);
		if( size > 0 )
		{
			LPTSTR str = (LPTSTR)calloc(size+1,sizeof(TCHAR));
			_tcsnccpy_s(str,size+1,res,size);
			_tprintf_s(TEXT("%s\n"),str);
		}
		else
			error(argv[2]);
		FreeLibrary(hdll);
		return 0;
	}
	else
		error(argv[1]);
	return 1;

}