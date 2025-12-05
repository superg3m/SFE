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

particles and instance renderering