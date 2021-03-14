#include "FilmGrainEffect.h"

void FilmGrainEffect::Init(unsigned width, unsigned height)
{
	int index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);

	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/film_grain_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
}

void FilmGrainEffect::ApplyEffect(PostEffect* buffer)
{
	BindShader(0);
	_time++;
	float result = sin(_time / 10) * 10;
	_shaders[0]->SetUniform("u_Time", result);
	_shaders[0]->SetUniform("u_Strength", _strength);
	buffer->BindColorAsTexture(0, 0, 0);
	_buffers[0]->RenderToFSQ();
	buffer->UnbindTexture(0);
	UnbindShader();
}

/*void FilmGrainEffect::DrawToScreen()
{
	BindShader(0);
	_shaders[0]->SetUniform("u_Intensity", _intensity);
	BindColorAsTexture(0, 0, 0);
	_buffers[0]->DrawFullscreenQuad();
	UnbindTexture(0);
	UnbindShader();
}*/

float FilmGrainEffect::GetStrength() const
{
	return _strength;
}

void FilmGrainEffect::SetStrength(float strength)
{
	_strength = strength;
}