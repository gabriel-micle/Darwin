#version 300 es

in vec4 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 Normal;
out vec2 TexCoord;

void main() {

	Normal   = inNormal;
	TexCoord = inTexCoord;

	
	gl_Position = inPosition;

	if (gl_InstanceID == 1) {
		gl_Position.x -= 0.25;
	} else {
		gl_Position.x += 0.25;
	}
	
}

