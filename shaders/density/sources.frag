#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform vec2 u_mouse;
uniform float u_strength;
uniform vec4 u_color;
uniform sampler2D u_density;

vec2 pow_2(vec2 v, float e)
{
	return vec2(pow(v.x, e), pow(v.y, e));
}

void main()
{
	vec2 screen = 100. * u_screen / ((u_screen.x + u_screen.y) / 2.);
	vec2 coord = (u_mouse - v_texcoord) * screen;
	vec3 splat = u_color.rgb * exp(-dot(coord, coord) / u_strength) * 0.3;
	frag_color = vec4(clamp(texture2D(u_density, v_texcoord).rgb + splat, 0., 1.), 1.);
}
