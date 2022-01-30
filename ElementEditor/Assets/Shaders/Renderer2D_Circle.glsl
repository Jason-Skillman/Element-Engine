//It is recommened to install the Visual Studio extention "GLSL language intergration" for glsl syntax highlighting.

#type vertex
#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;	//Editor only

//Get data in uniform buffer 0
layout(std140, binding = 0) uniform CameraBuffer {
	mat4 u_ViewProjection;
};

struct VertexOutput {
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout(location = 0) out VertexOutput vertexOutput;
layout(location = 4) out flat int v_EntityID;

void main() {
	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);

	vertexOutput.localPosition = a_LocalPosition;
	vertexOutput.color = a_Color;
	vertexOutput.thickness = a_Thickness;
	vertexOutput.fade = a_Fade;

	//Editor only
	v_EntityID = a_EntityID;
}



#type fragment
#version 450 core

//Frame buffer attachment output
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;	//Editor only

struct VertexOutput {
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout(location = 0) in VertexOutput vertexInput;
layout(location = 4) in flat int v_EntityID;	//Editor only

void main() {
	float localDistance = 1.0 - length(vertexInput.localPosition);

	float circleAlpha = smoothstep(0.0, vertexInput.fade, localDistance);
	circleAlpha *= smoothstep(vertexInput.thickness + vertexInput.fade, vertexInput.thickness, localDistance);

	if(circleAlpha == 0)
		discard;

	o_Color = vertexInput.color;
	o_Color.a *= circleAlpha;

	//Editor only
	o_EntityID = v_EntityID;
}
