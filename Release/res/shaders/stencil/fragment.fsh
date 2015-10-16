#version 330

varying vec4 rgbColor0;
varying vec2 texCoords0;

uniform sampler2D diffuse;

float thickness = 0.01;

void main()
{	
	gl_FragColor = vec4(0,0,0,1);
}