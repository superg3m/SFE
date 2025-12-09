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
Math::Vec3 this is annoying but I think necessary


man this naming situation is really annoying, come back to this with a fresh pair of eyes.

// Core
- Math::Vec3::Distance(Math::Vec3(1.0f), Math::Vec3(5.0f));
- Memory::Copy(...)
- Memory::Malloc(...)
- Memory::Free(...)
- String::Copy(...)
- String::Contains(...)
- DS::Vector<char*>(...)

// Platform
- Platform::ReadEntireFile(file);
- Platform::Sleep(32);

// Input
- Input::GetKeyDown(Input::KEY_A)

particles

my goal should be to make this as painless as possible:

// OpenAL (Audio)
https://www.youtube.com/watch?v=kWQM1iQ1W0E

// texture cache
// adjust models to have bone info

// terrain

// Telemetry should be a seperate module thats robust

// restructure renderer module
// Platform should have window stuff, including glfw

// Sphere: https://andrewnoske.com/wiki/Generating_a_sphere_as_a_3D_mesh