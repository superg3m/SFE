There has to be a very well defined init and deinit
because using globals is such a footgun...

sound bruh
entities
animations (bones animation)

// Modules:
- Core
- Platform
- Renderer
- Input

remove as many inter-module dpenendencies as possilbe

renmae stuff like

Math::Vec3 to

GM_Vec3

