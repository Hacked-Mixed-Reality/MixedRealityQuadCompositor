# OBS Mixed Reality Quad Compositor

## Introduction

This plugin was generated from the OBS plugin template, and is meant to make it easier to composite the quadrants generated by games that do not have native LIV support. 

## Prerequisites
OBS Minimum version 28.0.0 (I believe).

Alt can be used to create Augmented reality by cutting out just the background of a scene, it is recommended to be used in conjunction with Exceldro's Source clone to make a two-step compositor. https://obsproject.com/forum/resources/source-clone.1632/

## Usage

1. Add your quad output to the scene, and resize it so that it is double the height and width of your canvas. Position the output so that the bottom left quadrant only is shown.
2. Add a `Source Clone` source to your scene, cloning the Quad Output. Add this `Quad Compositor Foreground` filter to the cloned source.

And that should be it. All you need to do is make sure your sources are placed in the following order:

1. Cloned / Filtered foreground
2. Keyed Camera
3. Overscaled Quad Source



## Building from source

Nothing exotic going on here, so this can be built using the scripts in the scripts folder.