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

my goal should be to make this as painless as possible:

// vertex buffer object
unsigned int buffer;
glGenBuffers(1, &buffer);
glBindBuffer(GL_ARRAY_BUFFER, buffer);
glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

// REMEMBER THIS IS FOR EVERY SINGLE MESH INSIDE THE MODEL
for(unsigned int i = 0; i < rock.meshes.size(); i++)
{
    unsigned int VAO = rock.meshes[i].VAO;
    glBindVertexArray(VAO);
    // vertex attributes
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

// Keep track of locations and then assert a location is not being used twice
geo.addVertexAttribute(location, T value);
geo.addInstanceVertexAttribute(location, Vector<T> values);