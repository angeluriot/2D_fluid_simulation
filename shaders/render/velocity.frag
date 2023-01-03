#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform sampler2D u_texture;

void main()
{
	frag_color = vec4(texture(u_texture, v_texcoord).rgb / 10. + 0.5, 1.);
}
