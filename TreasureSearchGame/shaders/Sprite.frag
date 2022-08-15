#version 330 core

// Tex coord input from vertex shader
in vec2 fragTexCoord;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;
uniform float gSpriteAlpha;

void main()
{
	// Sample color from texture
	vec4 texColor = texture(uTexture, fragTexCoord);
	if (texColor.a < 0.1f) {
		discard;
	}
	texColor.a = gSpriteAlpha;
    outColor = texColor;
}
