#include "OgreRoot.h"
#include <OgrePrerequisites.h>
#include "mxloader_plugin.h"

#ifndef OGRE_STATIC_LIB

#define _OgreMXloaderExport

namespace Ogre {

	MXloaderPlugin* plugin;
    //-----------------------------------------------------------------------
    extern "C" void _OgreMXloaderExport dllStartPlugin(void) throw()
    {
		plugin = OGRE_NEW MXloaderPlugin();
		Root::getSingleton().installPlugin(plugin);
    }

    //-----------------------------------------------------------------------
    extern "C" void _OgreMXloaderExport dllStopPlugin(void)
    {
		Root::getSingleton().uninstallPlugin(plugin);
		OGRE_DELETE plugin;

    }
}

#endif
