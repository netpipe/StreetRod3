/////////////////////////////////////////
//
//             Street Rod 3
//
// Copyright (C) Auxiliary Software 2002-2004
//
//
/////////////////////////////////////////
/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/


// File finding routines
// Created 30/9/01
// By Jason Boettcher


#include "defs.h"

#ifndef _LINUX
char	_dir[256];
long	handle = 0;
struct _finddata_t fileinfo;


///////////////////
// Find the first file
int FindFirst(char *dir, char *ext, char *filename)
{
	char basepath[256];

	strcpy(_dir,dir);

	strcpy(basepath, dir);
	strcat(basepath, "/");
	strcat(basepath, ext);

	if((handle = _findfirst(basepath, &fileinfo)) < 0)
		return false;

	do
	{
		//If file is not self-directory or parent-directory
		if(strcmp(fileinfo.name, "."))
		 if(strcmp(fileinfo.name, ".."))
		 {
			//If found file is not a directory
			if(!(fileinfo.attrib & _A_SUBDIR))
			{
				sprintf(filename,"%s\\%s",_dir,fileinfo.name);

				return true;
			}
		}

	} while(!_findnext(handle, &fileinfo));		// Keep going until we found the first file


	return false;
}


///////////////////
// Find the next file
int FindNext(char *filename)
{
	if(_findnext(handle, &fileinfo))
		return false;

	do
	{
		//If file is not self-directory or parent-directory
		if(strcmp(fileinfo.name, "."))
		 if(strcmp(fileinfo.name, ".."))
		 {
			//If found file is not a directory
			if(!(fileinfo.attrib & _A_SUBDIR))
			{
				sprintf(filename,"%s\\%s",_dir,fileinfo.name);

				return true;
			}
		}
	} while(!_findnext(handle, &fileinfo));		// Keep going until we found the next file


	return false;
}

#else
// For the Linux version, we must use glob().
#include <glob.h>
glob_t _glob_info;
size_t _glob_index;
char _pattern[256];

int FindFirst(char *dir, char *ext, char *filename)
{
  char *slash_ptr;
  sprintf(_pattern, "%s/%s", dir, ext);
  for (slash_ptr = strchr(_pattern, '\\');
       slash_ptr != NULL;
       slash_ptr = strchr(slash_ptr, '\\'))
  {
    *slash_ptr = '/';
  }
  if ((glob(_pattern, 0, NULL, &_glob_info) == 0) &&
      (_glob_info.gl_offs < _glob_info.gl_pathc))
  {
    strcpy(filename, _glob_info.gl_pathv[_glob_info.gl_offs]);
    _glob_index = _glob_info.gl_offs + 1;
    return true;
  }
  globfree(&_glob_info);
  return false;
}

int FindNext(char *filename)
{
  if (_glob_index < _glob_info.gl_pathc)
  {
    strcpy(filename, _glob_info.gl_pathv[_glob_index]);
    _glob_index ++;
    return true;
  }
  globfree(&_glob_info);
  return false;
}
#endif
