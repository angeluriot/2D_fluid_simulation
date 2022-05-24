#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform vec2 u_mouse;
uniform float u_strength;
uniform vec4 u_color;
uniform sampler2D u_density;

void main()
{
	float strength = clamp(0.6 - distance(v_texcoord * u_screen, u_mouse * u_screen) / (u_strength * 1.1), 0., 0.6);
	frag_color = vec4(clamp(texture2D(u_density, v_texcoord).rgb + u_color.rgb * strength, 0., 1.), 1.);
}
