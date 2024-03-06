#pragma once
#include "Vertex.h"
#include "MatrixCalculation.h"

class Camera
{
public:
	
	void Initialize();

public:

	Transform GetTransform() { return transform_; }

private:

	Transform transform_;

};

