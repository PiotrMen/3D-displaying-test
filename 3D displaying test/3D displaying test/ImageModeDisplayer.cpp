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
