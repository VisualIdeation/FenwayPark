/*
 * Fenway.h - Class for Fenway facility.
 *
 * Author: Patrick O'Leary
 * Created: June 3, 2010
 * Copyright: 2010
 */

#ifndef FENWAY_H_
#define FENWAY_H_

/* Vrui includes */
#include <GL/GLContextData.h>
#include <GL/GLObject.h>

/* Delta3D includes */
#include <dtABC/application.h>

/* osg includes */
#include <osg/Version>
#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Transform>
#include <osg/Group>
#include <osg/Node>
#include <osg/Camera>

#include <osgUtil/UpdateVisitor>

#include <osgViewer/Viewer>

#include <SYNC/MutexPosix.h>
#include <SYNC/NullMutex.h>

using namespace std;
using namespace dtCore;
using namespace dtABC;
using namespace dtUtil;

namespace dtCore {
class Environment;
class Object;
}
class dMass;

class Fenway: public Application , public GLObject {
public:
	Fenway(void);
protected:
	virtual ~Fenway(void);
private:
	struct DataItem: public GLObject::DataItem {
	public:
		/* Elements: */
		int data;
		osg::Group * root;
		osg::ref_ptr<osgViewer::Viewer> viewer;
		MutexPosix viewerLock;
		/* Constructors and destructors: */
		DataItem(void);
		virtual ~DataItem(void);
	};
public:
	void addObjects(void);
	virtual void config(void);
	virtual void display(GLContextData& contextData) const;
	void frame(void);
	virtual void initContext(GLContextData& contextData) const;
	void togglePark(void);
	void toggleWireframe(void);
	Fenway * fenway;
	bool drawMode;
	int frameNumber;
	osg::ref_ptr<osg::FrameStamp> frameStamp;
	double lastFrameTime;
	RefPtr<Object> park;
	osg::ref_ptr<osg::NodeVisitor> updateVisitor;
private:
	void createPark(void);
};

#endif
