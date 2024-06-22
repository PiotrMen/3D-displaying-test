#include "ImageModeDisplayer.h"


double ImageModeDisplayer::getScale() const
{
	return this->_scale;
}

const ImVec2 ImageModeDisplayer::getCurrentDisplayedImgPos() const
{
	return this->_topLeftCorner;
}

const ImVec2 ImageModeDisplayer::getDisplayedImgSize() const
{
	return this->_displayedImgSize;
}

Object3DDisplayer::Object3DDisplayer(int width, int height) {
    glEnable(GL_DEPTH_TEST);

// Create framebuffer
glGenFramebuffers(1, &this->_framebuffer);
glBindFramebuffer(GL_FRAMEBUFFER, this->_framebuffer);

// Create a color attachment texture
glGenTextures(1, &this->_texColorBuffer);
glBindTexture(GL_TEXTURE_2D, this->_texColorBuffer);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->_width, this->_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_texColorBuffer, 0);

// Create a renderbuffer object for depth and stencil attachment
glGenRenderbuffers(1, &this->_rbo);
glBindRenderbuffer(GL_RENDERBUFFER, this->_rbo);
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->_width, this->_height);
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->_rbo);

// Check if framebuffer is complete
if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
glBindFramebuffer(GL_FRAMEBUFFER, 0);
}