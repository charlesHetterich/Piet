#version 330

varying vec3 rgbColor0;
varying vec2 texCoords0;

uniform sampler2D diffuse;
uniform vec2 scale;

float outlineThikness = 0.05;

void main()
{	
	gl_FragColor = texture(diffuse, vec2(texCoords0.x, texCoords0.y)) * vec4(rgbColor0, 1);
	if (texCoords0.x < outlineThikness / scale.x || texCoords0.x > (1.0 - ( outlineThikness / scale.x))|| texCoords0.y < outlineThikness / scale.y || texCoords0.y > (1.0 - (outlineThikness / scale.y)))
		gl_FragColor = vec4(0,0,0,1);
}