#pragma once 

#include "DIPrimitives/primitives.hpp"
#include "DIPhysics/collider.hpp"
#include "interface_unit.hpp"

namespace DI{

  class Text;

  class InvisibleButton : public InterfaceUnit{
  protected:
    // Callbacks for usefull logic
    callable callbacks;
    // Collider for interaction
    ColliderSquare* collider;
  public:

    InvisibleButton(glm::vec2 pos, glm::vec2 size);
    ~InvisibleButton();
    void Draw() override;
    // Callbacks
    void PressedByMouse() override;
    void ReleasedByMouse() override;
    void HoveredByMouse() override;
    // Setters
    virtual void SetSize( glm::vec2 size) override;
    virtual void SetPosition( glm::vec2 pos) override;
    virtual void SetCollider( ColliderSquare* collider) override;
    virtual void SetCallback( GUIState state, const std::function<void()> callback ) override;
    // Getters
    inline ColliderSquare*  GetCollider() override { return this->collider; }
    inline callable_iter    GetCallback( GUIState key ) override { return this->callbacks.find(key); }
    inline callable         GetCallbackContainer() override { return this->callbacks; }
  };

  class PlaneButton : public InvisibleButton{
  protected:
    // Primitive to draw
    Primitive* primitive;
  public: 

    PlaneButton(glm::vec2 pos, glm::vec2 size);
    ~PlaneButton();
    void Draw() override;
    // Setters
            void SetPrimitive( Primitive* primitive );
    virtual void SetSize( glm::vec2 ) override ;
    virtual void SetPosition( glm::vec2 ) override ;
    // Getters
    inline Primitive*       GetPrimitive() { return this->primitive; }
  };

  class TextButton : public PlaneButton{
  private:
    // Text also for drawing information
    Text* text;
    int   padding;
  public:
    TextButton(const char* lable, glm::vec2 pos, glm::vec2 size);
    ~TextButton();
    void Draw() override;
    // Setters
            void SetText( const char* text );
    virtual void SetSize( glm::vec2 ) override ;
    virtual void SetPosition( glm::vec2 ) override ;
    // Getters
    inline Text*            GetText() { return this->text; }
  };
}
