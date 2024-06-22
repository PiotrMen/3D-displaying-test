#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include "opencv2/opencv.hpp"

enum class DisplayingImageModes {
	FitSizeToMax,
	Zoomable
};

class ImageModeDisplayer
{
public:
	virtual const ImVec2 calculateDisplayedImgSize(const cv::Mat& displayedImg) = 0;
	virtual const ImVec2 calculatePosDisplayedImg(const ImVec2& maxAvailableImgSize) const = 0;
	virtual void display(const cv::Mat& displayedImg, GLuint _ImageTexture) = 0;
	virtual DisplayingImageModes getDisplayingMode() const noexcept = 0;
	double getScale() const;

	const ImVec2 getCurrentDisplayedImgPos() const;
	const ImVec2 getDisplayedImgSize() const;
protected:
	ImVec2 _displayedImgSize = ImVec2(0, 0);
	ImVec2 _topLeftCorner = ImVec2(0, 0);
	double _scale = 1.0;
};

class Object3DDisplayer
	:public ImageModeDisplayer
{
public:
	Object3DDisplayer(int width, int height) : _width(width), _height(height) {};
	~Object3DDisplayer();

private:
	unsigned int _framebuffer;
	unsigned int _texColorBuffer;
	unsigned int _rbo;
	int _width;
	int _height;
};

