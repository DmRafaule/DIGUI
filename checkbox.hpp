#pragma once
#include "interface_unit.hpp"


namespace DI{
  
  class Text;

  class InvisibleCheckbox : public InterfaceUnit{
  protected:
    // Callbacks for usefull logic
    callable callbacks;
    // Collider for interaction
    ColliderSquare* collider;
    bool isSwitched;
  public: 
    InvisibleCheckbox(glm::vec2 pos);
    ~InvisibleCheckbox();
    // Draw an checkbox(all primitives)
    void Draw() override;
    // Callbacks
    void PressedByMouse() override;
    // Setters 
    virtual void SetSize(glm::vec2) override;
    virtual void SetPosition(glm::vec2) override;
    virtual void SetCollider( ColliderSquare* collider) override;
    virtual void SetCallback( GUIState state, const std::function<void()> callback ) override;
    inline  void SetSwitch( const bool is ) { this->isSwitched = is; }
    // Getters
    inline ColliderSquare*  GetCollider() override { return this->collider; }
    inline callable_iter    GetCallback( GUIState key ) override { return this->callbacks.find(key); }
    inline callable         GetCallbackContainer() override { return this->callbacks; }
    inline const bool&      GetSwitch() { return isSwitched; }
  };

  class PlaneCheckbox : public InvisibleCheckbox{
  protected:
    // Primitive to draw
    Primitive* box;
    Primitive* cross;
  public: 
    PlaneCheckbox(glm::vec2 pos);
    ~PlaneCheckbox();
    // Draw an checkbox(all primitives)
    void Draw() override;
    // Setters 
    void SetSize(glm::vec2) override;
    void SetPosition(glm::vec2) override;
    void SetPrimitive(Primitive* in);
    // Getters
    inline Primitive*       GetPrimitive( ) { return this->box; };
  };

  class TextCheckbox : public PlaneCheckbox{
  protected:
    // Text also for drawing information
    Text* text;
    int padding;
    Primitive* background;
  public: 
    TextCheckbox(const char* const lable, glm::vec2 pos);
    ~TextCheckbox();
    // Draw an checkbox(all primitives)
    void Draw() override;
    // Setters 
    void SetPosition(glm::vec2) override;
    void SetSize(glm::vec2) override;
    void SetText(const char* const text) ;
    void SetPadding(int padding);
    // Getters
    inline Text* GetText() { return this->text; }
    inline Primitive* GetBackground() { return this->background; }
    glm::vec2 GetSize() override; 
    glm::vec2 GetPosition() override;
  };
}
