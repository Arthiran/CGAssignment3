#pragma once

#include "Graphics/Post/PostEffect.h"

class BloomEffect : public PostEffect
{
public:
	//initialize framebuffer
	//override post effect init
	void Init(unsigned width, unsigned height) override;

	//applies effect to this buffer
	void ApplyEffect(PostEffect* buffer) override;

	//applies the effect to the screen
	//void DrawToScreen() override;

	//Getters
	float GetThreshold() const;
	int GetPasses() const;

	//setters
	void SetThreshold(float threshold);
	void SetPasses(float passes);
	void SetShaderUniform(int _shaderNum, std::string name, float value);

private:
	float _threshold = 0.05f;
	int _passes = 10;
};
