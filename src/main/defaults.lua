debug = 0
debuglvl = 0
debugmask = 0

-- Video configuration
fullscreen = true
width = 1024
height = 768
bpp = 24
vsync = true
gamma = 0 -- if positive, set the screen gamma value
drawnormals = false
drawbasis = true
stereo = false
stereorightcolor = 0 -- 0 = green, 1 = blue, 2 = cyan
stereoeyesep = 20
stereofocaldist = 750
-- Temporary - materials make things really slow right now
materials = true

-- Audio configuration
audiofreq = 44100
audiobufsize = 1024

-- Input configuration
mousesensx = 0.6 -- Negative values will result in an inverted axis
mousesensy = 0.6

-- TODO make deactivation of that vlaue dependant of SDL_JOY in /make.global file.
joystick = false

-- Level to load
level = 'level/fsknmx.lua'

-- Camera physics (temporary)
accel = 3000
drag = 50
turnaccel = 500
turndrag = 30

-- Temporary options for level/model to load and texture directory
--texdir = '../data/data/textures/'

-- FIXME file level/fsknmx.lua is buggy:
-- texture loading form levels in data/data/levels/* doses not work properly!
--
--texdir = '../data/data/levels/arena/textures/'
--lvlfile = '../data/data/levels/arena/arena.mxv'
--lvltexdir = '../data/data/levels/arena/textures/'

-- level specific texture files not in separate folder here...
texdir = 'level/ship'
lvlfile = 'level/ship/ship.mxv'
lvltexdir = 'level/ship'

