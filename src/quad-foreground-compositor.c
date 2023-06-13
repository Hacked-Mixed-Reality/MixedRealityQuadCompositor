#include <obs-module.h>

struct foreground_filter_data {
	obs_source_t *source;
	gs_effect_t *effect;
	gs_texture_t *output_texture;
};

void *foreground_filter_create(obs_data_t *settings, obs_source_t *source)
{
	UNUSED_PARAMETER(settings);
	struct foreground_filter_data *filter_data =
		(struct foreground_filter_data *)bzalloc(
			sizeof(struct foreground_filter_data));

	// Retrieve any configuration settings from 'settings' and initialize the filter data

	filter_data->source = source;

	//effect file is found in the data folder
	char *filename = obs_module_file("quad_foreground_shader.effect");

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
void foreground_filter_video(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);

	//Get the data from the data structure outlined above.
	struct foreground_filter_data *filter_data =
		(struct foreground_filter_data *)data;

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

const char *foreground_filter_get_name(void *data)
{
	UNUSED_PARAMETER(data);
	return "Quad Compositor Foreground";
}

void foreground_filter_destroy(void *data)
{
	struct foreground_filter_data *filter_data =
		(struct foreground_filter_data *)data;

	// Cleanup and deallocate any resources used by the filter data

	bfree(filter_data);
}

void foreground_filter_defaults(obs_data_t *settings)
{
	UNUSED_PARAMETER(settings);
}

struct obs_source_info foreground_filter_info = {
	.id = "foreground_filter",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_VIDEO,
	.get_name = foreground_filter_get_name,
	.create = foreground_filter_create,
	.destroy = foreground_filter_destroy,
	.get_defaults = foreground_filter_defaults,
	.video_render = foreground_filter_video,
};
