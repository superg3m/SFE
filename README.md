# On going development

There has to be a very well defined init and deinit
because using globals is such a footgun...

// Modules:
- Core
- Platform
- GFX
- Input

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

// Texture Cache (Probably just cache by path is good for now)
// Adjust models to have bone info
// restructure renderer module
// Particles need Dynamic_Draw (for updating the Instance ModelVertexAttribute)
// Sphere: https://andrewnoske.com/wiki/Generating_a_sphere_as_a_3D_mesh
// Implement Fog: https://www.youtube.com/watch?v=qslBNLeSPUc
// Telemetry should be a seperate module thats robust
// Fix the directory structure (the names, the include paths and such)
// Fix the namespacing (has to be less stuff to type)
// Add ECS
// OpenAL (Audio): https://www.youtube.com/watch?v=kWQM1iQ1W0E
// Add GUI Module (Text, Immediate Mode Buttons, Slider, Color Picker, CheckMark, Radio, GUIZMO)
// Add Hot Reloading

// Standardize Shaders
- ShaderModel (pos, normal, texture) (lambertian diffuse shading, specular, emissive)
- ShaderAnimation (pos, normal, texture) (lambertian diffuse shading, specular, emissive)
- ShaderUI (for ui and screen space stuff, text)
- ShaderParticle (pos, texture, instancing and stuff)
- ShaderSkyBox (pos, texture, instancing and stuff)