#ifndef __MXLOADERPLUGIN_H__
#define __MXLOADERPLUGIN_H__
#include <OgrePlugin.h>

namespace Ogre {
	
	class MXloaderPlugin : public Plugin
	{
		
		public:
			MXloaderPlugin();

			// Ogre Plugin members
			Ogre::String const & getName() const;
			void initialise();
			void install();
			void shutdown();
			void uninstall();
	}

}

#endif
