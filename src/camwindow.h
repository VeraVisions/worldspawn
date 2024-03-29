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

#if !defined( INCLUDED_CAMWINDOW_H )
#define INCLUDED_CAMWINDOW_H

#include <uilib/uilib.h>
#include "math/vector.h"
#include "signal/signalfwd.h"

class CamWnd;

CamWnd *NewCamWnd();

void DeleteCamWnd(CamWnd *camwnd);

void AddCameraMovedCallback(const SignalHandler &handler);

void CamWnd_Update(CamWnd &camwnd);

ui::GLArea CamWnd_getWidget(CamWnd &camwnd);

void CamWnd_setParent(CamWnd &camwnd, ui::Window parent);

void GlobalCamera_setCamWnd(CamWnd &camwnd);

void fill_view_camera_menu(ui::Menu menu);

void CamWnd_constructToolbar(ui::Toolbar toolbar);

void CamWnd_registerShortcuts();

void GlobalCamera_Benchmark();

const Vector3 &Camera_getOrigin(CamWnd &camwnd);

void Camera_setOrigin(CamWnd &camwnd, const Vector3 &origin);

enum {
	CAMERA_PITCH = 0, // up / down
	CAMERA_YAW = 1, // left / right
	CAMERA_ROLL = 2, // fall over
};

const Vector3 &Camera_getAngles(CamWnd &camwnd);

void Camera_setAngles(CamWnd &camwnd, const Vector3 &angles);


struct camwindow_globals_t {
	Vector3 color_cameraback;
	Vector3 color_selbrushes3d;

	int m_nCubicScale;

	camwindow_globals_t() :
		color_cameraback(0.15f, 0.15f, 0.15f),
		color_selbrushes3d(1.0f, 0.0f, 0.0f),
		m_nCubicScale(13)
	{
	}

};

extern camwindow_globals_t g_camwindow_globals;

void CamWnd_Construct();

void CamWnd_Destroy();

#endif
