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
    outColor = texture(uTexture, fragTexCoord);
	outColor.a = gSpriteAlpha;
}
