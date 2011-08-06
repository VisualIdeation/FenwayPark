/*
 * ClippingPlane.h - Class for clipping plane.
 *
 * Author: Patrick O'Leary
 * Created: June 3, 2010
 * Copyright: 2010. All rights reserved.
 */

#ifndef CLIPPINGPLANE_H_
#define CLIPPINGPLANE_H_

/* Vrui includes */
#include <Geometry/Plane.h>
#include <Vrui/Geometry.h>

class ClippingPlane {
public:
	ClippingPlane(void);
	~ClippingPlane(void);
	bool isActive(void);
	void setActive(bool _active);
	bool isAllocated(void);
	void setAllocated(bool _allocated);
	Vrui::Plane getPlane(void);
	void setPlane(Vrui::Plane plane);
private:
	bool active;
	bool allocated;
	Vrui::Plane plane;
};

#endif /*CLIPPINGPLANE_H_*/
