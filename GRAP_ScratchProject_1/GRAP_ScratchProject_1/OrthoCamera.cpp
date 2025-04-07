#include "OrthoCamera.h"

void OrthoCamera::positionAbove() {

	pitch = -89;
	position.y += 10;

	updateCamera();

}