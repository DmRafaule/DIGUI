#pragma once

#include "DIRenderer/drawable.hpp"
#include "DIWindow/viewport.hpp"

namespace DI{

  struct ColliderPoint;
  struct ColliderSquare;
  struct Font;
  class OrthographicCamera;
  class InterfaceUnit;
  class InterfacerSingleton;
  class Primitive;
  class Text;

  enum GUIState{
    NORMAL,
    HOVERED,
    PRESSED,
    RELEASED,
    TOGGLED,
    DRAGGED,
  };

  typedef std::unordered_map<GUIState,const std::function<void()>>::iterator callable_iter;
  typedef std::unordered_map<GUIState,const std::function<void()>> callable;

  class InterfaceUnit : public Drawable{
  friend class InterfacerSingleton;
  friend class InterfaceContainer;
  private:
    virtual void Draw() = 0;
  protected:
    InterfaceUnit();
    ~InterfaceUnit();
  public:
    // Default callback func interface 
    virtual void PressedByMouse(){};
    virtual void PressedOutSide(){};
    virtual void ReleasedByMouse(){};
    virtual void HoveredByMouse(){};
    virtual void PressedByKey(){};
    virtual void ReleasedByKey(){};
    // Setters
    virtual void SetPosition( glm::vec2 ){};
    virtual void SetSize( glm::vec2 ){};
    virtual void SetCallback(  GUIState state, const std::function<void()> callback ){};
    virtual void SetCollider( ColliderSquare* cldr ){};
    // Getters
    virtual ColliderSquare* GetCollider() = 0;
    virtual callable_iter   GetCallback( GUIState key ) = 0;
    virtual callable        GetCallbackContainer() = 0;
    virtual glm::vec2        GetPosition() { return this->position; }
    virtual glm::vec2        GetSize() { return this->size; }
    inline const int        GetId() { return id; }
    inline const int        GetNextId() { return next_id; }
    inline const ViewportsSupportedID GetViewportId() { return viewport_id; }
  protected:
    int id;
    static int next_id;
    bool isInContainer;
    ViewportsSupportedID viewport_id;
    glm::vec2 position;
    glm::vec2 size;
  };
}
// TODO: Implement rotation method

