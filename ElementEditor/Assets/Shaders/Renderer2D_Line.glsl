//It is recommened to install the Visual Studio extention "GLSL language intergration" for glsl syntax highlighting.

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_EntityID;	//Editor only

//Get data in uniform buffer 0
layout(std140, binding = 0) uniform CameraBuffer {
	mat4 u_ViewProjection;
};

struct VertexOutput {
	vec4 color;
};

layout(location = 0) out VertexOutput vertexOutput;
layout(location = 1) out flat int v_EntityID;

void main() {
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);

	vertexOutput.color = a_Color;

	//Editor only
	v_EntityID = a_EntityID;
}



#type fragment
#version 450 core

//Frame buffer attachment output
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;	//Editor only

struct VertexOutput {
	vec4 color;
};

layout(location = 0) in VertexOutput vertexInput;
layout(location = 1) in flat int v_EntityID;	//Editor only

void main() {
	o_Color = vertexInput.color;

	//Editor only
	o_EntityID = v_EntityID;
}
