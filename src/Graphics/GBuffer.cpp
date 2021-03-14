#include "GBuffer.h"

void GBuffer::Init(unsigned width, unsigned height)
{
	//Stores the window width and height
	_windowWidth = width;
	_windowHeight = height;

	//Adds color targets to our GBuffer
	_gBuffer.AddColorTarget(GL_RGBA8); //Albedo Buffer, needs all channels
	_gBuffer.AddColorTarget(GL_RGB8); //Normals Buffer, does not need alpha
	_gBuffer.AddColorTarget(GL_RGB8); //Specular Buffer, technically only needs 1 channel

	//Important note, you can obtain the positional data using the depth buffer (there's a calculation that you can do)
	//But here, we're going to use POSITION buffer
	_gBuffer.AddColorTarget(GL_RGB32F);

	//Add a depth buffer
	_gBuffer.AddDepthTarget();

	//Initializes our framebuffer
	_gBuffer.Init(width, height);

	//Initialize pass through shader
	_passThrough = Shader::Create();
	_passThrough->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_passThrough->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
	_passThrough->Link();
}

void GBuffer::Bind()
{
	_gBuffer.Bind();
}

void GBuffer::BindLighting()
{
	_gBuffer.BindColorAsTexture(Target::ALBEDO, 0);
	_gBuffer.BindColorAsTexture(Target::NORMAL, 1);
	_gBuffer.BindColorAsTexture(Target::SPECULAR, 2);
	_gBuffer.BindColorAsTexture(Target::POSITION, 3);
}

void GBuffer::Clear()
{
	_gBuffer.Clear();
}

void GBuffer::Unbind()
{
	_gBuffer.Unbind();
}

void GBuffer::UnbindLighting()
{
	_gBuffer.UnbindTexture(0);
	_gBuffer.UnbindTexture(1);
	_gBuffer.UnbindTexture(2);
	_gBuffer.UnbindTexture(3);
}

void GBuffer::DrawBuffersToScreen(int bufferNumber)
{
	bufferNum = bufferNumber;
	//Binds passthrough shader	
	_passThrough->Bind();

	glViewport(0, 0, _windowWidth, _windowHeight);
	if (bufferNumber == 0)
	{
		_gBuffer.BindColorAsTexture(Target::ALBEDO, 0);
		_gBuffer.DrawFullscreenQuad();
		_gBuffer.UnbindTexture(0);
	}
	else if (bufferNumber == 1)
	{
		_gBuffer.BindColorAsTexture(Target::NORMAL, 0);
		_gBuffer.DrawFullscreenQuad();
		_gBuffer.UnbindTexture(0);
	}
	else if (bufferNumber == 2)
	{
		_gBuffer.BindColorAsTexture(Target::SPECULAR, 0);
		_gBuffer.DrawFullscreenQuad();
		_gBuffer.UnbindTexture(0);
	}
	else if (bufferNumber == 3)
	{
		_gBuffer.BindColorAsTexture(Target::POSITION, 0);
		_gBuffer.DrawFullscreenQuad();
		_gBuffer.UnbindTexture(0);
	}
	else
	{
		//Set viewport to top left
		glViewport(0, _windowHeight / 2.0f, _windowWidth / 2.0f, _windowHeight / 2.0f);
		_gBuffer.BindColorAsTexture(Target::ALBEDO, 0);
		_gBuffer.DrawFullscreenQuad();
		_gBuffer.UnbindTexture(0);

		//Set viewport to top right
		glViewport(_windowWidth / 2.0f, _windowHeight / 2.0f, _windowWidth / 2.0f, _windowHeight / 2.0f);
		_gBuffer.BindColorAsTexture(Target::NORMAL, 0);
		_gBuffer.DrawFullscreenQuad();
		_gBuffer.UnbindTexture(0);

		//Set viewport to bottom left
		glViewport(0, 0, _windowWidth / 2.0f, _windowHeight / 2.0f);
		_gBuffer.BindColorAsTexture(Target::SPECULAR, 0);
		_gBuffer.DrawFullscreenQuad();
		_gBuffer.UnbindTexture(0);

		//Set viewport to bottom right
		glViewport(_windowWidth / 2.0f, 0, _windowWidth / 2.0f, _windowHeight / 2.0f);
		_gBuffer.BindColorAsTexture(Target::POSITION, 0);
		_gBuffer.DrawFullscreenQuad();
		_gBuffer.UnbindTexture(0);
	}

	//Unbind our passthrough shader
	_passThrough->UnBind();
}

void GBuffer::Reshape(unsigned width, unsigned height)
{
	//Stores new width and height
	_windowWidth = width;
	_windowHeight = height;

	//Reshapes the framebuffer
	_gBuffer.Reshape(width, height);
}

bool GBuffer::GetIsDrawing()
{
	return isDrawing;
}

void GBuffer::SetIsDrawing(bool _isDrawing)
{
	isDrawing = _isDrawing;
}
