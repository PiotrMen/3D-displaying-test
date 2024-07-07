#pragma once
#include "IImageModeDisplayer.h"
#include"Shaders.h"

//ToDo zamiast enum class ShaderMode zmien to na klasy ktore obsluguja kazda swoj tryb wyswietlania. Uwzglednij ze w przyszlosci bedzie wiecej trybow czyli zastanow sie nad zrobieniem klasy bazowe typu Displaying mode i reszta bedzie po niej dziedziczyc.
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
	virtual void display(const Model& modelToDisplay/*, const std::unique_ptr<ShaderProgram> shaderProgram, RenderingMode renderingMode, float elementUsagePercentage*/) = 0;
};

class Object3DDisplayer
	:public IObject3DDisplayer
{
public:
	Object3DDisplayer(int width, int height, std::unique_ptr<ShaderProgram> shaderProgram);
	~Object3DDisplayer();

	void display(const Model& modelToDisplay) override; // Implementacja z IObject3DDisplayer
	void display(const Model& modelToDisplay, ShaderProgram* shaderProgram, RenderingMode renderingMode, float elementUsagePercentage); // Implementacja z IObject3DDisplayer
	unsigned int getFrameBuffer()const override;
	unsigned int getTexColorBuffer()const override;
	std::shared_ptr<ShaderProgram> getShaderProgram() const;
	RenderingMode getRenderingMode();
	void setShaderProgram(std::unique_ptr<ShaderProgram> shaderProgram);
	void setRenderMode(RenderingMode mode);

	template <typename ShaderProgram, typename... Args>
	void setShaderValues(Args&&... args)
	{
		
	}
public:
	//ShaderMode currentShaderMode;

private:


private:
	RenderingMode _currentRenderingMode = RenderingMode::FILL;
	std::shared_ptr<ShaderProgram> _shaderProgram;
	float _elementUsagePercentage;
	unsigned int _framebuffer{};
	unsigned int _texColorBuffer{};
	unsigned int _rbo{};
	int _width;
	int _height;
};
