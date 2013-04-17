#version 300 es
// Note: Version must be on first line!

in vec4 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;


void main() {

	Normal   = inNormal;
	TexCoord = inTexCoord;

	
	gl_Position = ModelViewProjectionMatrix * inPosition;


	if (gl_InstanceID == 1) {
		gl_Position.x -= 1.0;
	} else {
		gl_Position.x += 1.0;
	}
	
}

