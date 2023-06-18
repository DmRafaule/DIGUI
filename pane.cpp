#include "pane.hpp"
#include "DIPrimitives/primitives.hpp"
#include "interfacer.hpp"
#include "DIWindow/win.hpp"

namespace DI{
  
  extern WinData* eg_winData;

  Pane::Pane(StickySide side, glm::vec2 sz, glm::vec2 offset, int padding){
    position = glm::vec2(0);
    size = sz;
    SetPadding(padding);
    SetOffset(offset);
    // Set up a default background
    background = new Reqtangle(POSITION_AND_COLORS);
    background->SetShader(InterfacerSingleton::GetInstance()->GetShader(POSITION_AND_COLORS));
    background->SetCamera(*InterfacerSingleton::GetInstance()->GetCamera());
    // Set up initial position and size
    SetSize(size);
    SetStickTo(side); 
  }
  Pane::~Pane(){
    delete background; 
  }
  void Pane::DrawThisContainer(){
    RendererSingleton::GetInstance()->Draw(*background);    
  }
  void Pane::SetOffset( glm::vec2 offset){
    this->offset = offset; 
  }
  void Pane::SetPadding( int padding){
    this->padding = padding;
  }
  void Pane::SetStickTo(StickySide side){
    // Notice that I'm not using window data because it's not handled by camera.
    // If camera orthographic we have stable(not changing ratio and size) so this is why using camera properties
    int winW = gui_handler->GetCamera()->GetRight(), winH = gui_handler->GetCamera()->GetTop();
    this->side = side; 
    background->ResetModelMatrix();
    // Define pivot point of Pane
    switch(side){
      case TOP_SIDE:{
        position = glm::vec2(winW/2.f + offset.x,winH - size.y/2.f + offset.y);
        break;
      }
      case BOTTOM_SIDE:{
        position = glm::vec2(winW/2.f + offset.x,size.y/2.f + offset.y);
        break;
      }
      case RIGHT_SIDE:{
        position = glm::vec2(winW-size.x/2.f + offset.x,winH/2.f + offset.y);
        break;
      }
      case LEFT_SIDE:{
        position = glm::vec2(size.x/2.f + offset.x,winH/2.f + offset.y);
        break;
      }
    }
    background->SetPosition(glm::vec3(position.x,position.y,0));
    background->SetScale(glm::vec3(size.x/2- padding,size.y/2-padding,0.f));
  }
  void Pane::SetSize(glm::vec2 size){
    int winW = eg_winData->size.x, winH = eg_winData->size.y;
    this->size = size;
    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(position.x+ offset.x,position.y+ offset.y,0));
    background->SetScale(glm::vec3(size.x/2-padding,size.y/2-padding,0.f));
  }   
}
