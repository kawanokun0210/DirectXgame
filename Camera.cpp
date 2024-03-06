#include "Camera.h"

void Camera::Initialize() {
	transform_ = { 
		{1.0f,1.0f,1.0f},//scale
		{0.0f,0.0f,0.0f},//rotate
		{0.0f,0.0f,-10.0f}//translate
	};
}