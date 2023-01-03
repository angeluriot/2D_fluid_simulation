#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform sampler2D u_velocity;

vec2 get_velocity(float x, float y)
{
	return texture(u_velocity, v_texcoord + vec2(x, y) / u_screen).xy;
}

void main()
{
	vec2 velocity = get_velocity(0., 0.);

	if (v_texcoord.x <= 1. / u_screen.x)
		velocity.x = -get_velocity(1., 0.).x;
	if (v_texcoord.y <= 1. / u_screen.y)
		velocity.y = -get_velocity(0., 1.).y;
	if (1. - v_texcoord.x <= 1. / u_screen.x)
		velocity.x = -get_velocity(-1., 0.).x;
	if (1. - v_texcoord.y <= 1. / u_screen.y)
		velocity.y = -get_velocity(0., -1.).y;

	frag_color = vec4(velocity, 0., 1.);
}
