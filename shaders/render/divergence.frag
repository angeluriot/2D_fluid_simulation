#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform sampler2D u_texture;

void main()
{
	float divergence = texture2D(u_texture, v_texcoord).r * 50.;
	vec3 color;

	if (divergence < 0.)
		color = mix(vec3(0.1, 0, 0.3), vec3(0., 0., 0.), -divergence);
	else if (divergence < 0.5)
		color = mix(vec3(0.1, 0., 0.3), vec3(0.7, 0.2, 0.1), divergence * 2.);
	else
		color = mix(vec3(0.7, 0.2, 0.1), vec3(1., 0.7, 0.1), (divergence - 0.5) * 2.);

	frag_color = vec4(color, 1.);
}
