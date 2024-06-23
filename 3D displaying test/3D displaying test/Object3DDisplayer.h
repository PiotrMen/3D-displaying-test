#pragma once
#include "IImageModeDisplayer.h"

enum class ShaderMode
{
	Gradient,
	Lighting
};

enum class RenderingMode {
	FILL,
	LINE,
	POINT
};

class IObject3DDisplayer : public IDisplayer {
public:
	virtual ~IObject3DDisplayer() = default;
	virtual unsigned int getFrameBuffer() const = 0;
	virtual unsigned int getTexColorBuffer() const = 0;
	virtual void display(const Model& displayedImg, ShaderMode shaderMode, RenderingMode renderingMode) = 0;
};

class Object3DDisplayer
	:public IObject3DDisplayer
{
public:
	Object3DDisplayer(int width, int height);
	~Object3DDisplayer();

	void display(const Model& modelToDisplay, ShaderMode shaderMode,RenderingMode renderingMode) override; // Implementacja z IObject3DDisplayer
	unsigned int getFrameBuffer()const override;
	unsigned int getTexColorBuffer()const override;

private:
	unsigned int _framebuffer{};
	unsigned int _texColorBuffer{};
	unsigned int _rbo{};
	int _width;
	int _height;
};
