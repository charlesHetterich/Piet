#version 330

attribute vec3 position;
attribute vec3 rgbColor;
attribute vec2 texCoords;

varying vec3 rgbColor0;
varying vec2 texCoords0;

uniform vec2 scale;
uniform mat4 transform;
uniform vec3 overrideColor;

void main()
{
    gl_Position =  transform * vec4(position, 1.0);
	
	rgbColor0 = overrideColor;// * rgbColor;
	texCoords0 = texCoords;
}