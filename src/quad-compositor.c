/*
Mixed Reality Quad Compositor
Copyright (C) 2023 Hacked Mixed Reality <hackedmixedreality@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>

OBS_DECLARE_MODULE()

extern struct obs_source_info background_filter_info;
extern struct obs_source_info foreground_filter_info;

bool obs_module_load(void)
{
	obs_register_source(&background_filter_info);
	obs_register_source(&foreground_filter_info);
	return true;
}
