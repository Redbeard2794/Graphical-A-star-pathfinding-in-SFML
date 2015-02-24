////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")


#include <fstream>

#include <utility>
#include <string>
#include <map>
using namespace std;

typedef GraphArc<aStarInfoStorage, int> Arc1;//**If I leave this as 'Arc' it gives an error saying it is a redefinition**
typedef GraphNode<aStarInfoStorage, int> Node;
////////////////////////////////////////////////////////////
///Entrypoint of application
////////////////////////////////////////////////////////////

void visitAstar(Node * pNode) {
	cout << "A* is visiting a node" << endl;
	pNode->setVisited(true);
}


int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(1000, 600, 32), "Graphical A* Pathfinding");
	std::cout << "** Info output here is debug information **" << std::endl << ::endl;
	Graph<aStarInfoStorage, int> graph(30);

	//used to position the nodes
	int prevX = 100;
	int prevY = 100;

	int xCount = 1;
	int yCount = 1;

	string c;
	int i = 0;
	ifstream myfile;
	myfile.open("Nodes.txt");

	while (myfile >> c) {
		sf::Vector2f p = sf::Vector2f(prevX*xCount, prevY*yCount);
		graph.addNode(aStarInfoStorage(c, FLT_MAX, FLT_MAX, p), i++);
		if (xCount < 6)
		{
			xCount += 1;
		}
		else {
			xCount = 1;
			yCount += 1;
		}
	}

	myfile.close();
	myfile.open("newArcs.txt");

	int from, to;
	int weight;
	int ifrom, ito;
	int arc1X, arc1Y;//positions of the arcs form arcs file
	int arc2X, arc2Y;//positions of the arcs form arcs file


	while (myfile >> from >> to >> weight >> arc1X >> arc1Y >> arc2X >> arc2Y) {
		ifrom = from;
		ito = to;
		graph.addArc(ifrom, ito, weight, arc1X, arc1Y, arc2X, arc2Y);
	}
	myfile.close();
	std::cout << "Graph created" << std::endl;


	//load a font
	sf::Font font;
	if (font.loadFromFile("assets/JingJing.TTF")) { std::cout << "Font loaded" << std::endl; }

	//create text to draw on the button
	sf::Text text;
	text.setFont(font);
	text.setString("Compute path");
	text.setStyle(sf::Text::Italic | sf::Text::Bold);
	text.setPosition(810, 40);
	text.setCharacterSize(15);
	text.setColor(sf::Color::Black);
	//create text to draw on the button
	sf::Text text2;
	text2.setFont(font);
	text2.setString("Reset nodes");
	text2.setStyle(sf::Text::Italic | sf::Text::Bold);
	text2.setPosition(815, 140);
	text2.setCharacterSize(15);
	text2.setColor(sf::Color::Black);
	//create text to draw the instructions
	sf::Text instructions1;
	instructions1.setFont(font);
	instructions1.setString("* Select 2 nodes.\n* They will be coloured \ngreen when you have \nselected them. \n* Yellow means the \nnode was visited. \n* White nodes \nindicate the \npath between the two \nselected nodes");
	instructions1.setStyle(sf::Text::Italic | sf::Text::Bold);
	instructions1.setPosition(700, 250);
	instructions1.setCharacterSize(15);
	instructions1.setColor(sf::Color::White);

	//create the buttons
	Button* butt1 = new Button(sf::Vector2f(800, 20), font, sf::Color(0,255,0,255));
	Button* butt2 = new Button(sf::Vector2f(800, 120), font, sf::Color(205, 0, 0, 255));

	//for controlling the mouse click
	bool clickOccurred = false;
	int clickTime = 0;
	bool canClick = true;
	int selectionCount = 0;

	// Start game loop
	while (window.isOpen())
	{
		// Process events
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit
			if (Event.type == sf::Event::Closed)
				window.close();
			// Escape key : exit
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();
		}
		//prepare frame
		window.clear();
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && canClick == true)
		{
			sf::Vector2i localPosition = sf::Mouse::getPosition(window); // window is a sf::Window
			if (butt1->Clicked(localPosition) == true)
			{
					std::cout << "Compute path" << std::endl;
					canClick = false;
					clickOccurred = true;
					vector<Node*> bestPath;
					if (selectionCount == 2)//if 2 nodes are selected then call A*
					{
						graph.findSelectedIndices();
						graph.aStar(graph.nodeArray()[graph.selectedNodeIndices.at(0)], graph.nodeArray()[graph.selectedNodeIndices.at(1)], visitAstar, bestPath);//9,28
						cout << endl;
						cout << "Here is the path: " << endl;
						graph.outputPath(bestPath);
						cout << endl;
					}
					else cout << "Please select 2 nodes" << endl;
			}
			else if (butt2->Clicked(localPosition) == true && canClick == true)
			{
				//reset the graph
				std::cout << "Reset nodes" << std::endl;
				canClick = false;
				clickOccurred = true;
				graph.clearMarks();
				selectionCount = 0;
				graph.selectedNodeIndices.clear();
			}
			if (selectionCount < 2 && canClick == true)
			{
				graph.highlightNodes(localPosition, true);
				canClick = false;
				clickOccurred = true;
				selectionCount += 1;
			}
		}
		if (clickOccurred == true)
		{
			if (clickTime < 61)
				clickTime += 1;
			else
			{
				clickOccurred = false;
				canClick = true;
				clickTime = 0;
			}

		}
		//draw frame items
		butt1->draw(window);
		butt2->draw(window);
		graph.draw(window);
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);
		graph.highlightNodes(localPosition, false);
		butt1->Highlight(localPosition);
		butt2->Highlight(localPosition);
		window.draw(text);
		window.draw(text2);
		window.draw(instructions1);
		// Finally, display rendered frame on screen
		window.display();
	} //loop back for next frame
	return EXIT_SUCCESS;
}
