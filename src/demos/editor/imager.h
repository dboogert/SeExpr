#pragma once

class Qimage;

#include <SeExpression.h>

class QImage* MakeImage(const std::string& exprStr, int width, int height, std::vector<SeExpression::Error>& errors);
