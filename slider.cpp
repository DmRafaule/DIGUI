#include "interfacer.hpp"

#include "DIPrimitives/primitives.hpp"
#include "DIWindow/win.hpp"
#include "slider.hpp"
#include "DIPhysics/collider.hpp"
#include "DIEvents/event.hpp"
#include "button.hpp"
#include "DIParser/text.hpp"
#include "DIUtils/utils.hpp"
#include "DIDebuger/log.hpp"


namespace DI{

  extern WinData* eg_winData;
  extern EventData* eg_eventData;

/***************************Slider***************************/
  
  Slider::Slider(glm::vec2 pos, float min, float max, float current){
    InterfacerSingleton* guiInstance = InterfacerSingleton::GetInstance();
    // Set inhereted fields
    spacing = 10;
    pos  = pos;
    isHolding   = false;
    slider_position = pos;
    slider_size = glm::vec2(30);
    SetMinMax(min, max);
    SetCurrent(current);
    collider_on_background = new ColliderSquare();
    minT = new Text(Utils::ToStr(min));
    maxT = new Text(Utils::ToStr(max));
    currentT = new Text(Utils::ToStr(current));
    lable = new Text("Slider");
    // Set primitives 
    bigBackground = new Reqtangle(PrimitiveMaterial::POSITION_AND_COLORS);
    bigBackground->SetShader(guiInstance->GetShader(PrimitiveMaterial::POSITION_AND_COLORS));
    bigBackground->SetCamera(*guiInstance->GetCamera());
    bigBackground->SetColor(glm::vec4(0/255.f, 0/255.f, 110/255.f,1.0));
    background = new Reqtangle(PrimitiveMaterial::POSITION_AND_COLORS);
    background->SetShader(guiInstance->GetShader(PrimitiveMaterial::POSITION_AND_COLORS));
    background->SetCamera(*guiInstance->GetCamera());
    background->SetColor(glm::vec4(255/255.f, 255/255.f, 255/255.f, 1.0f));
    filler = new Reqtangle(PrimitiveMaterial::POSITION_AND_COLORS);
    filler->SetShader(guiInstance->GetShader(PrimitiveMaterial::POSITION_AND_COLORS));
    filler->SetCamera(*guiInstance->GetCamera());
    filler->SetColor(glm::vec4(0/255.f, 0/255.f, 0/255.f, 0.0f));
    slider = new Reqtangle(PrimitiveMaterial::POSITION_AND_COLORS);
    slider->SetShader(guiInstance->GetShader(PrimitiveMaterial::POSITION_AND_COLORS));
    slider->SetCamera(*guiInstance->GetCamera());
    slider->SetColor(glm::vec4(133/255.f, 5/255.f, 30/255.f, 1.0f));
    // Set physical collider
    SetCollider(collider_on_background);
    // Set default text
    minT->ApplyFont(InterfacerSingleton::GetInstance()->GetFont());
    maxT->ApplyFont(InterfacerSingleton::GetInstance()->GetFont());
    currentT->ApplyFont(InterfacerSingleton::GetInstance()->GetFont());
    lable->ApplyFont(InterfacerSingleton::GetInstance()->GetFont());
  }
  Slider::~Slider(){
    delete bigBackground;
    delete slider;
    delete background;
    delete filler;  
    delete minT;
    delete maxT;
    delete currentT;
    delete lable;
    PhysicsHandler::UnSet(collider_on_background);
    callbacks.clear();
  }

  void Slider::Draw() {
    RendererSingleton::GetInstance()->Draw(*slider);    
    RendererSingleton::GetInstance()->Draw(*filler);    
    RendererSingleton::GetInstance()->Draw(*background);    
    RendererSingleton::GetInstance()->Draw(*bigBackground);    
    RendererSingleton::GetInstance()->Draw(*minT);
    RendererSingleton::GetInstance()->Draw(*maxT);
    RendererSingleton::GetInstance()->Draw(*currentT);
    RendererSingleton::GetInstance()->Draw(*lable);
  }

  void Slider::SetCollider( ColliderSquare* cldr){
    PhysicsHandler::SetColliderSquare(cldr, glm::vec2(position.x-size.x/2,position.y-size.y/2),size);
    collider_on_background = cldr;
  }

  void Slider::SetCallback( GUIState state, const std::function<void()> callback){
    // Do not need for check if some callback already exist. It will rewrite mem for new one
    callbacks.emplace(state,callback);
  }  

  void Slider::SetCurrent(const float new_current){
    current = new_current;  
    // Ensure that current value will not out of range
    if (new_current > max)
      current = max;
    else if ( new_current < min)
      current = min;
  }

  void Slider::SetMinMax(float new_min, float new_max){
    // Swap values if they are wrong meaning(min is more than max)
    if (new_min > new_max){
      float buf = new_min;
      new_min = new_max;
      new_max = buf;
    }

    min = new_min;
    max = new_max;
  }

  void Slider::PressedByMouse(){
    isHolding = true;
  }
  
  void Slider::ReleasedByMouse(){
    isHolding = false;
  }
  
  void Slider::HoveredByMouse(){
  }

/***************************HorizontalSlider***************************/


  HorizontalSlider::HorizontalSlider(glm::vec2 pos, float min, float max, float current) : 
    Slider(pos,min,max,current){
    size = glm::vec2(100.f,10.f);
    SetPosition(pos);
    SetSize(size);
    CalculateCurrent();
  }

  void HorizontalSlider::SetSize( glm::vec2 size ){
    bigBackground->ResetModelMatrix();
    int w = (size.x + minT->GetSize().x + maxT->GetSize().x + lable->GetSize().x + spacing * 4)/2;
    int h = (size.y + currentT->GetSize().y + spacing*2)/2;
    bigBackground->SetPosition(glm::vec3((position.x - minT->GetSize().x - spacing - size.x/2) + w  ,(position.y - size.y/2- spacing/2) + h,0.f));
    bigBackground->SetScale(glm::vec3(w,h,0.f));

    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(position.x,position.y,0.f));
    background->SetScale(glm::vec3(size.x/2,size.y/2,0.f));

    filler->ResetModelMatrix();
    filler->SetPosition(glm::vec3(position.x,position.y,0.f));
    filler->SetScale(glm::vec3(size.x/2 * 0.95,size.y/2 * 0.8 ,0.f));

    slider_size = glm::vec2(size.y,size.y*2);
    slider->ResetModelMatrix();
    slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
    slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));

    minT->SetPosition(glm::vec2(position.x - size.x/2 - minT->GetSize().x/2 - spacing,position.y));
    maxT->SetPosition(glm::vec2(position.x + size.x/2 + maxT->GetSize().x/2 + spacing,position.y));
    currentT->SetPosition(glm::vec2(slider_position.x,position.y + currentT->GetSize().y + spacing));
    lable->SetPosition(glm::vec2(position.x + size.x/2 + lable->GetSize().x/2 + maxT->GetSize().x + spacing * 2,position.y));

    this->size = size;
    SetCollider(collider_on_background);
  }

  void HorizontalSlider::SetPosition( glm::vec2 pos){
    bigBackground->ResetModelMatrix();
    int w = (size.x + minT->GetSize().x + maxT->GetSize().x + lable->GetSize().x + spacing * 4)/2;
    int h = (size.y + currentT->GetSize().y + spacing*2)/2;
    bigBackground->SetPosition(glm::vec3((pos.x - minT->GetSize().x - spacing - size.x/2) + w  ,(pos.y - size.y/2- spacing/2) + h,0.f));
    bigBackground->SetScale(glm::vec3(w,h,0.f));

    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    background->SetScale(glm::vec3(size.x/2,size.y/2,0.f));

    filler->ResetModelMatrix();
    filler->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    filler->SetScale(glm::vec3(size.x/2 * 0.95,size.y/2 * 0.8 ,0.f));

    slider_position = glm::vec2(pos.x - size.x/2.f + size.x * Utils::GetPercent(min,max,current) ,pos.y);
    slider->ResetModelMatrix();
    slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
    slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));

    minT->SetPosition(glm::vec2(pos.x - size.x/2 - minT->GetSize().x/2 - spacing,pos.y));
    maxT->SetPosition(glm::vec2(pos.x + size.x/2 + maxT->GetSize().x/2 + spacing,pos.y));
    currentT->SetPosition(glm::vec2(slider_position.x,pos.y + currentT->GetSize().y + spacing));
    lable->SetPosition(glm::vec2(pos.x + size.x/2 + lable->GetSize().x/2 + maxT->GetSize().x + spacing * 2,pos.y));

    this->position = pos;
    SetCollider(collider_on_background);
    // We have to update filler size after all
    CalculateCurrent();
  }
 
  void HorizontalSlider::SetText( const char* const lable){
    this->lable->ApplyText(lable);
    this->lable->SetPosition(glm::vec2(position.x + size.x/2 + this->lable->GetSize().x/2 + maxT->GetSize().x + spacing * 2,position.y));
  }  

  void HorizontalSlider::CalculateCurrent(){
    float start_value = min;
    float range = max - min;
    float relitive_pos_in_percent = (slider_position.x - position.x + size.x/2) / (float)size.x;
    const float current_value = start_value + range * relitive_pos_in_percent;
    SetCurrent(current_value);
    // Set up changing filler for slider
    glm::vec2 new_size = glm::vec2((size.x/2 * 0.95) * (1 - relitive_pos_in_percent),size.y/2 * 0.8);
    filler->ResetModelMatrix();
    filler->SetPosition(glm::vec3(slider_position.x + new_size.x,position.y,0.f));
    filler->SetScale(glm::vec3(new_size.x * 1.05,new_size.y,0.f));
  }
  void HorizontalSlider::HoveredByMouse(){
    // Moving slider only if we pressing on slider
    float leftCornerPosition = position.x - size.x/2 ;
    float rightCornerPosition = position.x + size.x/2;
    if (isHolding){
      if (slider_position.x >= leftCornerPosition  &&
          slider_position.x <= rightCornerPosition ){
        slider_position = glm::vec2(InterfacerSingleton::GetInstance()->GetMousePosition().x,position.y);
        slider->ResetModelMatrix();
        slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
        slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));

        CalculateCurrent();
        currentT->ApplyText(Utils::ToStr(current).c_str());
        currentT->SetPosition(glm::vec2(slider_position.x,position.y + currentT->GetSize().y + spacing));
      }
      // If slider will be out of range we put him in range of action of first if
      // This is exist because slider will stuck on edges
      if (slider_position.x <= leftCornerPosition){
        slider_position = glm::vec2(leftCornerPosition + 1,position.y);
        slider->ResetModelMatrix();
        slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
        slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));
      }
      if (slider_position.x >= rightCornerPosition + 1){
        slider_position = glm::vec2(rightCornerPosition ,position.y);
        slider->ResetModelMatrix();
        slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
        slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));
      }
    }
  }

  void HorizontalSlider::PressedByMouse(){
    isHolding = true;
    slider_position = glm::vec2(InterfacerSingleton::GetInstance()->GetMousePosition().x,position.y);
    slider->ResetModelMatrix();
    slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
    slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));
    CalculateCurrent();
    currentT->ApplyText(Utils::ToStr(current).c_str());
    currentT->SetPosition(glm::vec2(slider_position.x,position.y + currentT->GetSize().y + spacing));
  }


/***************************VerticalSlider***************************/

  VerticalSlider::VerticalSlider(glm::vec2 pos, float min, float max, float current) : 
    Slider(pos,min,max,current){
    size = glm::vec2(10,100);
    SetPosition(pos);
    SetSize(size);
    CalculateCurrent();
  }

  void VerticalSlider::SetSize( glm::vec2 size ){
    bigBackground->ResetModelMatrix();
    int w = (size.x + std::max(currentT->GetSize().x,lable->GetSize().x) + spacing*2)/2;
    int h = (size.y + minT->GetSize().y + maxT->GetSize().y + lable->GetSize().y + spacing * 4)/2;
    bigBackground->SetPosition(glm::vec3((position.x - size.x/2 - spacing/2) + w, (position.y - minT->GetSize().y - spacing - size.y/2) + h,0.f));
    bigBackground->SetScale(glm::vec3(w,h,0.f));

    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(position.x,position.y,0.f));
    background->SetScale(glm::vec3(size.x/2,size.y/2,0.f));

    filler->ResetModelMatrix();
    filler->SetPosition(glm::vec3(position.x,position.y,0.f));
    filler->SetScale(glm::vec3(size.x/2 * 0.8,size.y/2 * 0.96 ,0.f));

    slider_size = glm::vec2(size.x*2,size.x);
    slider->ResetModelMatrix();
    slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
    slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));

    minT->SetPosition(glm::vec2(position.x,position.y - size.y/2 - minT->GetSize().y/2 - spacing));
    maxT->SetPosition(glm::vec2(position.x,position.y + size.y/2 + maxT->GetSize().y/2 + spacing));
    currentT->SetPosition(glm::vec2(position.x + currentT->GetSize().x + spacing,slider_position.y));
    lable->SetPosition(glm::vec2(position.x,position.y + size.y/2 + lable->GetSize().y/2 + maxT->GetSize().y + spacing * 2));

    this->size = size;
    SetCollider(collider_on_background);
  }

  void VerticalSlider::SetPosition( glm::vec2 pos){
    bigBackground->ResetModelMatrix();
    int w = (size.x + std::max(maxT->GetSize().x*2,lable->GetSize().x) + spacing*2)/2;
    int h = (size.y + minT->GetSize().y + maxT->GetSize().y + lable->GetSize().y + spacing * 4)/2;
    bigBackground->SetPosition(glm::vec3((pos.x - size.x/2 - spacing/2 - std::max(currentT->GetSize().x,lable->GetSize().x)/2) + w, (pos.y - minT->GetSize().y - spacing - size.y/2) + h,0.f));
    bigBackground->SetScale(glm::vec3(w,h,0.f));

    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    background->SetScale(glm::vec3(size.x/2,size.y/2,0.f));

    filler->ResetModelMatrix();
    filler->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    filler->SetScale(glm::vec3(size.x/2 * 0.8,size.y/2 * 0.96 ,0.f));

    slider_position = glm::vec2(pos.x,pos.y - size.y/2.f + size.y * Utils::GetPercent(min,max,current));
    slider->ResetModelMatrix();
    slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
    slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));

    minT->SetPosition(glm::vec2(pos.x,pos.y - size.y/2 - minT->GetSize().y/2 - spacing));
    maxT->SetPosition(glm::vec2(pos.x,pos.y + size.y/2 + maxT->GetSize().y/2 + spacing));
    currentT->SetPosition(glm::vec2(pos.x + currentT->GetSize().x + spacing,slider_position.y));
    lable->SetPosition(glm::vec2(pos.x,pos.y + size.y/2 + lable->GetSize().y/2 + maxT->GetSize().y + spacing * 2));

    this->position = pos;
    SetCollider(collider_on_background);
    // We have to update filler size after all
    CalculateCurrent();
  }
 
  void VerticalSlider::SetText( const char* const lable){
    this->lable->ApplyText(lable);
    this->lable->SetPosition(glm::vec2(position.x,position.y + size.y/2 + this->lable->GetSize().y/2 + maxT->GetSize().y + spacing * 2));
  }  

  void VerticalSlider::CalculateCurrent(){
    float start_value = min;
    float range = max - min;
    float relitive_pos_in_percent = (slider_position.y - position.y + size.y/2) / (float)size.y;
    const float current_value = start_value + range * relitive_pos_in_percent;
    SetCurrent(current_value);
    // Set up changing filler for slider
    glm::vec2 new_size = glm::vec2(size.x/2 * 0.8, (size.y/2 * 0.95) * (1 - relitive_pos_in_percent));
    filler->ResetModelMatrix();
    filler->SetPosition(glm::vec3(position.x,slider_position.y + new_size.y,0.f));
    filler->SetScale(glm::vec3(new_size.x,new_size.y * 1.05,0.f));
  }
  void VerticalSlider::HoveredByMouse(){
    // Moving slider only if we pressing on slider
    float bottomCornerPosition = position.y - size.y/2 ;
    float topCornerPosition = position.y + size.y/2;
    if (isHolding){
      if (slider_position.y >= bottomCornerPosition  &&
          slider_position.y <= topCornerPosition ){
        slider_position = glm::vec2(position.x,InterfacerSingleton::GetInstance()->GetMousePosition().y);
        slider->ResetModelMatrix();
        slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
        slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));
        CalculateCurrent();
        currentT->ApplyText(Utils::ToStr(current).c_str());
        currentT->SetPosition(glm::vec2(position.x + currentT->GetSize().x + spacing,slider_position.y));
      }
      // If slider will be out of range we put him in range of action of first if
      // This is exist because slider will stuck on edges
      if (slider_position.y <= bottomCornerPosition){
        slider_position = glm::vec2(position.x,bottomCornerPosition + 1);
        slider->ResetModelMatrix();
        slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
        slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));
      }
      if (slider_position.y >= topCornerPosition + 1){
        slider_position = glm::vec2(position.x,topCornerPosition);
        slider->ResetModelMatrix();
        slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
        slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));
      }
    }
  }

  void VerticalSlider::PressedByMouse(){
    isHolding = true;
    slider_position = glm::vec2(position.x,InterfacerSingleton::GetInstance()->GetMousePosition().y);
    slider->ResetModelMatrix();
    slider->SetPosition(glm::vec3(slider_position.x,slider_position.y,0.f));
    slider->SetScale(glm::vec3(slider_size.x/2,slider_size.y/2,0.f));
    CalculateCurrent();
    currentT->ApplyText(Utils::ToStr(current).c_str());
    currentT->SetPosition(glm::vec2(position.x + currentT->GetSize().x + spacing,slider_position.y));
  }


}


