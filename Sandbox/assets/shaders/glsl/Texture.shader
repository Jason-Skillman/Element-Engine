#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in float a_Tiling;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TextureIndex;
out float v_Tiling;

void main() {
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);

	v_Position = a_Position;
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TextureIndex = a_TextureIndex;
	v_Tiling = a_Tiling;
}



#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TextureIndex;
in float v_Tiling;

//uniform vec4 u_Color = vec4(1);
//uniform float u_Tiling = 1;
uniform sampler2D u_Textures[32];

void main() {
	//vec4 tex = texture(u_Textures[int(v_TextureIndex)], v_TexCoord * u_Tiling);
	vec4 tex = texture(u_Textures[int(v_TextureIndex)], v_TexCoord);

	//o_Color = tex * v_Color;
	o_Color = tex;
	//o_Color = vec4(v_TexCoord, 0, 1);
	//o_Color = vec4(v_Tiling, v_Tiling, v_Tiling, 1);
	//o_Color = vec4(v_TextureIndex / 2, v_TextureIndex / 2, v_TextureIndex / 2, 1);
	//o_Color = v_Color;
	//o_Color = vec4(1, 1, 1, 1);
}
