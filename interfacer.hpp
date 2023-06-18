#pragma once

#include "DIRenderer/renderer.hpp"
#include "DIWindow/viewport.hpp"
#include <map>

namespace DI{

  class OrthographicCamera;
  class Font;
  class InterfaceUnit;
  struct ColliderPoint;
  struct ViewportData;

  // Singleton for InterfaceUnit's objects
  class InterfacerSingleton {
    friend class Engine;
    friend class Editor;
    private:
      InterfacerSingleton(){};
      // Initialize all data in App constructor
      static void Init();
      // Remove all data in App destructor 
      static void Kill();
    private:
      // Uniqe instance for InterfacerSingleton object
      static InterfacerSingleton*    m_instance;
      // Default shader for UI
      Shader* flat_shader;
      Shader* texture_shader;
      // Default camera for UI
      OrthographicCamera* camera;
      // RendererSingleton instance
      RendererSingleton*  rendererSingleton;
      // Font to render text
      Font*                 font;
      // Containing all UI elements for interaction(Pool)
      std::vector<InterfaceUnit*> units;
      // Simple point collider for interacting with UI colliders
      ColliderPoint*        mouse_collider;
      // Default mouse position for 'static' OrthographicCamera
      glm::vec2             mouse_position;
    public:
      // Containment
      // Add unit to pool
      static void Add(InterfaceUnit* unit, ViewportsSupportedID id);
      // Remove specific unit from pool
      void Remove(InterfaceUnit* unit);      
      // Remove all units from pool
      void Clear();
      // Getters
      // Get instance of InterfacerSingleton
      static InterfacerSingleton* GetInstance();
      // Get currently using camera
      inline OrthographicCamera* GetCamera() { return camera; } 
      // Get currently loaded and using font 
      inline Font* GetFont() { return font; }
      // Get shader by it's type(PrimitiveMaterial enum)
      Shader* GetShader(int type);
      // Get pool of units
      inline std::vector<InterfaceUnit*> GetElements() { return units;} 
      // Get specific unit by it's id
      InterfaceUnit* Get(const int id); 
      inline ColliderPoint* GetMouseCollider() { return mouse_collider; }
      inline glm::vec2& GetMousePosition() { return mouse_position; }
  };
}

// TODO:  1)  Implement collisions "Sort and Sweep" alg
//        2)  Implement collisions "Dynamic Bounding Volume Trees" alg
//        3)  Implement collisions "Uniform Grid Based" alg
//        4)  Implement multithreding collisions detection
