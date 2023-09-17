#ifndef ADDON_H
#define ADDON_H

class Addon {
public:
    Addon();  // Constructor
    ~Addon(); // Destructor

    void Render(); // Addon render function

private:
    void RenderEvents();
    void Update();
};


#endif // ADDON_H