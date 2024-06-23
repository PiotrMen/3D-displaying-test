#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "opencv2/opencv.hpp"
#include "Model.h"

enum class DisplayingImageModes {
	FitSizeToMax,
	Zoomable,
	Object3D
};

class IDisplayer {
public:
	virtual ~IDisplayer() = default;
};

class IImageModeDisplayer : public IDisplayer {
public:
	virtual ~IImageModeDisplayer() = default;
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

