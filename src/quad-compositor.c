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

struct quad_compositor_data {
	obs_source_t *source;
	gs_effect_t *effect;
	gs_texture_t *output_texture;
};

void *quad_compositor_filter_create(obs_data_t *settings, obs_source_t *source)
{
	UNUSED_PARAMETER(settings);
	struct quad_compositor_data *filter_data =
		(struct quad_compositor_data *)bzalloc(
			sizeof(struct quad_compositor_data));

	// Retrieve any configuration settings from 'settings' and initialize the filter data

	filter_data->source = source;

	//effect file is found in the data folder
	char *filename = obs_module_file("quad_shader.effect");

	//Need to "enter" graphics to load the shader
	obs_enter_graphics();
	gs_effect_t *effect = gs_effect_create_from_file(filename, NULL);

	if (!effect) {
		blog(LOG_WARNING,
		     "[obs-mixed-reality-quad-compositor] Shader not loaded");
	} else {
		filter_data->effect = effect;
	}
	obs_leave_graphics();

	return filter_data;
}

//Shader based render implementation
void quad_compositor_filter_video(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);

	//Get the data from the data structure outlined above.
	struct quad_compositor_data *filter_data =
		(struct quad_compositor_data *)data;

	//Set a more friendly name for the source object
	obs_source_t *source = filter_data->source;

	if (!filter_data->effect) {
		obs_source_skip_video_filter(source);
		return;
	}
	
	//Begin rendering. Exit if it fails.
	if (!obs_source_process_filter_begin(source, GS_RGBA,
					     OBS_ALLOW_DIRECT_RENDERING)) {
		return;
	}

	//Store the current blend state before using our own
	gs_blend_state_push();
	gs_blend_function(GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);

	//Run the effect (shader) across the full frame
	obs_source_process_filter_end(source, filter_data->effect, 0, 0);

	//Retrieve the previous render state
	gs_blend_state_pop();
}

const char *quad_compositor_filter_get_name(void *data)
{
	UNUSED_PARAMETER(data);
	return "Quad Compositor Foreground";
}

void quad_compositor_filter_destroy(void *data)
{
	struct quad_compositor_data *filter_data =
		(struct quad_compositor_data *)data;

	// Cleanup and deallocate any resources used by the filter data

	bfree(filter_data);
}

void quad_compositor_filter_defaults(obs_data_t *settings)
{
	UNUSED_PARAMETER(settings);
}

struct obs_source_info quad_compositor_video = {
	.id = "quad_compositor",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_VIDEO,
	.get_name = quad_compositor_filter_get_name,
	.create = quad_compositor_filter_create,
	.destroy = quad_compositor_filter_destroy,
	.get_defaults = quad_compositor_filter_defaults,
	.video_render = quad_compositor_filter_video,
};

OBS_DECLARE_MODULE()

bool obs_module_load(void)
{
	obs_register_source(&quad_compositor_video);
	return true;
}
