#pragma once
#include "IImageModeDisplayer.h"
#include "Object3DDisplayer.h"
#include "opencv2/opencv.hpp"

class DisplayManager {
public:
	DisplayManager(int width, int height) : currentDisplayer(nullptr), currentImageDisplayer(nullptr), currentObject3DDisplayer(nullptr) { this->setDisplayMode<Object3DDisplayer>(width, height); }

	template <typename DisplayerType, typename... Args>
	void setDisplayMode(Args&&... args) {
		currentDisplayer = std::make_unique<DisplayerType>(std::forward<Args>(args)...);
		currentImageDisplayer = dynamic_cast<IImageModeDisplayer*>(currentDisplayer.get());
		currentObject3DDisplayer = dynamic_cast<IObject3DDisplayer*>(currentDisplayer.get());
	}

	void displayImage(const cv::Mat& image, GLuint texture) {
		if (currentImageDisplayer) {
			currentImageDisplayer->display(image, texture);
		}
		else {
			throw std::runtime_error("Current displayer is not an image displayer");
		}
	}

	void displayObject3D(const Model& modelToDisplay, ShaderMode shaderMode) {
		if (currentObject3DDisplayer) {
			currentObject3DDisplayer->display(modelToDisplay,shaderMode);
		}
		else {
			throw std::runtime_error("Current displayer is not an object 3D displayer");
		}
	}

private:
	std::unique_ptr<IDisplayer> currentDisplayer;
	IImageModeDisplayer* currentImageDisplayer;
	IObject3DDisplayer* currentObject3DDisplayer;
};
