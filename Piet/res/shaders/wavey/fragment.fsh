#version 330

varying vec4 rgbColor0;
varying vec2 texCoords0;

uniform sampler2D diffuse;
uniform vec2 scale;
uniform int clock;

float outlineThikness = 0.1;

void main()
{	
	float posX = float(int(texCoords0.x*scale.x * 3))/20;
	float posY = float(int(texCoords0.y*scale.y* 3))/20;

	float waveY = (0.05/scale.y + 0.05/scale.y * sin(posX * 20.28 + float(clock)/200))/1.0;
	float waveX = (0.05/scale.x + 0.05/scale.x * sin(posY * 20.28 + float(clock)/200))/1.0;
	
	if (texCoords0.y > waveY && texCoords0.y < 1 - waveY && texCoords0.x > waveX && texCoords0.x < 1 - waveX)
	{
	gl_FragColor = vec4(rgbColor0);
	if (texCoords0.x < outlineThikness / scale.x + waveX|| texCoords0.x > (1.0 - ( outlineThikness / scale.x)) - waveX|| texCoords0.y < outlineThikness / scale.y + waveY|| texCoords0.y > (1.0 - (outlineThikness / scale.y)) - waveY)
		gl_FragColor = vec4(1,1,1,1);

	}
	else
	{
		gl_FragColor = vec4(0,0,0,0);
	}
}