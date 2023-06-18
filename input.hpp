#pragma once
#include "interface_unit.hpp"
#include <string>

namespace DI{

  union Data{
    int integer;
    float floating_point;
    char character;
  };

  
  class InvisibleInput : public InterfaceUnit{
  protected:
    std::string bufferT;
    ColliderSquare* collider;
    callable callbacks;
    bool isFocused;
  protected:
    virtual void PressedByMouse() override; // Gain focus
    virtual void PressedByKey() override; // Gather keys
    virtual void PressedOutSide() override; // For loosing focus
  public:
    InvisibleInput(glm::vec2 pos, glm::vec2 size);
    ~InvisibleInput();
    // Draw an checkbox(all primitives)
    virtual void Draw() override;
    // Setters
    virtual void SetPosition( glm::vec2 ) override;
    virtual void SetSize( glm::vec2 ) override;
            void SetCollider( ColliderSquare* collider ) override;
            void SetCallback( GUIState state, const std::function<void()> callback ) override;
            void SetText( const char* const text );
    // Getters
    inline std::string      GetText() { return bufferT; }
    inline bool             GetFocus() { return isFocused; }
    inline ColliderSquare*  GetCollider() override { return this->collider; }
    inline callable_iter    GetCallback( GUIState key ) override { return this->callbacks.find(key); }
    inline callable         GetCallbackContainer() override { return this->callbacks; }
  };

  // This class is a base class for all graphycal inputs. And exept any keys
  class StringInput : public InvisibleInput{
  protected:
    int padding;
    std::string lableT;
    Text* buffer;
    Text* lable;
    Primitive* borders;
    Primitive* background;
    Primitive* carret;
    Primitive* bigBackground;
  protected:
    void PressedByMouse() override;
    void PressedByKey() override; 
    void PressedOutSide() override; 
  public:
    StringInput(glm::vec2 pos, glm::vec2 size, std::string lable, std::string hint = "Enter some");
    ~StringInput();
    void Draw() override;
    void SetPosition( glm::vec2 ) override;
    void SetSize( glm::vec2 ) override;
    void SetPrimitive( Primitive* in );
    void SetText( const char* const text, const char* const lable);
    glm::vec2 GetSize() override;
  };

  // This class handle numbers(integers)
  class IntInput : public StringInput{
  protected:
    Data data;
    std::string set;
  private:
    void PressedByMouse() override;
    void PressedByKey() override; 
    void PressedOutSide() override; 
  protected:
    virtual void SetData(Data& data);
  public:
    IntInput(glm::vec2 pos, glm::vec2 size, std::string lable, int data = 0);
    ~IntInput();
    inline const int GetData() const { return data.integer; }
  };

  // This class handle numbers(integers)
  class FloatInput : public IntInput{
  protected:
    void SetData(Data& data) override;
  public:
    FloatInput(glm::vec2 pos, glm::vec2 size, std::string lable, float data = 0);
    ~FloatInput();
    inline const float GetData() const { return data.floating_point; }
  };

  // This class made for handling only one stroke of keyboard
  // After then cleaning cleaning whole buffer
  class CharInput : public StringInput{
  protected:
    char data;
    std::string set;
  private:
    void PressedByMouse() override;
    void PressedByKey() override; 
    void PressedOutSide() override; 
  public:
    CharInput(glm::vec2 pos, glm::vec2 size, std::string lable);
    ~CharInput();
    inline const int GetData() const { return data; }
  };
}
// TODO: Add inputs for vectors and matricies
