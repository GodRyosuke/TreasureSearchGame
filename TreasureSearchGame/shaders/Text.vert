#version 330 core


// Uniforms for world transform and view-proj
uniform mat4 gTranslate;
uniform mat4 gRotate;
uniform mat4 gSpriteViewProj;

// Attribute 0 is position, 1 is normal, 2 is tex coords.
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

// Any vertex outputs (other than position)
out vec2 fragTexCoord;

void main()
{
	// Convert position to homogeneous coordinates
	vec4 pos = vec4(inPosition, 0.0, 1.0);
	gl_Position = gSpriteViewProj * gTranslate  * gRotate * pos;
	fragTexCoord = inTexCoord;
}