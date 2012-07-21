#include "OgreRoot.h"
#include "mxloader_plugin.h"

#ifndef OGRE_STATIC_LIB

namespace Ogre {

	MXloaderPlugin* plugin;
    //-----------------------------------------------------------------------
    extern "C" void _OgreParticleFXExport dllStartPlugin(void) throw()
    {
		plugin = OGRE_NEW MXloaderPlugin();
		Root::getSingleton().installPlugin(plugin);
    }

    //-----------------------------------------------------------------------
    extern "C" void _OgreParticleFXExport dllStopPlugin(void)
    {
		Root::getSingleton().uninstallPlugin(plugin);
		OGRE_DELETE plugin;

    }
}

#endif
