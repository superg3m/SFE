// use <sfe_shader.hpp> as much as possible
// all files lowercase sfe_lower_case.cpp
// StructName
// variable_name
// functionName()

// constructors and destructors first
// public methods
// static public at the end of everything else
// lastly private stuff, all other rules apply as well

// V_Compile() for virtual functions

int main() {
    Geometry dog_model = Geometry::Model("dog-model.obj");

    // shader.use()
    // shader.setColor();
    dog_model.draw();
}