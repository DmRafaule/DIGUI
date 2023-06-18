#pragma once

#include "interface_container.hpp"

namespace DI{

  // Main theme of this GUI Container are that
  // There are may exist only one popup
  // It's like a single ton but you can create variose types
  // of popups
  class Popup : public InterfaceContainer{
  private:
    // Pair that ensure there will be only one popup per window
    static std::pair<bool,int*> isActive;
  private:
    void DrawThisContainer() override;
  protected:
    Primitive* background;
    int padding;
    int marging;
  public: 
    Popup( glm::vec2 pos, glm::vec2 size );
    ~Popup();
    void SetSize( glm::vec2 size ) override;
    void SetPosition( glm::vec2 pos ) override;
    void SetPadding (int padding );
    void UpdateSize();
    void UpdatePosition();
    inline  Primitive*  GetPrimitive() { return background; }
  };

}
