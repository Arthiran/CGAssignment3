#pragma once

#include "Graphics/Post/PostEffect.h"

class FilmGrainEffect : public PostEffect
{
public:
	//Initializes framebuffer
	//Overrides post effect Init
	void Init(unsigned width, unsigned height) override;

	//Applies the effect to this buffer
	//Passes the previous framebuffer with the texture to apply as a parameter
	void ApplyEffect(PostEffect* buffer) override;

	//Applies the effect to the screen
	//void DrawToScreen() override;

	//Getters
	float GetStrength() const;

	//Setters
	void SetStrength(float strength);

private:
	float _strength = 10.0f;
	float _time = 0;
	bool goUp = true;
};