/*
 * Description: FenwayPark.cpp - A Virtual Fenway Park
 * Author: Patrick O'Leary
 * Date: June 3, 2010
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <vector>

#include <Math/Math.h>
#include <Math/Constants.h>

#include <GL/gl.h>

#include <GL/GLColorTemplates.h>
#include <GL/GLMatrixTemplates.h>
#include <GL/GLMaterial.h>
#include <GL/GLContextData.h>
#include <GL/GLModels.h>
#include <GL/GLGeometryWrappers.h>
#include <GL/GLPolylineTube.h>
#include <GL/GLExtensionManager.h>
#include <GL/Extensions/GLARBVertexBufferObject.h>
#include <GL/GLTransformationWrappers.h>
#include <GL/GLWindow.h>
#include <GLMotif/StyleSheet.h>
#include <GLMotif/WidgetManager.h>
#include <GLMotif/Blind.h>
#include <GLMotif/Label.h>
#include <GLMotif/Button.h>
#include <GLMotif/CascadeButton.h>
#include <GLMotif/Menu.h>
#include <GLMotif/SubMenu.h>
#include <GLMotif/Popup.h>
#include <GLMotif/PopupMenu.h>
#include <GLMotif/PopupWindow.h>
#include <GLMotif/RowColumn.h>
#include <GLMotif/TextField.h>
#include <Vrui/CoordinateManager.h>
#include <Vrui/Tools/SurfaceNavigationTool.h>
#include <Vrui/Vrui.h>
#include <Vrui/Application.h>

#include <ANALYSIS/BaseLocator.h>
#include <ANALYSIS/ClippingPlane.h>
#include <ANALYSIS/ClippingPlaneLocator.h>
#include <MODEL/Fenway.h>

#include "FenwayPark.h"

/*****************************************
 Methods of class FenwayPark::DataItem:
 *****************************************/

/****************************************************
 Constructors and Destructors of class FenwayPark:
 ****************************************************/
/*
 * DataItem - constructor
 */
FenwayPark::DataItem::DataItem(void) {
} // end DataItem()

/*
 * ~DataItem - destructor
 */
FenwayPark::DataItem::~DataItem(void) {
} // end ~DataItem()

/****************************************************
 Constructors and Destructors of class FenwayPark:
 ****************************************************/

/* FenwayPark - Constructor for FenwayPark class.
 * 		extends Vrui::Application
 *
 * parameter argc - int&
 * parameter argv - char**&
 * parameter applicationDefaults - char**&
 */
FenwayPark::FenwayPark(int& argc, char**& argv, char**& appDefaults) :
	Vrui::Application(argc, argv, appDefaults), analysisTool(0),
			clippingPlanes(0), mainMenu(0), renderDialog(0) {

	/* Create the Fenway Scene */
	fenway = new Fenway();
	fenway->config();

	/* Initialize Clippling Planes */
	numberOfClippingPlanes = 6;
	clippingPlanes = new ClippingPlane[numberOfClippingPlanes];
	for (int i = 0; i < numberOfClippingPlanes; ++i) {
		clippingPlanes[i].setAllocated(false);
		clippingPlanes[i].setActive(false);
	}

	/* Create the user interface: */
	mainMenu = createMainMenu();
	Vrui::setMainMenu(mainMenu);
	renderDialog = createRenderDialog();

	/* Initialize Vrui navigation transformation: */
	centerDisplayCallback(0);
} // end FenwayPark()

/*
 * ~FenwayPark - Destructor for FenwayPark class.
 */
FenwayPark::~FenwayPark(void) {
	/* Delete the user interface: */
	delete mainMenu;
	delete renderDialog;
} // end ~FenwayPark()

/*******************************
 Methods of class FenwayPark:
 *******************************/

/*
 * centerDisplayCallback
 *
 * parameter callbackData - Misc::CallbackData *
 */
void FenwayPark::centerDisplayCallback(Misc::CallbackData * callbackData) {
	/* Center the Sphere in the available display space, but do not scale it: */
	Vrui::NavTransform nav = Vrui::NavTransform::identity;
	nav *= Vrui::NavTransform::translateFromOriginTo(Vrui::getDisplayCenter());
	nav *= Vrui::NavTransform::scale(Vrui::Scalar(8) * Vrui::getInchFactor()
			/ Vrui::Scalar(1));
	Vrui::setNavigationTransformation(nav);
} // end centerDisplayCallback()

/*
 * changeAnalysisToolsCallback
 *
 * parameter callbackData - GLMotif::RadioBox::ValueChangedCallbackData *
 */
void FenwayPark::changeAnalysisToolsCallback(
		GLMotif::RadioBox::ValueChangedCallbackData * callbackData) {
	/* Set the new analysis tool: */
	analysisTool = callbackData->radioBox->getToggleIndex(
			callbackData->newSelectedToggle);
} // end changeAnalysisToolsCallback()

/*
 * createAnalysisToolsSubMenu
 *
 * return - GLMotif::Popup *
 */
GLMotif::Popup * FenwayPark::createAnalysisToolsSubMenu(void) {
	GLMotif::Popup * analysisToolsMenuPopup = new GLMotif::Popup(
			"analysisToolsMenuPopup", Vrui::getWidgetManager());
	GLMotif::RadioBox * analysisTools = new GLMotif::RadioBox("analysisTools",
			analysisToolsMenuPopup, false);
	analysisTools->setSelectionMode(GLMotif::RadioBox::ALWAYS_ONE);

	/* Add the clipping plane analysisTool: */
	int analysisToolIndex = 0;
	analysisTools->addToggle("Clipping Plane");
	++analysisToolIndex;

	analysisTools->setSelectedToggle(analysisTool);
	analysisTools->getValueChangedCallbacks().add(this,
			&FenwayPark::changeAnalysisToolsCallback);

	analysisTools->manageChild();

	return analysisToolsMenuPopup;
} // end createAnalysisToolsSubMenu()

/*
 * createMainMenu
 *
 * return - GLMotif::Popup *
 */
GLMotif::PopupMenu* FenwayPark::createMainMenu(void) {
	/* Create a top-level shell for the main menu: */
	GLMotif::PopupMenu * mainMenuPopup = new GLMotif::PopupMenu(
			"MainMenuPopup", Vrui::getWidgetManager());
	mainMenuPopup->setTitle("Interactive FenwayPark");

	/* Create the actual menu inside the top-level shell: */
	GLMotif::Menu * mainMenu = new GLMotif::Menu("MainMenu", mainMenuPopup,
			false);

	/* Create a cascade button to show the "Rendering Modes" submenu: */
	GLMotif::CascadeButton * renderTogglesCascade = new GLMotif::CascadeButton(
			"RenderTogglesCascade", mainMenu, "Rendering Modes");
	renderTogglesCascade->setPopup(createRenderTogglesMenu());

	/* Create a cascade button to show the "Analysis Tools" submenu: */
	GLMotif::CascadeButton * analysisToolsCascade = new GLMotif::CascadeButton(
			"AnalysisToolsCascade", mainMenu, "Analysis Tools");
	analysisToolsCascade->setPopup(createAnalysisToolsSubMenu());

	/* Create a toggle button to show the render settings dialog: */
	GLMotif::ToggleButton* showRenderDialogToggle = new GLMotif::ToggleButton(
			"showRenderDialogToggle", mainMenu, "Show Render Dialog");
	showRenderDialogToggle->setToggle(false);
	showRenderDialogToggle->getValueChangedCallbacks().add(this,
			&FenwayPark::menuToggleSelectCallback);

	/* Create a button to reset the navigation coordinates to the default (showing the entire Sphere): */
	GLMotif::Button * centerDisplayButton = new GLMotif::Button(
			"CenterDisplayButton", mainMenu, "Center Display");
	centerDisplayButton->getSelectCallbacks().add(this,
			&FenwayPark::centerDisplayCallback);

	/* Calculate the main menu's proper layout: */
	mainMenu->manageChild();

	/* Return the created top-level shell: */
	return mainMenuPopup;
} // end createMainMenu()

/*
 * createRenderDialog
 *
 * return - GLMotif::PopupWindow *
 */
GLMotif::PopupWindow * FenwayPark::createRenderDialog(void) {
	const GLMotif::StyleSheet& ss = *Vrui::getWidgetManager()->getStyleSheet();

	GLMotif::PopupWindow* renderDialogPopup = new GLMotif::PopupWindow(
			"RenderDialogPopup", Vrui::getWidgetManager(), "Display Settings");
	renderDialogPopup->setResizableFlags(true, false);

	GLMotif::RowColumn* rowColumn = new GLMotif::RowColumn("RowColumn",
			renderDialogPopup, false);
	rowColumn->setOrientation(GLMotif::RowColumn::VERTICAL);
	rowColumn->setPacking(GLMotif::RowColumn::PACK_TIGHT);
	rowColumn->setNumMinorWidgets(2);

	new GLMotif::Label("ObjectTransparencyLabel", rowColumn,
			"Object Transparency");

	GLMotif::Slider* surfaceTransparencySlider = new GLMotif::Slider(
			"SurfaceTransparencySlider", rowColumn,
			GLMotif::Slider::HORIZONTAL, ss.fontHeight * 5.0f);
	surfaceTransparencySlider->setValueRange(0.0, 1.0, 0.001);
	surfaceTransparencySlider->getValueChangedCallbacks().add(this,
			&FenwayPark::sliderCallback);

	new GLMotif::Label("GridTransparencyLabel", rowColumn, "Grid Transparency");

	GLMotif::Slider* gridTransparencySlider = new GLMotif::Slider(
			"GridTransparencySlider", rowColumn, GLMotif::Slider::HORIZONTAL,
			ss.fontHeight * 5.0f);
	gridTransparencySlider->setValueRange(0.0, 1.0, 0.001);
	gridTransparencySlider->setValue(0.1);
	gridTransparencySlider->getValueChangedCallbacks().add(this,
			&FenwayPark::sliderCallback);

	showParkToggleRD = new GLMotif::ToggleButton("showParkToggle",
			rowColumn, "Show Park");
	showParkToggleRD->setBorderWidth(0.0f);
	showParkToggleRD->setMarginWidth(0.0f);
	showParkToggleRD->setHAlignment(GLFont::Left);
	showParkToggleRD->setToggle(true);
	showParkToggleRD->getValueChangedCallbacks().add(this,
			&FenwayPark::menuToggleSelectCallback);

	wireframeToggleRD = new GLMotif::ToggleButton("wireframeToggle", rowColumn,
			"Wireframe");
	wireframeToggleRD->setBorderWidth(0.0f);
	wireframeToggleRD->setMarginWidth(0.0f);
	wireframeToggleRD->setHAlignment(GLFont::Left);
	wireframeToggleRD->setToggle(false);
	wireframeToggleRD->getValueChangedCallbacks().add(this,
			&FenwayPark::menuToggleSelectCallback);

	rowColumn->manageChild();

	return renderDialogPopup;
} // end createRenderDialog()

/*
 * createRenderTogglesMenu
 *
 * return - GLMotif::Popup *
 */
GLMotif::Popup * FenwayPark::createRenderTogglesMenu(void) {
	/* Create the submenu's top-level shell: */
	GLMotif::Popup * renderTogglesMenuPopup = new GLMotif::Popup(
			"RenderTogglesMenuPopup", Vrui::getWidgetManager());

	/* Create the array of render toggle buttons inside the top-level shell: */
	GLMotif::SubMenu * renderTogglesMenu = new GLMotif::SubMenu(
			"RenderTogglesMenu", renderTogglesMenuPopup, false);

	/* Create a toggle button to render Building: */
	showParkToggle = new GLMotif::ToggleButton("showParkToggle",
			renderTogglesMenu, "Show Park");
	showParkToggle->setToggle(true);
	showParkToggle->getValueChangedCallbacks().add(this,
			&FenwayPark::menuToggleSelectCallback);

	/* Create a toggle button to render Wireframe: */
	wireframeToggle = new GLMotif::ToggleButton("wireframeToggle",
			renderTogglesMenu, "Wireframe");
	wireframeToggle->setToggle(false);
	wireframeToggle->getValueChangedCallbacks().add(this,
			&FenwayPark::menuToggleSelectCallback);

	/* Calculate the submenu's proper layout: */
	renderTogglesMenu->manageChild();

	/* Return the created top-level shell: */
	return renderTogglesMenuPopup;
} // end createRenderTogglesMenu

/*
 * display
 *
 * parameter glContextData - GLContextData &
 */
void FenwayPark::display(GLContextData & glContextData) const {

	/* Get context data item: */
	DataItem* dataItem = glContextData.retrieveDataItem<DataItem> (this);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushAttrib(GL_TRANSFORM_BIT);
	glPushAttrib(GL_VIEWPORT_BIT);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();

	/* Enable all clipping planes: */
	int numberOfSupportedClippingPlanes;
	glGetIntegerv(GL_MAX_CLIP_PLANES, &numberOfSupportedClippingPlanes);
	int clippingPlaneIndex = 0;
	for (int i = 0; i < numberOfClippingPlanes && clippingPlaneIndex
			< numberOfSupportedClippingPlanes; ++i) {
		if (clippingPlanes[i].isActive()) {
			/* Enable the clipping plane: */
			glEnable(GL_CLIP_PLANE0 + clippingPlaneIndex);
			GLdouble clippingPlane[4];
			for (int j = 0; j < 3; ++j)
				clippingPlane[j] = clippingPlanes[i].getPlane().getNormal()[j];
			clippingPlane[3] = -clippingPlanes[i].getPlane().getOffset();
			glClipPlane(GL_CLIP_PLANE0 + clippingPlaneIndex, clippingPlane);
			/* Go to the next clipping plane: */
			++clippingPlaneIndex;
		}
	}

	fenway->display(glContextData);

	/* Disable all clipping planes: */
	clippingPlaneIndex = 0;
	for (int i = 0; i < numberOfClippingPlanes && clippingPlaneIndex
			< numberOfSupportedClippingPlanes; ++i) {
		if (clippingPlanes[i].isActive()) {
			/* Disable the clipping plane: */
			glDisable(GL_CLIP_PLANE0 + clippingPlaneIndex);
			/* Go to the next clipping plane: */
			++clippingPlaneIndex;
		}
	}

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopClientAttrib();
	glPopAttrib();
	glPopAttrib();
	glPopAttrib();
} // end display()

/*
 * frame
 */
void FenwayPark::frame(void) {
	fenway->frame();
} // end frame()

/*
 * getClippingPlanes
 *
 * return - ClippingPlane *
 */
ClippingPlane * FenwayPark::getClippingPlanes(void) {
	return clippingPlanes;
} // end getClippingPlanes()

/*
 * getNumberOfClippingPlanes
 *
 * return - int
 */
int FenwayPark::getNumberOfClippingPlanes(void) {
	return numberOfClippingPlanes;
} // end getNumberOfClippingPlanes()

/*
 * initContext
 *
 * parameter glContextData - GLContextData &
 */
void FenwayPark::initContext(GLContextData & glContextData) const {
	/* Create a new context data item: */
	DataItem* dataItem = new DataItem();

	glContextData.addDataItem(this, dataItem);
} // end initContext()

/*
 * menuToggleSelectCallback
 *
 * parameter callbackData - GLMotif::ToggleButton::ValueChangedCallbackData *
 */
void FenwayPark::menuToggleSelectCallback(
		GLMotif::ToggleButton::ValueChangedCallbackData * callbackData) {
	/* Adjust program state based on which toggle button changed state: */
	if (strcmp(callbackData->toggle->getName(), "showParkToggle") == 0) {
		fenway->togglePark();
		showParkToggle->setToggle(callbackData->set);
		showParkToggleRD->setToggle(callbackData->set);
	} else if (strcmp(callbackData->toggle->getName(), "wireframeToggle") == 0) {
		fenway->toggleWireframe();
		wireframeToggle->setToggle(callbackData->set);
		wireframeToggleRD->setToggle(callbackData->set);
	} else if (strcmp(callbackData->toggle->getName(), "showRenderDialogToggle")
			== 0) {
		if (callbackData->set) {
			/* Open the render dialog at the same position as the main menu: */
			Vrui::getWidgetManager()->popupPrimaryWidget(
					renderDialog,
					Vrui::getWidgetManager()->calcWidgetTransformation(mainMenu));
		} else {
			/* Close the render dialog: */
			Vrui::popdownPrimaryWidget(renderDialog);
		}
	}
} // end menuToggleSelectCallback()

/*
 * sliderCallback
 *
 * parameter callbackData - GLMotif::Slider::ValueChangedCallbackData *
 */
void FenwayPark::sliderCallback(
		GLMotif::Slider::ValueChangedCallbackData * callbackData) {
	if (strcmp(callbackData->slider->getName(), "SurfaceTransparencySlider")
			== 0) {
		;
	} else if (strcmp(callbackData->slider->getName(), "GridTransparencySlider")
			== 0) {
		;
	}
} // end sliderCallback()

/*
 * toolCreationCallback
 *
 * parameter callbackData - Vrui::ToolManager::ToolCreationCallbackData *
 */
void FenwayPark::toolCreationCallback(
		Vrui::ToolManager::ToolCreationCallbackData * callbackData) {
	/* Check if the new tool is a locator tool: */
	Vrui::LocatorTool* locatorTool =
			dynamic_cast<Vrui::LocatorTool*> (callbackData->tool);
	if (locatorTool != 0) {
		BaseLocator* newLocator;
		if (analysisTool == 0) {
			/* Create a clipping plane locator object and associate it with the new tool: */
			newLocator = new ClippingPlaneLocator(locatorTool, this);
		}
		/* Add new locator to list: */
		baseLocators.push_back(newLocator);
	}
} // end toolCreationCallback()

/*
 * toolDestructionCallback
 *
 * parameter callbackData - Vrui::ToolManager::ToolDestructionCallbackData *
 */
void FenwayPark::toolDestructionCallback(
		Vrui::ToolManager::ToolDestructionCallbackData * callbackData) {
	/* Check if the to-be-destroyed tool is a locator tool: */
	Vrui::LocatorTool* locatorTool =
			dynamic_cast<Vrui::LocatorTool*> (callbackData->tool);
	if (locatorTool != 0) {
		/* Find the data locator associated with the tool in the list: */
		for (BaseLocatorList::iterator blIt = baseLocators.begin(); blIt
				!= baseLocators.end(); ++blIt) {

			if ((*blIt)->getTool() == locatorTool) {
				/* Remove the locator: */
				delete *blIt;
				baseLocators.erase(blIt);
				break;
			}
		}
	}
} // end toolDestructionCallback()

/*
 * main - The application main method.
 *
 * parameter argc - int
 * parameter argv - char**
 */
int main(int argc, char* argv[]) {
	try {
		/* Create the FenwayPark application object: */
		char** applicationDefaults = 0;
		FenwayPark application(argc, argv, applicationDefaults);

		/* Run the FenwayPark application: */
		application.run();

		/* Return to the OS: */
		return 0;
	} catch (std::runtime_error err) {
		/* Print an error message and return to the OS: */
		std::cerr << "Caught exception " << err.what() << std::endl;
		return 1;
	}
} // end main()
