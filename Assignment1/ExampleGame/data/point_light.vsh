#version 150

// Scene info
uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform mat3 worldIT;

// Attributes
in vec4 a_position;
in vec3 a_normal;
in vec2 a_uv1;

// Varyings
out vec3 v_normal;
out vec4 v_pos;
out vec2 v_uv1;

void main()
{
    gl_Position = projection * view * world * a_position;
	vec3 n = (worldIT * a_normal).xyz;

	// Output
    v_normal = n;
	v_pos = world * a_position;
	v_uv1 = a_uv1;
}


