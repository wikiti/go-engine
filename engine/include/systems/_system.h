#ifndef __SYSTEM_H_
#define __SYSTEM_H_

// enabled? de momento para nada

class CSystem
{
  protected:
    bool enabled;

  public:
    CSystem(): enabled(false){}
    virtual ~CSystem(){};

    virtual bool Init(){ enabled = true; return true;}
    virtual void Close(){ enabled = false;}

    virtual void OnRender(){};
    virtual void OnLoop(){};
    virtual void OnEvent(){};
    virtual void OnKeyEvent(){};

    void Enable()
    {
      enabled = true;
    }
    void Disable()
    {
      enabled = false;
    }
    void SetState(bool state)
    {
      enabled = state;
    }
};

#endif /* __SYSTEM_H_ */
