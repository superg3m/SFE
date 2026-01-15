# On going development

// Add ECS: https://austinmorlan.com/posts/entity_component_system/
// Texture Cache (Probably just cache by path is good for now)
// Adjust models to have bone info
// restructure renderer module
// Sphere: https://andrewnoske.com/wiki/Generating_a_sphere_as_a_3D_mesh
// Implement Fog: https://www.youtube.com/watch?v=qslBNLeSPUc
// Telemetry should be a seperate module thats robust
// Fix the directory structure (the names, the include paths and such)
// Fix the namespacing (has to be less stuff to type)
// OpenAL (Audio): https://www.youtube.com/watch?v=kWQM1iQ1W0E
// Add GUI Module (Text, Immediate Mode Buttons, Slider, Color Picker, CheckMark, Radio, GUIZMO)
// Add Hot Reloading

// Standardize Shaders
- ShaderMaterial (pos, normal, texture) (lambertian diffuse shading, specular, emissive)
- ShaderAnimation (pos, normal, texture) (lambertian diffuse shading, specular, emissive)
- ShaderUI (for ui and screen space stuff, text)
- ShaderParticle (pos, texture, instancing and stuff)
- ShaderSkyBox (pos, texture, instancing and stuff)

// CPU Profiler Module! Right now i'm hard cpu bound I wonder why?
// start optimizing the math library