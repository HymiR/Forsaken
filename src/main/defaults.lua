debug = 2
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
drawbasis = false
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

joystick = false

-- Map loader to use
level = 'level/fsknmx.lua'

-- Camera physics (temporary)
accel = 3000
drag = 50
turnaccel = 500
turndrag = 30

-- Temporary options for level/model to load and texture directory
--texdir = '../data/data/textures/'

-- FIXME currently only 256 bit indexed *.bmp textures woking.
-- TODO  write a routine to load *.png textures.
-- So textures from ../../data/data/level/*/textures don't work for now.
--
--
texdir = '../data/data/levels/arena/textures/'

lvlfile = 'level/arena/arena.mxv'
lvltexdir = 'level/arena/textures'

