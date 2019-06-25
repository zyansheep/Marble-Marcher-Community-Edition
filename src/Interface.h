#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <map>
#include <algorithm>
#include <iterator>

struct ColorFloat
{
	float r, g, b, a;
	ColorFloat(float red = 0.f, float green = 0.f, float blue = 0.f, float alpha = 255.f);

	void operator=(sf::Color a);
};

ColorFloat operator+(ColorFloat a, ColorFloat b);

ColorFloat operator-(ColorFloat a, ColorFloat b);

ColorFloat operator*(ColorFloat a, float b);

sf::Color ToColor(ColorFloat a);
ColorFloat ToColorF(sf::Color a);


struct InputState
{
	bool keys[sf::Keyboard::KeyCount];
	bool mouse[3];
	sf::Vector2f mouse_pos;
	sf::Vector2f mouse_speed;
	float time, dt;

	InputState();
	InputState(bool keys[sf::Keyboard::KeyCount], bool mouse[3], sf::Vector2f mouse_pos, sf::Vector2f mouse_speed);
};


//the object parameters
struct State
{
	sf::Vector2f position = sf::Vector2f(0,0);
	sf::Vector2f size = sf::Vector2f(0.1f, 0.1f);
	float border_thickness = 0.f;
	float margin = 0.f;
	float font_size = 1.f;
	float scroll = 0.f;
	ColorFloat color_main = ToColorF(sf::Color::Black);
	ColorFloat color_second = ToColorF(sf::Color::Transparent);
	ColorFloat color_border = ColorFloat(128,128,128);
};

//interpolate between states for animation effects
State interpolate(State a, State b, float t);

void UpdateAllObjects(sf::RenderWindow * window, sf::Vector2i mouse, bool RMB, bool LMB, bool all_keys[], float dt);

//the object base class
class Object
{
public:
	enum States
	{
		DEFAULT, ONHOVER, ACTIVE
	};

	void SetPosition(float x, float y);
	void SetSize(float x, float y);
	void SetBackgroundColor(sf::Color color);
	void SetBorderColor(sf::Color color);
	void SetBorderWidth(float S);
	void SetMargin(float x);
	void SetScroll(float x);

	void SetCallbackFunction(void(*fun)(InputState & state));
	void SetHoverFunction(void(*fun)(InputState & state));

	void clone_states();

	virtual void KeyboardAction(InputState & state);
	virtual void Draw(sf::RenderWindow * window);

	void Update(sf::RenderWindow * window, InputState& state);

	Object();
	~Object();

	State curstate;
	State activestate;
	State hoverstate;
	State defaultstate;
	States curmode;

	sf::View used_view;

	int id;

	void(*callback)(InputState & state);
	void(*hoverfn)(InputState & state);

};

//a box to add stuff in
class Box: public Object
{
public:
	enum Allign
	{
		LEFT, CENTER, RIGHT
	};
	void AddObject(Object* something, Allign a = LEFT);

	void Draw(sf::RenderWindow *window);

	Box(float x, float y, float dx, float dy, sf::Color color_main);
	Box();

private:
	sf::RectangleShape rect;
	sf::RectangleShape scroll1, scroll2;
	sf::View boxView;

	//objects inside the box
	std::map<int, Object*> objects;
	std::map<int, Allign> object_alligns;
	std::vector<int> object_ids;
	
};


class Text: public Object
{
public:
	void Draw(sf::RenderWindow *window);
	sf::Text text;

	Text(std::string text, sf::Font &f, float size, sf::Color col);
	Text(sf::Text t);
};

class Window: public Box
{
public:
	std::unique_ptr<Box> Bar, Inside, Scroll, Scroll_Slide;
	std::unique_ptr<Text> Title;

	sf::Vector2f prev_mouse;

	Window(float x, float y, float dx, float dy, sf::Color color_main, sf::Text title);
};

class Menu : public Box
{

};



/*

class Image : public Object
{
	Image();


	void Draw(sf::RenderWindow *window);
};

*/