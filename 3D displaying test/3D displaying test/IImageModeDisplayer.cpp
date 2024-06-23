#include "IImageModeDisplayer.h"


double IImageModeDisplayer::getScale() const
{
	return this->_scale;
}

const ImVec2 IImageModeDisplayer::getCurrentDisplayedImgPos() const
{
	return this->_topLeftCorner;
}

const ImVec2 IImageModeDisplayer::getDisplayedImgSize() const
{
	return this->_displayedImgSize;
}
