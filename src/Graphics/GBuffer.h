#pragma once

#include "Framebuffer.h"

enum Target
{
	ALBEDO,
	NORMAL,
	SPECULAR,
	POSITION,
};


class GBuffer
{
public:
	//Initialize this effects (will be overriden in each derived class)
	void Init(unsigned width, unsigned height);

	//Binds the Gbuffer
	void Bind();

	//Bind the lighting
	void BindLighting();

	//Clears the Gbuffer
	void Clear();

	//Unbinds the Gbuffer
	void Unbind();

	//Unbinds the lighting
	void UnbindLighting();

	//Draws out the buffers to the screen
	void DrawBuffersToScreen(int bufferNumber);

	//Reshape the framebuffer
	void Reshape(unsigned width, unsigned height);

	bool GetIsDrawing();

	void SetIsDrawing(bool _isDrawing);
private:
	Framebuffer _gBuffer;
	Shader::sptr _passThrough;

	int _windowWidth;
	int _windowHeight;
	int bufferNum = 10;
	bool isDrawing = false;
};