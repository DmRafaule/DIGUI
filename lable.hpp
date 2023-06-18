#pragma once

#include "DIPrimitives/primitives.hpp"
#include "interface_unit.hpp"

namespace DI{

  class Text;

  class Lable : public InterfaceUnit{
  private:
    inline ColliderSquare*  GetCollider() override { return this->collider; }
    inline callable_iter    GetCallback( GUIState key ) override { return this->callbacks.end(); }
    inline callable         GetCallbackContainer() override { return this->callbacks; }
  private:
    ColliderSquare* collider;
    callable callbacks;
    int padding;
    Text* lable;
    const char* string;
    Primitive*  background;
  public:
    // Constructros/Destructor
    Lable(glm::vec2 pos, const char* str );
    ~Lable();
    void Draw() override;
    // Setters
    virtual void SetSize( glm::vec2 size) override;
    virtual void SetPosition( glm::vec2 pos) override;
    // Changing displaying text, and update size and position of InterfaceUnit
    void SetLable( const char* str);
    // Accept only already ready Text objects(this means they also have to have attached font)
    void SetLable( Text* str);
    void SetPrimitive( Primitive* prmt);
    // Getters
    inline Text* GetLable() { return lable; }
    inline const char* GetString() { return string; }
    inline Primitive* GetPrimitive() { return background; }
  };

}
