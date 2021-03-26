#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
		
out vec3 v_Position;
out vec2 v_TexCoord;
out vec4 v_Color;

void main() {
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		
	v_Position = a_Position;
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
}



#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec4 v_Color;

uniform vec3 u_Color;
uniform sampler2D u_Texture;

void main() {
	//o_Color = vec4(0.8, 0.2, 0.3, 1.0);
	//o_Color = vec4(v_Position * 0.5 + 0.5, 1.0);
	//o_Color = v_Color;
	//o_Color = vec4(u_Color, 1.0);

	//o_Color = vec4(v_TexCoord, 0.0, 1.0);
	o_Color = texture(u_Texture, v_TexCoord);
}
