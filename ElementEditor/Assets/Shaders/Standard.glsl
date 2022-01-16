//It is recommened to install the Visual Studio extention "GLSL language intergration" for glsl syntax highlighting.

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in float a_Tiling;
layout(location = 5) in int a_EntityID;	//Editor only

//Get data in uniform buffer 0
layout(std140, binding = 0) uniform CameraBuffer {
	mat4 u_ViewProjection;
};

struct VertexOutput {
	vec4 color;
	vec2 texCoord;
	float textureIndex;
	float tiling;
};

layout(location = 0) out VertexOutput vertexOutput;
layout(location = 4) out flat int v_EntityID;

void main() {
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);

	vertexOutput.color = a_Color;
	vertexOutput.texCoord = a_TexCoord;
	vertexOutput.textureIndex = a_TextureIndex;
	vertexOutput.tiling = a_Tiling;

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
	vec2 texCoord;
	float textureIndex;
	float tiling;
};

layout(location = 0) in VertexOutput vertexInput;
layout(location = 4) in flat int v_EntityID;	//Editor only

layout(binding = 0) uniform sampler2D u_Textures[32];

void main() {
	//Get the texture from the texture slot
	vec4 texColor = vertexInput.color;

	//Does not work on AMD graphic cards
	//vec4 texColor = texture(u_Textures[int(v_TextureIndex)], vertexInput.texCoord * vertexInput.tiling);
	switch(int(vertexInput.textureIndex)) {
		case 0: texColor = texture(u_Textures[0], vertexInput.texCoord * vertexInput.tiling); break;
		case 1: texColor = texture(u_Textures[1], vertexInput.texCoord * vertexInput.tiling); break;
		case 2: texColor = texture(u_Textures[2], vertexInput.texCoord * vertexInput.tiling); break;
		case 3: texColor = texture(u_Textures[3], vertexInput.texCoord * vertexInput.tiling); break;
		case 4: texColor = texture(u_Textures[4], vertexInput.texCoord * vertexInput.tiling); break;
		case 5: texColor = texture(u_Textures[5], vertexInput.texCoord * vertexInput.tiling); break;
		case 6: texColor = texture(u_Textures[6], vertexInput.texCoord * vertexInput.tiling); break;
		case 7: texColor = texture(u_Textures[7], vertexInput.texCoord * vertexInput.tiling); break;
		case 8: texColor = texture(u_Textures[8], vertexInput.texCoord * vertexInput.tiling); break;
		case 9: texColor = texture(u_Textures[9], vertexInput.texCoord * vertexInput.tiling); break;
		case 10: texColor = texture(u_Textures[10], vertexInput.texCoord * vertexInput.tiling); break;
		case 11: texColor = texture(u_Textures[11], vertexInput.texCoord * vertexInput.tiling); break;
		case 12: texColor = texture(u_Textures[12], vertexInput.texCoord * vertexInput.tiling); break;
		case 13: texColor = texture(u_Textures[13], vertexInput.texCoord * vertexInput.tiling); break;
		case 14: texColor = texture(u_Textures[14], vertexInput.texCoord * vertexInput.tiling); break;
		case 15: texColor = texture(u_Textures[15], vertexInput.texCoord * vertexInput.tiling); break;
		case 16: texColor = texture(u_Textures[16], vertexInput.texCoord * vertexInput.tiling); break;
		case 17: texColor = texture(u_Textures[17], vertexInput.texCoord * vertexInput.tiling); break;
		case 18: texColor = texture(u_Textures[18], vertexInput.texCoord * vertexInput.tiling); break;
		case 19: texColor = texture(u_Textures[19], vertexInput.texCoord * vertexInput.tiling); break;
		case 20: texColor = texture(u_Textures[20], vertexInput.texCoord * vertexInput.tiling); break;
		case 21: texColor = texture(u_Textures[21], vertexInput.texCoord * vertexInput.tiling); break;
		case 22: texColor = texture(u_Textures[22], vertexInput.texCoord * vertexInput.tiling); break;
		case 23: texColor = texture(u_Textures[23], vertexInput.texCoord * vertexInput.tiling); break;
		case 24: texColor = texture(u_Textures[24], vertexInput.texCoord * vertexInput.tiling); break;
		case 25: texColor = texture(u_Textures[25], vertexInput.texCoord * vertexInput.tiling); break;
		case 26: texColor = texture(u_Textures[26], vertexInput.texCoord * vertexInput.tiling); break;
		case 27: texColor = texture(u_Textures[27], vertexInput.texCoord * vertexInput.tiling); break;
		case 28: texColor = texture(u_Textures[28], vertexInput.texCoord * vertexInput.tiling); break;
		case 29: texColor = texture(u_Textures[29], vertexInput.texCoord * vertexInput.tiling); break;
		case 30: texColor = texture(u_Textures[30], vertexInput.texCoord * vertexInput.tiling); break;
		case 31: texColor = texture(u_Textures[31], vertexInput.texCoord * vertexInput.tiling); break;
	}

	o_Color = texColor * vertexInput.color;

	//Editor only
	o_EntityID = v_EntityID;
}
