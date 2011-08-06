/*
 * Description: FenwayPark.h - A Virtual Fenway Park
 * Author: Patrick O'Leary
 * Date: June 3, 2010
 */
#ifndef FENWAYPARK_INCLUDED
#define FENWAYPARK_INCLUDED

#include <vector>

#include <GL/gl.h>
#include <GL/GLObject.h>
#include <GLMotif/RadioBox.h>
#include <GLMotif/Slider.h>
#include <GLMotif/ToggleButton.h>
#include <Misc/CallbackData.h>
#include <Vrui/Tools/LocatorTool.h>
#include <Vrui/LocatorToolAdapter.h>
#include <Vrui/ToolManager.h>
#include <Vrui/Application.h>

/* Begin Forward declarations: */
class Fenway;
class ClippingPlane;

namespace GLMotif {
class Popup;
class PopupMenu;
class PopupWindow;
class TextField;
}
/* End Forward declarations: */

class FenwayPark: public Vrui::Application, public GLObject {
	typedef std::vector<BaseLocator*> BaseLocatorList;
	/* Embedded classes: */
private:
	struct DataItem: public GLObject::DataItem {
	public:
		/* Elements: */
		int data;
		/* Constructors and destructors: */
		DataItem(void);
		virtual ~DataItem(void);
	};

public:
	/* Constructors and destructors: */
	FenwayPark(int& argc, char**& argv, char**& appDefaults);
	virtual ~FenwayPark(void);

	/* Methods: */
	void centerDisplayCallback(Misc::CallbackData * callbackData);
	virtual void display(GLContextData& contextData) const;
	virtual void frame(void);
	ClippingPlane * getClippingPlanes(void);
	int getNumberOfClippingPlanes(void);
	virtual void initContext(GLContextData& contextData) const;
	void menuToggleSelectCallback(
			GLMotif::ToggleButton::ValueChangedCallbackData * callbackData);
	void sliderCallback(
			GLMotif::Slider::ValueChangedCallbackData * callbackData);

private:
	int analysisTool;
	Fenway * fenway;
	BaseLocatorList baseLocators;
	ClippingPlane * clippingPlanes;
	GLMotif::PopupMenu* mainMenu;
	int numberOfClippingPlanes;
	GLMotif::PopupWindow* renderDialog;
	GLMotif::ToggleButton * showParkToggle;
	GLMotif::ToggleButton * showParkToggleRD;
	GLMotif::ToggleButton * wireframeToggle;
	GLMotif::ToggleButton * wireframeToggleRD;

	/* Private methods: */
	void changeAnalysisToolsCallback(
			GLMotif::RadioBox::ValueChangedCallbackData * callbackData);
	GLMotif::Popup * createAnalysisToolsSubMenu(void);
	GLMotif::PopupMenu * createMainMenu(void);
	GLMotif::PopupWindow * createRenderDialog(void);
	GLMotif::Popup * createRenderTogglesMenu(void);
	virtual void toolCreationCallback(
			Vrui::ToolManager::ToolCreationCallbackData * callbackData);
	virtual void toolDestructionCallback(
			Vrui::ToolManager::ToolDestructionCallbackData * callbackData);
};
#endif
