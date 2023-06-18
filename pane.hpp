#pragma once

#include "interface_container.hpp"

namespace DI{


  // Where to stick(wich side of window)
  enum StickySide{
    TOP_SIDE,
    BOTTOM_SIDE,
    LEFT_SIDE,
    RIGHT_SIDE,
  };

  // Pane is a general surface for containing InterfaceUnits
  // Placment this InterfaceContainer are limited
  // But you still can change size or paddings 
  class Pane : public InterfaceContainer{
  private:
    void DrawThisContainer() override;
  protected:
    Primitive* background;
    StickySide side;
    // Space between Pane and window sides 
    int padding;
    // This limited field for positioning this InterfaceContainer related to sides of windows
    glm::vec2 offset;
  public: 
    // Constructors/Destructor
    Pane(StickySide side, glm::vec2 size, glm::vec2 offset = glm::vec2(0), int padding = 0);
    ~Pane();
    // Setters
    // Where to stick
    void SetStickTo(StickySide side);
    void SetSize(glm::vec2 size) override;
    // Set position relative to side of window
    void SetOffset( glm::vec2 offset);
    void SetPadding( int padding);
    // Getters
    inline glm::vec2 GetOffset() { return offset; }
    inline int GetPadding() { return padding; }
    inline StickySide GetSide() { return side; }
    inline Primitive* GetPrimitive() { return background; }
  };

}
