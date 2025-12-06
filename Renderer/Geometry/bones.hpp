#pragma once

#include <Math/math.hpp>
#include <DataStructure/ds.hpp>

// https://bartwaszak.com/introduction-to-the-skeletal-animation-technique/

struct Bone {
    Math::Mat4 local_transform;
    Math::Mat4 world_transform;
    int parent_index;
};

static const int MAX_BONES_IN_SKELETON = 4;
struct Bone Skeleton[MAX_BONES_IN_SKELETON];

static const int NUMBER_OF_VERTICES_IN_SKIN_MESH = 600;
struct SkinMeshVertex SkinMesh[NUMBER_OF_VERTICES_IN_SKIN_MESH];

void UpdateSkeleton_EvaluteWorldMatrices() {
    Skeleton[0].world_transform = Skeleton[0].local_transform;

    for (int i=1; i < MAX_BONES_IN_SKELETON; ++i) {
        const int parent = Skeleton[i].parent_index;
        Skeleton[i].world_transform = Skeleton[i].local_transform * Skeleton[parent].world_transform;
    }
}

void UpdateSkinMesh_EvaluateVerticesPosition(void) {
    Math::Vec3 v1, v2, v3, v4;
    Math::Vec3 h1, h2, h3, h4;
 
    // For every vertex in the skin mesh
    for (int i=0; i < NUMBER_OF_VERTICES_IN_SKIN_MESH; ++i) {
        v1 = Math::Vec3(Skeleton[0].world_transform * Math::Vec4(SkinMesh[i].local_position, 1));
        v2 = Math::Vec3(Skeleton[1].world_transform * Math::Vec4(SkinMesh[i].local_position, 1));
        v3 = Math::Vec3(Skeleton[2].world_transform * Math::Vec4(SkinMesh[i].local_position, 1));
        v4 = Math::Vec3(Skeleton[3].world_transform * Math::Vec4(SkinMesh[i].local_position, 1));
 
        h1 = v1.scale(SkinMesh[i].weights.x);
        h2 = v2.scale(SkinMesh[i].weights.y);
        h3 = v3.scale(SkinMesh[i].weights.z);
        h4 = v4.scale(SkinMesh[i].weights.w);
 
        // Sum all vectors to get the final world positon for vertex
        for (int j = 0; j <= 2; j++) {                          
            SkinMesh[i].world_position = (h1 + h2 + h3 + h4);                  
        }          
    }  
}  

struct SkinMeshVertex {
    Math::Vec3 local_position; // Local space
    Math::Vec3 world_position; // World space
    Math::Vec4 weights; // The weights should sum to 1
};

void CreateSkeleton() {
    Skeleton[0].parent_index = 0;
    Skeleton[1].parent_index = 0;
    Skeleton[2].parent_index = 1;
    Skeleton[3].parent_index = 2;
}
 
void EvaluateSkinWeightForVertex(SkinMeshVertex& v) {
    if (v.local_position.y < 0.25f) {
        v.weights.x = (1.0f - (v.local_position.y * 4.0f));
        v.weights.y = (1.0f - v.weights.x);
        v.weights.z = (0.0f);
        v.weights.w = (0.0f);
    } else if (v.local_position.y < 0.50f) {
        v.weights.x = (0.0f);
        v.weights.y = (1.0f-((v.local_position.y - 0.25f) * 4.0f));
        v.weights.z = (1.0f-(v.weights.y));
        v.weights.w = (0.0f);
    } else if (v.local_position.y < 0.75f) {
        v.weights.x = (0.0f);
        v.weights.y = (0.0f);
        v.weights.z = (1.0f-((v.local_position.y - 0.50f) * 4.0f));
        v.weights.w = (1.0f-(v.weights.z));
    } else {
        v.weights = Math::Vec4(0.0f);
    }
}
 
void CreateSkinMeshVertices(void) {
    Math::Vec3 v1, v2, v3, v4;
 
    float Y = 0.0f;
    const float YStepUp = 0.01f;
 
    float Thickness = 0.31f;
    const float ThickStepDown = 0.003f;
 
    for(int i = 0; i < NUMBER_OF_VERTICES_IN_SKIN_MESH; i += 6) {
        v1.x = (-Thickness);
        v1.y = (Y);
        v1.z = (0.0f);
 
        v2.x = (+Thickness);
        v2.y = (Y);
        v2.z = (0.0f);
 
        v3.x = (-Thickness);
        v3.y = (Y+YStepUp);
        v3.z = (0.0f);
 
        v4.x = (+Thickness);
        v4.y = (Y+YStepUp);
        v4.z = (0.0f);
 
        // Triangle #1
        SkinMesh[i].local_position   = v1;
        SkinMesh[i+1].local_position = v2;
        SkinMesh[i+2].local_position = v3;
 
        // Triangle #2
        SkinMesh[i+3].local_position = v2;
        SkinMesh[i+4].local_position = v4;
        SkinMesh[i+5].local_position = v3;
 
        EvaluateSkinWeightForVertex( SkinMesh[i] );
        EvaluateSkinWeightForVertex( SkinMesh[i+1] );
        EvaluateSkinWeightForVertex( SkinMesh[i+2] );
        EvaluateSkinWeightForVertex( SkinMesh[i+3] );
        EvaluateSkinWeightForVertex( SkinMesh[i+4] );
        EvaluateSkinWeightForVertex( SkinMesh[i+5] );
 
        Y += YStepUp;
        Thickness -= ThickStepDown;
    }
}
 
void UpdateSkeleton_Movement(float dt) {
    const float F1 = dt / 3;
    const float F2 = dt;
    const float F3 = dt * 2;
 
    Math::Mat4 mT = Math::Mat4::Identity();
    Math::Mat4 mR = Math::Mat4::Identity();
    Math::Mat4 identity = Math::Mat4::Identity();

    Skeleton[0].local_transform = Math::Mat4::Translate(identity, 0, -1.0f, 0);
 
    float rot1 = sin(TAU * F1) * 0.52f;
    mT = Math::Mat4::Translate(identity, 0.0f , 1.25f , 0.0f);
    mR = Math::Mat4::Rotate(identity, Math::Quat::FromEuler(0, 0, rot1));
    Skeleton[1].local_transform = mT * mR;;
 
    float rot2 = sin(TAU * F2) * 0.30f;
    mT = Math::Mat4::Translate(identity, 0.0f, 0.80f, 0.0f);
    mR = Math::Mat4::Rotate(identity, Math::Quat::FromEuler(0, 0, rot2));
    Skeleton[2].local_transform = mT * mR;

    float rot3 = sin(TAU * F3) * 0.20f;
    mT = Math::Mat4::Translate(identity, 0.0f, 0.60f, 0.0f);
    mR = Math::Mat4::Rotate(identity, Math::Quat::FromEuler(0, 0, rot3));
    Skeleton[3].local_transform = mT * mR;
}
 
void SkeletalAnimationTechnique_INIT(void) {
    CreateSkeleton();
    CreateSkinMeshVertices();
}
 
void SkeletalAnimationTechnique_UPDATE(float dt) {
    UpdateSkeleton_Movement(dt);
    UpdateSkeleton_EvaluteWorldMatrices();
    UpdateSkinMesh_EvaluateVerticesPosition();
}