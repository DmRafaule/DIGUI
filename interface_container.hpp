#pragma once

#include "interface_unit.hpp"

namespace DI{

  class InterfaceContainer : public InterfaceUnit{
  private:
    // This is for disabling collision for InterfaceContainer class and their derived
    using InterfaceUnit::PressedByKey;
    using InterfaceUnit::PressedByMouse;
    using InterfaceUnit::ReleasedByKey;
    using InterfaceUnit::ReleasedByMouse;
    using InterfaceUnit::HoveredByMouse;
    using InterfaceUnit::PressedOutSide;
    // Not used actually (I know ... You should avoid such implementation.)
    // But dude I need InterfaceUnit as a base class so ... here we are :)
    inline  ColliderSquare*  GetCollider() override { return collider; };
    inline  callable_iter    GetCallback( GUIState key ) override { return callbacks.end(); };
    inline  callable         GetCallbackContainer() override { return callbacks; };
  private:
    callable          callbacks;
    ColliderSquare*   collider;
  protected:
    // Constructors/Destructor
    InterfaceContainer();
    ~InterfaceContainer();
    // Custom children drawing functions
    virtual void DrawThisContainer() = 0;
  public:
    // Draw all children
    void Draw() override;
    // Setters
    virtual void SetPosition( glm::vec2 ) override;
    virtual void SetSize( glm::vec2 ) override;
    // Containments
    void Add(InterfaceUnit* unit, ViewportsSupportedID viewport_id);
    void Remove(InterfaceUnit* unit);
    void Clear();
    // Getters
    // Get InterfaceUnit by specific id
    InterfaceUnit* Get(const int id);
  protected:
    // Instance of InterfacerSingleton class for manipulating a memory
    InterfacerSingleton*       gui_handler;
    ViewportsSupportedID viewport_id;
    // This vector actualy holding only id's of InterfaceUnits
    std::vector<int>  children;
  };

}

