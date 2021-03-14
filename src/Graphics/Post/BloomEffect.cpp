#include "BloomEffect.h"

void BloomEffect::Init(unsigned width, unsigned height)
{
	//initialize buffers
	int index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(width, height);

	_buffers.push_back(new Framebuffer());
	_buffers[1]->AddColorTarget(GL_RGBA8);
	_buffers[1]->Init(width, height);

	_buffers.push_back(new Framebuffer());
	_buffers[2]->AddColorTarget(GL_RGBA8);
	_buffers[2]->Init(width, height);

	_buffers.push_back(new Framebuffer());
	_buffers[3]->AddColorTarget(GL_RGBA8);
	_buffers[3]->Init(width, height);

	//initializing shaders
	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/bloom_normal_lighting_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();

	_shaders.push_back(Shader::Create());
	_shaders[1]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[1]->LoadShaderPartFromFile("shaders/Post/bloom_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[1]->Link();

	_shaders.push_back(Shader::Create());
	_shaders[2]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[2]->LoadShaderPartFromFile("shaders/Post/gaussian_blur_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[2]->Link();

	_shaders.push_back(Shader::Create());
	_shaders[3]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[3]->LoadShaderPartFromFile("shaders/Post/bloom_composite_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[3]->Link();
}

void BloomEffect::ApplyEffect(PostEffect* buffer)
{
	BindShader(0);
	buffer->BindColorAsTexture(0, 0, 0);
	_buffers[0]->RenderToFSQ();
	buffer->UnbindTexture(0);
	UnbindShader();

	BindShader(1);
	_shaders[1]->SetUniform("u_Threshold", _threshold);
	BindColorAsTexture(0, 0, 0);
	_buffers[1]->RenderToFSQ();
	UnbindTexture(0);
	UnbindShader();

	for (int i = 0; i < _passes; i++)
	{
		BindShader(2);
		_shaders[2]->SetUniform("u_Horizontal", (int)true);
		BindColorAsTexture(1, 0, 0);
		_buffers[2]->RenderToFSQ();
		UnbindTexture(0);
		UnbindShader();

		BindShader(2);
		_shaders[2]->SetUniform("u_Horizontal", (int)false);
		BindColorAsTexture(2, 0, 0);
		_buffers[1]->RenderToFSQ();
		UnbindTexture(0);
		UnbindShader();
	}

	BindShader(3);
	buffer->BindColorAsTexture(0, 0, 0);
	BindColorAsTexture(1, 0, 1);
	_buffers[0]->RenderToFSQ();
	UnbindTexture(1);
	UnbindTexture(0);
	UnbindShader();

}

float BloomEffect::GetThreshold() const
{
	return _threshold;
}

int BloomEffect::GetPasses() const
{
	return _passes;
}


void BloomEffect::SetThreshold(float threshold)
{
	_threshold = threshold;
}

void BloomEffect::SetPasses(float passes)
{
	_passes = passes;
}

void BloomEffect::SetShaderUniform(int _shaderNum, std::string name, float value)
{
	_shaders[_shaderNum]->SetUniform(name, value);
}
