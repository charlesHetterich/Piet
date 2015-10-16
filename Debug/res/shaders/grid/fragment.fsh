#version 330

varying vec4 rgbColor0;
varying vec2 texCoords0;

uniform sampler2D diffuse;

float thickness = 0.01;

void main()
{	
	//if (texCoords0.x < thickness || texCoords0.x > 1 - thickness || texCoords0.y < thickness || texCoords0.y > 1 - thickness)
	//	gl_FragColor = vec4(0,0,0,1);
	//else
	gl_FragColor = texture(diffuse, vec2(texCoords0.x, texCoords0.y)) * vec4(rgbColor0);
}