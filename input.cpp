#include "interfacer.hpp"

#include "DIPrimitives/primitives.hpp"
#include "DIWindow/win.hpp"
#include "input.hpp"
#include "DIPhysics/collider.hpp"
#include "DIEvents/event.hpp"
#include "DIParser/text.hpp"
#include "DIEvents/keys.hpp"


namespace DI{

  extern WinData* eg_winData;
  extern EventData* eg_eventData;

  
  InvisibleInput::InvisibleInput(glm::vec2 pos, glm::vec2 size){
    InterfacerSingleton* guiInstance = InterfacerSingleton::GetInstance();
    this->position = pos;
    this->size = size;
    isFocused   = false;
    // Set position for primitive
    collider = new ColliderSquare();
    SetPosition(pos);
    SetSize(size);
    // Set physical collider
    SetCollider(collider);
    // Set default text
    SetText("");
  }

  
  InvisibleInput::~InvisibleInput(){
    delete collider;
    callbacks.clear();
  }

  
  void InvisibleInput::Draw() {}

  
  void InvisibleInput::SetSize(glm::vec2 size){
    this->size = size;
    SetCollider(collider);
  }
  
  void InvisibleInput::SetPosition( glm::vec2 pos){
    this->position = pos;
    SetCollider(collider);
  }
 
  
  void InvisibleInput::SetText(const char* const text){
    // Some complex logic will be here after appearing Text class
    this->bufferT = text;
  }  

  
  void InvisibleInput::SetCollider( ColliderSquare* cldr){
    // Set needed properties for collider
    PhysicsHandler::SetColliderSquare(cldr, glm::vec2(position.x-size.x/2,position.y-size.y/2), size);
    this->collider = cldr;
  }
  
  
  void InvisibleInput::SetCallback( GUIState state, const std::function<void()> callback){
    // Do not need for check if some callback already exist. It will rewrite mem for new one
    callbacks.emplace(state,callback);
  }  

  void InvisibleInput::PressedByMouse(){
    isFocused = !isFocused; 
  }

  void InvisibleInput::PressedByKey(){
    if(isFocused){
      switch(eg_eventData->key){
        case Keys::ENTER:{
          isFocused = false; 
          bufferT.push_back('|');
          return;
        }
        case Keys::ESCAPE:{
          isFocused = false; 
          bufferT.pop_back();
          return;
        }
        case Keys::BACKSPACE:{
          if (!bufferT.empty())
            bufferT.erase(bufferT.end()-1); 
          return;
        }
      }
      bufferT += eg_eventData->key;
    }
  }
  
  void InvisibleInput::PressedOutSide(){
    isFocused = false;
  }
/*******************************StringInput************************************/
  StringInput::StringInput(glm::vec2 pos, glm::vec2 size, std::string lable, std::string hint) :
  InvisibleInput(pos,size){
    borders = new Reqtangle(POSITION_AND_COLORS);
    SetPrimitive(borders);
    borders->SetColor(glm::vec4(80/255.f, 91/255.f, 110/255.f,1));

    background = new Reqtangle(POSITION_AND_COLORS);
    SetPrimitive(background);
    background->SetColor(glm::vec4(eg_winData->bg.r,eg_winData->bg.g,eg_winData->bg.b,1));

    bigBackground = new Reqtangle(POSITION_AND_COLORS);
    SetPrimitive(bigBackground);
    bigBackground->SetColor(glm::vec4(0));

    bufferT = hint;
    lableT = lable;
    buffer = new Text(bufferT);
    this->lable = new Text(lableT);
    padding = 20;
    carret = new Reqtangle(POSITION_AND_COLORS);
    SetPrimitive(carret);
    carret->SetColor(glm::vec4(160/255.f, 180/255.f, 110/255.f,1));

    SetPosition(pos);
    SetSize(size);
    SetText(bufferT.c_str(),lableT.c_str());
    SetCollider(collider);

    carret = new Reqtangle(POSITION_AND_COLORS);
    SetPrimitive(carret);
    carret->SetColor(glm::vec4(160/255.f, 180/255.f, 110/255.f,1));
    carret->ResetModelMatrix();
    carret->SetPosition(glm::vec3(pos.x + size.x/2 - padding, pos.y,0.f));
    carret->SetScale(glm::vec3(size.x/bufferT.size(), size.y, 0.f));
  }
  StringInput::~StringInput(){
    delete borders;
    delete bigBackground;
    delete background;
    delete buffer;
    delete lable;
  }
  void StringInput::Draw() {
    if (isFocused)
      RendererSingleton::GetInstance()->Draw(*carret);
    RendererSingleton::GetInstance()->Draw(*background);    
    RendererSingleton::GetInstance()->Draw(*borders);    
    RendererSingleton::GetInstance()->Draw(*bigBackground);    
    RendererSingleton::GetInstance()->Draw(*buffer);
    RendererSingleton::GetInstance()->Draw(*lable);
  }
  void StringInput::SetPosition( glm::vec2 pos) {
    borders->ResetModelMatrix();
    borders->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    borders->SetScale(glm::vec3(size.x/2,size.y/2,0.f));

    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    background->SetScale(glm::vec3(size.x/2*0.95,size.y/2*0.8,0.f));

    carret->ResetModelMatrix();
    carret->SetPosition(glm::vec3(pos.x + size.x/2 - padding, pos.y,0.f));
    carret->SetScale(glm::vec3(5, size.y/2 *0.8, 0.f));

    buffer->SetPosition(glm::vec2(pos.x,pos.y));
    lable->SetPosition(glm::vec2(pos.x + size.x/2 + lable->GetSize().x/2 + padding,pos.y));

    bigBackground->ResetModelMatrix();
    bigBackground->SetPosition(glm::vec3(lable->GetPosition().x + lable->GetSize().x/2, lable->GetPosition().y + lable->GetSize().y/2,0.f));
    bigBackground->SetScale(glm::vec3(lable->GetSize().x, lable->GetSize().y, 0.f));

    this->position = pos;
    SetCollider(collider);
  }
  void StringInput::SetSize( glm::vec2 size ) {
    borders->ResetModelMatrix();
    borders->SetPosition(glm::vec3(position.x,position.y,0.f));
    borders->SetScale(glm::vec3(size.x/2,size.y/2,0.f));

    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(position.x,position.y,0.f));
    background->SetScale(glm::vec3(size.x/2 *0.95,size.y/2 * 0.8,0.f));

    carret->ResetModelMatrix();
    carret->SetPosition(glm::vec3(position.x + size.x/2 - padding, position.y,0.f));
    carret->SetScale(glm::vec3(5, size.y/2 * 0.8, 0.f));

    buffer->SetPosition(glm::vec2(position.x,position.y));
    lable->SetPosition(glm::vec2(position.x + size.x/2 + lable->GetSize().x/2 + padding,position.y));

    bigBackground->ResetModelMatrix();
    bigBackground->SetPosition(glm::vec3(lable->GetPosition().x + lable->GetSize().x/2, lable->GetPosition().y + lable->GetSize().y/2,0.f));
    bigBackground->SetScale(glm::vec3(lable->GetSize().x, lable->GetSize().y, 0.f));

    this->size = size;
    SetCollider(collider);
  }
  void StringInput::SetPrimitive( Primitive* in ){
    in->SetShader(InterfacerSingleton::GetInstance()->GetShader(POSITION_AND_COLORS));
    in->SetCamera(*InterfacerSingleton::GetInstance()->GetCamera());
    in->SetPosition(glm::vec3(position.x,position.y,0));
    in->SetScale(glm::vec3(size.x/2,size.y/2,0));
  }
  void StringInput::SetText( const char* const text, const char* const title){
    bufferT = text;
    lableT  = title;
    buffer->ApplyFont(InterfacerSingleton::GetInstance()->GetFont());
    buffer->ApplyText(text);
    buffer->SetPosition(glm::vec2(position.x,position.y));

    lable->ApplyFont(InterfacerSingleton::GetInstance()->GetFont());
    lable->ApplyText(title);
    lable->SetPosition(glm::vec2(position.x + size.x/2 + lable->GetSize().x/2 + padding,position.y));

    bigBackground->ResetModelMatrix();
    bigBackground->SetPosition(glm::vec3(lable->GetPosition().x + lable->GetSize().x/2, lable->GetPosition().y + lable->GetSize().y/2,0.f));
    bigBackground->SetScale(glm::vec3(lable->GetSize().x, lable->GetSize().y, 0.f));

    size = buffer->GetSize();
    size.x += padding; 
    size.y += padding/2.f; 
    SetPosition(position);
    SetSize(size);
  }    
  glm::vec2 StringInput::GetSize(){
    return glm::vec2(bigBackground->GetScale().x + background->GetScale().x + padding*2, bigBackground->GetScale().y + padding);
  }
  void StringInput::PressedByMouse(){ // Gain focusF
    isFocused = !isFocused; 
    SetText(bufferT.c_str(),lableT.c_str());
  }

  void StringInput::PressedByKey(){ // Gather keysF
    if(isFocused){
      switch(eg_eventData->key){
        case Keys::ENTER:{
          isFocused = false; 
          return;
        }
        case Keys::ESCAPE:{
          isFocused = false; 
          return;
        }
        case Keys::BACKSPACE:{
          if (!bufferT.empty()){
            bufferT.pop_back(); 
            SetText(bufferT.c_str(),lableT.c_str());
          }
          return;
        }
      }
      bufferT += eg_eventData->key;
      SetText(bufferT.c_str(),lableT.c_str());
    }
  }

  void StringInput::PressedOutSide(){ 
    if (isFocused){
      isFocused = false;
      SetText(bufferT.c_str(),lableT.c_str());
    }
  }

/*******************************IntInput************************************/
  IntInput::IntInput(glm::vec2 pos, glm::vec2 size, std::string lable, int data) :
  StringInput(pos,size,lable,std::to_string(data)){
    this->data.integer = data;
    this->set = "0123456789-";
  }
  IntInput::~IntInput(){

  }
  void IntInput::SetData(Data& data){
    data.integer = std::atoi(bufferT.c_str());
  }
  void IntInput::PressedByMouse(){ // Gain focusF
    isFocused = !isFocused; 
    SetText(bufferT.c_str(),lableT.c_str());
  }

  void IntInput::PressedByKey(){ // Gather keysF
    if(isFocused){
      switch(eg_eventData->key){
        case Keys::ENTER:{
          isFocused = false; 
          SetData(data);
          return;
        }
        case Keys::ESCAPE:{
          isFocused = false; 
          SetData(data);
          return;
        }
        case Keys::BACKSPACE:{
          if (!bufferT.empty()){
            bufferT.pop_back(); 
            SetText(bufferT.c_str(),lableT.c_str());
          }
          return;
        }
      }
      if (set.find(eg_eventData->key) != set.npos){
        bufferT += eg_eventData->key;
        SetText(bufferT.c_str(),lableT.c_str());
      }
    }
  }

  void IntInput::PressedOutSide(){ 
    if (isFocused){
      isFocused = false;
      SetData(data);
      SetText(bufferT.c_str(),lableT.c_str());
    }
  }
/*******************************FloatInput************************************/
  FloatInput::FloatInput(glm::vec2 pos, glm::vec2 size, std::string lable, float data) :
  IntInput(pos,size,lable,data){
    this->data.floating_point = data;
    this->set = "0123456789-.,";
  }
  FloatInput::~FloatInput(){

  }
  void FloatInput::SetData(Data& data){
    data.floating_point = std::atof(bufferT.c_str());
  }
/*******************************CharInput************************************/
  CharInput::CharInput(glm::vec2 pos, glm::vec2 size, std::string lable) :
  StringInput(pos,size,lable,"A"){
    this->data = 'A';
    this->set = "qwertyuiopsdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
  }
  CharInput::~CharInput(){

  }
  void CharInput::PressedByMouse(){ // Gain focusF
    isFocused = !isFocused; 
    SetText(bufferT.c_str(),lableT.c_str());
  }

  void CharInput::PressedByKey(){ // Gather keysF
    if(isFocused){
      switch(eg_eventData->key){
        case Keys::ENTER:{
          isFocused = false; 
          data = bufferT[0];
          return;
        }
        case Keys::ESCAPE:{
          isFocused = false; 
          data = bufferT[0];
          return;
        }
      }
      if (set.find(eg_eventData->key) != set.npos){
        bufferT[0] = eg_eventData->key;
        SetText(bufferT.c_str(),lableT.c_str());
      }
    }
  }

  void CharInput::PressedOutSide(){ 
    if (isFocused){
      isFocused = false;
      data = bufferT[0];
      SetText(bufferT.c_str(),lableT.c_str());
    }
  }
} 
