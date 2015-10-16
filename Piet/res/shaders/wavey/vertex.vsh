#version 330

attribute vec3 position;
attribute vec3 rgbColor;
attribute vec2 texCoords;

varying vec4 rgbColor0;
varying vec2 texCoords0;

uniform mat4 transform;
uniform vec4 overrideColor;
uniform vec2 scale;
uniform int clock;

void main()
{
    gl_Position =  transform * vec4(position, 1.0);
	
	rgbColor0 = overrideColor;//vec3(overrideColor.r, overrideColor.g, overrideColor.b);// * rgbColor;
	texCoords0 = texCoords;
}