#include "stdafx.h"
#ifndef BUTTON_H
#define BUTTON_H

class Button : public sf::Drawable, public sf::Transformable
{
private:

	sf::RectangleShape rect;
	sf::Color colour;
	bool selected;
public:
	sf::Text displayText;
	Button(sf::Vector2f pos, sf::Font font, sf::Color col);
	~Button();
	void Highlight(sf::Vector2i mousePos);	//if mouse is hovering over it
	bool Clicked(sf::Vector2i mousePos);//if it is clicked
	void changeColour(sf::Color col);//change colour of button
	void draw(sf::RenderTarget& window, sf::RenderStates state) const;
	void draw(sf::RenderTarget& window);

#pragma region Gets
	bool getSelected(){ return selected; }
	sf::Text getText(){ return displayText; }
	sf::RectangleShape getRect(){ return rect; }
#pragma endregion

#pragma region Sets
	void setSelected(bool s){ selected = s; }
	void setColour(sf::Color col){ colour = col; }

#pragma endregion


};

#endif