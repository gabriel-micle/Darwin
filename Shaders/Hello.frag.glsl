#version 300 es

precision highp float;

in	vec3 Normal;
in	vec2 TexCoord;

out vec4 FragColor;

void main() {

	FragColor = vec4(Normal, TexCoord.x);
}
