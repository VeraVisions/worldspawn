/*
   Copyright (C) 1999-2006 Id Software, Inc. and contributors.
   For a list of contributors, see the accompanying CONTRIBUTORS file.

   This file is part of GtkRadiant.

   GtkRadiant is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   GtkRadiant is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GtkRadiant; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "console.h"

#include <time.h>
#include <uilib/uilib.h>

#include "stream/stringstream.h"
#include "convert.h"

#include "mainframe.h"

std::size_t Sys_Print(int level, const char *buf, std::size_t length)
{
	StringOutputStream name(256);
	name << StringRange(buf, buf + length);
	printf(name.c_str());
	return length;
}


class SysPrintOutputStream : public TextOutputStream {
public:
    std::size_t write(const char *buffer, std::size_t length)
    {
        return Sys_Print(SYS_STD, buffer, length);
    }
};

class SysPrintErrorStream : public TextOutputStream {
public:
    std::size_t write(const char *buffer, std::size_t length)
    {
        return Sys_Print(SYS_ERR, buffer, length);
    }
};

SysPrintOutputStream g_outputStream;

TextOutputStream &getSysPrintOutputStream()
{
    return g_outputStream;
}

SysPrintErrorStream g_errorStream;

TextOutputStream &getSysPrintErrorStream()
{
    return g_errorStream;
}
