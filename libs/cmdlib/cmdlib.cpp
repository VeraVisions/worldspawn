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

//
// start of shared cmdlib stuff
//

#include "cmdlib.h"
#include "globaldefs.h"

#include <string.h>
#include <stdio.h>

#include "string/string.h"
#include "os/path.h"
#include "container/array.h"


#if GDEF_OS_POSIX

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

bool Q_Exec( const char *cmd, char *cmdline, const char *, bool, bool waitfor ){
	char fullcmd[2048];
	char *pCmd;
	pid_t pid;
#if GDEF_DEBUG
	printf( "Q_Exec damnit\n" );
#endif
	switch ( ( pid = fork() ) )
	{
	default:
		if ( waitfor ) {
			waitpid( pid, NULL, 0 );
		}
		break;
	case -1:
		return true;
		break;
	case 0:
		// always concat the command on linux
		if ( cmd ) {
			strcpy( fullcmd, cmd );
		}
		else{
			fullcmd[0] = '\0';
		}
		if ( cmdline ) {
			strcat( fullcmd, " " );
			strcat( fullcmd, cmdline );
		}
		pCmd = fullcmd;
		while ( *pCmd == ' ' )
			pCmd++;
#if GDEF_DEBUG
		printf( "Running system...\n" );
		printf( "Command: %s\n", pCmd );
#endif
		system( pCmd );
#if GDEF_DEBUG
		printf( "system() returned\n" );
#endif
		_exit( 0 );
		break;
	}
	return true;
}

#elif GDEF_OS_WINDOWS

#include <windows.h>

// NOTE TTimo windows is VERY nitpicky about the syntax in CreateProcess
bool Q_Exec( const char *cmd, char *cmdline, const char *execdir, bool bCreateConsole, bool waitfor ){
	PROCESS_INFORMATION ProcessInformation;
	STARTUPINFO startupinfo = {0};
	DWORD dwCreationFlags;
	GetStartupInfo( &startupinfo );
	if ( bCreateConsole ) {
		dwCreationFlags = CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS;
	}
	else{
		dwCreationFlags = DETACHED_PROCESS | NORMAL_PRIORITY_CLASS;
	}
	const char *pCmd;
	char *pCmdline;
	pCmd = cmd;
	if ( pCmd ) {
		while ( *pCmd == ' ' )
			pCmd++;
	}
	pCmdline = cmdline;
	if ( pCmdline ) {
		while ( *pCmdline == ' ' )
			pCmdline++;
	}

	if ( CreateProcess(
		     pCmd,
		     pCmdline,
		     NULL,
		     NULL,
		     FALSE,
		     dwCreationFlags,
		     NULL,
		     execdir,
		     &startupinfo,
		     &ProcessInformation
		     ) ) {
		if ( waitfor ) {
			WaitForSingleObject( ProcessInformation.hProcess, INFINITE );
		}
		return true;
	}
	return false;
}

#endif
