#pragma once
#include "interface_unit.hpp"


namespace DI{
  
  class Text;
  class PlaneButton;

  class Slider : public InterfaceUnit{
  protected:
    // Callbacks for usefull logic
    callable callbacks;
    // Primitive to draw
    Primitive*      bigBackground;
    Primitive*      background;
    Primitive*      filler;
    Primitive*      slider;
    // Collider for interaction
    ColliderSquare* collider_on_background;
    // Text also for drawing information
    Text* minT;
    Text* maxT;
    Text* currentT;
    Text* lable;
    // Slider
    glm::vec2 slider_position;
    glm::vec2 slider_size;
    // Range values
    float min;
    float max;
    float current;
    int spacing;
    bool isHolding;
  protected:
    Slider(glm::vec2 pos, float min, float max, float current);
    ~Slider();
    // Callbacks
    virtual void PressedByMouse() override;
    virtual void ReleasedByMouse() override;
    virtual void HoveredByMouse() override;
    virtual void CalculateCurrent(){};
    void SetCurrent(const float new_current);
    void SetMinMax(float new_min, float new_max);
  public: 
    // Draw an checkbox(all primitives)
    void Draw() override;
    // Setters
    virtual void SetText( const char* const lable ){};
    virtual void SetSize( glm::vec2 ) override{};
    virtual void SetPosition( glm::vec2 ) override{};
    void SetCollider( ColliderSquare* cldr) override;
    void SetCallback( GUIState state, const std::function<void()> callback ) override;
    // Getters
    inline Primitive*       GetBackground() { return background; }
    inline Primitive*       GetFiller() { return filler; }
    inline Primitive*       GetSlider() { return slider; }
    inline ColliderSquare*  GetCollider() override { return this->collider_on_background; }
    inline callable_iter    GetCallback( GUIState key ) override { return this->callbacks.find(key); }
    inline callable         GetCallbackContainer() override { return this->callbacks; }
    inline const float&     GetCurrent() { return this->current; }

  };

  class HorizontalSlider : public Slider{
  private:
    void HoveredByMouse() override;
    void CalculateCurrent() override;
  public: 
    HorizontalSlider(glm::vec2 pos, float min, float max, float current);
    void SetText( const char* const lable ) override;
    void SetSize( glm::vec2 ) override;
    void SetPosition( glm::vec2 ) override;
    void PressedByMouse() override;
  };
  
  class VerticalSlider : public Slider{
  private:
    void HoveredByMouse() override;
    void CalculateCurrent() override;
  public: 
    VerticalSlider(glm::vec2 pos, float min, float max, float current);
    void SetText( const char* const lable ) override;
    void SetSize( glm::vec2 ) override;
    void SetPosition( glm::vec2 ) override;
    void PressedByMouse() override;
  };
}


