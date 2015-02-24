#include "stdafx.h"

#include "Button.h"

Button::Button(sf::Vector2f pos, sf::Font font,sf::Color col)
{
	int width = 160;
	int height = 60;

	setPosition(pos);

	rect = sf::RectangleShape(sf::Vector2f(width, height));

	rect.setPosition(getPosition());

	rect.setFillColor(col);
	rect.setOutlineColor(sf::Color(171,130,255, 255));
	rect.setOutlineThickness(1.5f);
	selected = false;
}

Button::~Button()
{
	//blank destructor
}

//checks if the mouse is within the buttons bounds(i.e hovering over it) and changes the alpha of it
void Button::Highlight(sf::Vector2i mousePos)
{
	if (rect.getGlobalBounds().contains(mousePos.x, mousePos.y))
	{
		rect.setFillColor(sf::Color(rect.getFillColor().r, rect.getFillColor().g, rect.getFillColor().b, 127));
	}
	else 		rect.setFillColor(sf::Color(rect.getFillColor().r, rect.getFillColor().g, rect.getFillColor().b, 255));
}

//checks if the mouse is within the bounds of the button and the result is used to determine if the button was clicked
bool Button::Clicked(sf::Vector2i mousePos)
{
	if (rect.getGlobalBounds().contains(mousePos.x, mousePos.y))
	{
		selected = true;
		return true;
	}
	else
	{
		return false;
	}
}

//allows the colour to be changed
void Button::changeColour(sf::Color col)
{
	colour = col;
}

void Button::draw(sf::RenderTarget& window, sf::RenderStates state) const
{

}

void Button::draw(sf::RenderTarget& window)
{
	window.draw(rect);
}

