#include "testPatterns.hpp"
#include "../LEDTableEngine/baseController.hpp"

TestPatternApp::TestPatternApp(){

}
TestPatternApp::~TestPatternApp (){

}

void TestPatternApp::initialize(BaseController * ctrl){
  BaseApplication::initialize(ctrl);
  m_palettes[0] = {
    0,0,0,
    255,0,0,
    255,90,0,
    255,154,0,
    255,206,0,
    255,232,8
  };
  for(int i = m_palettes[0].size(); i <= 255; i++){
    m_palettes[0].push_back(0);
    m_palettes[0].push_back(0);
    m_palettes[0].push_back(0);
  }
  for(int i = 0; i <= 255; i++){
    float s = i/127.0;
    if (i > 127){
      s = (i-127)/127.0;
      m_palettes[1].push_back(255*(1 - s));
      m_palettes[1].push_back(255*s);
      m_palettes[1].push_back(0);
    }
    else{
      m_palettes[1].push_back(0);
      m_palettes[1].push_back(255*(1 - s));
      m_palettes[1].push_back(255*s);
    }
    m_palettes[2].push_back(i);
    m_palettes[2].push_back(i);
    m_palettes[2].push_back(i);
  }

  m_palettes[3] = {
    0,0,0,
    255,0,0,
    255,255,0,
  };
  for(int i = m_palettes[3].size(); i <= 255; i++){
    m_palettes[3].push_back(0);
    m_palettes[3].push_back(0);
    m_palettes[3].push_back(0);
  }

  m_generator = std::default_random_engine(m_ctrl->getTimeMs());
  m_colDist = std::uniform_int_distribution<int>(0,5);
  m_posDist = std::uniform_int_distribution<int>(0,m_ctrl->getSize()-1);
  m_patternType = RANDOM;
  m_colorPalette = m_palettes[RANDOM];
  m_interpolate = 0;
  m_font.loadFromFile("font/myfont.fnt");
}

void TestPatternApp::continueApp(){
  m_ctrl->clearFrame({0});
}
void TestPatternApp::processInput(const BaseInput::InputEvents &events,
                          const BaseInput::InputEvents &eventsDebounced,
                          TimeUnit deltaTime){
    bool modeChanged = false;
    if(BaseInput::isPressed(eventsDebounced,BaseInput::InputEventName::ENTER)){
      m_patternType = (PatternType)((int)m_patternType + 1);
      if(m_patternType == END)
        m_patternType = (PatternType)0;
      modeChanged = true;
    }
    if(BaseInput::isPressed(eventsDebounced,BaseInput::InputEventName::EXIT)){
      m_hasFinished = true;
      return;
    }
    if(modeChanged)
      m_colorPalette = m_palettes[m_patternType];
    switch (m_patternType) {
      case RANDOM:
      break;
      case COLORFADE:
      case COLORFADE_BW:
          m_interpolate+=deltaTime/5000.0;
          if(m_interpolate > 1)
            m_interpolate = 0;
      break;
    }
}
void TestPatternApp::draw(Image &frame){
  switch (m_patternType) {
    case RANDOM:{
      int pos = m_posDist(m_generator);
      int col = m_colDist(m_generator);
      frame.data[pos] = col;
      pos = m_posDist(m_generator);
      frame.data[pos] = 0;
    }
    break;
    case COLORFADE:
    case COLORFADE_BW:
      std::fill(frame.data, frame.data+frame.size, m_interpolate*255);
    break;
    case FONT_TEST:{
      std::string txt = "B";
      m_font.draw(frame,txt,{2});
    }

    break;
  }
}