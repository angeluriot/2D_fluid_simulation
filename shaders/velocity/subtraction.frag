#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform sampler2D u_velocity;
uniform sampler2D u_gradient;

void main()
{
	vec2 velocity = texture(u_velocity, v_texcoord).xy - texture(u_gradient, v_texcoord).xy;
	frag_color = vec4(velocity, 0., 1.);
}
