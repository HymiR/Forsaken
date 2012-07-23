#include "OgreRoot.h"
#include "mxloader_plugin.h"
#include "mxloader.h"

namespace Ogre {

	const String sPluginName = "MXloader";

	MXloaderPlugin::MXloaderPlugin()
	{
	}

	String const & MXloaderPlugin::getName() const
	{
		return sPluginName;
	}

	void MXloaderPlugin::initialise()
	{
	}

	void MXloaderPlugin::install()
	{
		mxloader = OGRE_NEW MXloader();
	}

	void MXloaderPlugin::shutdown()
	{
	}

	void MXloaderPlugin::uninstall()
	{
		OGRE_DELETE mxloader;
		mxloader = NULL;
	}

}
