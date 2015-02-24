#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>
#include "stdafx.h"

// Forward references
template <typename NodeType, typename ArcType> class GraphArc;

// -------------------------------------------------------
// Name:        GraphNode
// Description: This is the node class. The node class 
//              contains data, and has a linked list of 
//              arcs.
// -------------------------------------------------------
template<class NodeType, class ArcType>
class GraphNode {
private:    
// typedef the classes to make our lives easier.
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;
// -------------------------------------------------------
// Description: data inside the node
// -------------------------------------------------------
    NodeType m_data;

// -------------------------------------------------------
// Description: list of arcs that the node has.
// -------------------------------------------------------
    list<Arc> m_arcList;

// -------------------------------------------------------
// Description: This remembers if the node is marked.
// -------------------------------------------------------
    bool m_marked;

	//previous node
	Node* prevNode;

	sf::CircleShape shape;//circle to represent the node
	//load a font
	sf::Font font;

	sf::Text name;//text to draw the name

	sf::Text costSoFar;//text to draw the costSoFar

	sf::Text heuristic;//text to draw the heuristic

	bool selected;

	bool visited;//whether the node has been visited or not

	bool inPath;//whether the node is in the path or not

	bool showHeuristic;

public:

	//constructor
	GraphNode(Node* previous = 0)
	{
		setPrevNode(previous);

		//set up the circle to represent the node
		shape.setRadius(25);
		shape.setOutlineThickness(3);
		shape.setFillColor(sf::Color(0,191,255,255));
		shape.setOutlineColor(sf::Color(16,78,139, 255));

		//set up the text to draw the name, cost and heurstic
		if (font.loadFromFile("assets/JingJing.TTF"));// { std::cout << "Font loaded for node" << std::endl; }
		name.setFont(font);
		name.setStyle(sf::Text::Italic | sf::Text::Bold);
		name.setCharacterSize(18);
		name.setColor(sf::Color(0, 0, 0, 255));
		
		costSoFar.setFont(font);
		costSoFar.setStyle(sf::Text::Italic | sf::Text::Bold);
		costSoFar.setCharacterSize(14);
		costSoFar.setColor(sf::Color(255, 0, 0, 0));

		heuristic.setFont(font);
		heuristic.setStyle(sf::Text::Italic | sf::Text::Bold);
		heuristic.setCharacterSize(14);
		heuristic.setColor(sf::Color(255, 0, 0, 0));

		selected = false;
		visited = false;
		inPath = false;
		showHeuristic = false;
	}

    // Accessor functions
    list<Arc> & arcList()  {
        return m_arcList;              
    }

    bool marked() const {
        return m_marked;
    }

    NodeType & data() {
        return m_data;
    }

	bool getSelected(){ return selected; }//see if it is selected

	bool getVisited(){ return visited; }//see if it was visited

	bool getInPath(){ return inPath; }//see if it is in the path

	bool getShowHeuristic(){ retrun showHeuristic; }

    // Manipulator functions
    void setData(NodeType data) {
        m_data = data;
    }
    
    void setMarked(bool mark) {
        m_marked = mark;
    }

	void setSelected(bool sel){ selected = sel; }//set selected

	void setInPath(bool inP){ inPath = inP; }//set if in path
           
    Arc* getArc( Node* pNode );    
    void addArc( Node* pNode, ArcType pWeight );
    void removeArc( Node* pNode );

	Node* getPrevNode(){return prevNode;}
	void setPrevNode(Node* p){prevNode = p;}

	void setVisited(bool v){ visited = v; }//set if it was visited

	void setShowHeuristic(bool sh){ showHeuristic = sh; }

	sf::CircleShape getShape(){ return shape; }//get the circle shape
	sf::Text getName(){ return name; }

	//methods for setting the text and positions of text and circle
	void setPosOfCircle(sf::Vector2f pos);
	void setNameProps(sf::String n, sf::Vector2f pos);
	void setCostProps(std::string c, sf::Vector2f pos);
	void setHeuristicProps(std::string h, sf::Vector2f pos);

	//if the node(circle) contains the mouse then highlight it
	void highlight(sf::Vector2i mousePos, bool click);

	//reset the colours when we want to reset
	void resetColour();

	void draw(sf::RenderTarget& window);
};

// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    This finds the arc in the current node that
//                  points to the node in the parameter.
//  Arguments:      The node that the arc connects to.
//  Return Value:   A pointer to the arc, or 0 if an arc doesn't
//                  exist from this to the specified input node.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
GraphArc<NodeType, ArcType>* GraphNode<NodeType, ArcType>::getArc( Node* pNode ) {

     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();
     Arc* pArc = 0;
     
     // find the arc that matches the node
     for( ; iter != endIter && pArc == 0; ++iter ) {         
          if ( (*iter).node() == pNode) {
               pArc = &( (*iter) );
          }
     }

     // returns null if not found
     return pArc;
}


// ----------------------------------------------------------------
//  Name:           addArc
//  Description:    This adds an arc from the current node pointing
//                  to the first parameter, with the second parameter 
//                  as the weight.
//  Arguments:      First argument is the node to connect the arc to.
//                  Second argument is the weight of the arc.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::addArc( Node* pNode, ArcType weight ) {
   // Create a new arc.
   Arc a;
   a.setNode(pNode);
   a.setWeight(weight);   
   // Add it to the arc list.
   m_arcList.push_back( a );
}


// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This finds an arc from this node to input node 
//                  and removes it.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::removeArc( Node* pNode ) {
     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();

     int size = m_arcList.size();
     // find the arc that matches the node
     for( ; iter != endIter && m_arcList.size() == size;  
                                                    ++iter ) {
          if ( (*iter).node() == pNode) {
             m_arcList.remove( (*iter) );
          }                           
     }
}

//methods for setting the text and positions of text and circle
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::setPosOfCircle(sf::Vector2f pos)
{
	shape.setPosition(pos);
	shape.setPointCount(100);
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::setNameProps(sf::String n, sf::Vector2f pos)
{
	name.setString(n);
	name.setPosition(pos);
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::setCostProps(std::string c, sf::Vector2f pos)
{
	costSoFar.setString(c);
	costSoFar.setPosition(sf::Vector2f(pos.x, pos.y - 20));
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::setHeuristicProps(std::string h, sf::Vector2f pos)
{
	heuristic.setString(h);
	heuristic.setPosition(sf::Vector2f(pos.x,pos.y+50));
}

//if the node(circle) contains the mouse then highlight it
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::highlight(sf::Vector2i mousePos, bool click)
{
	sf::Vector2f mouseF = sf::Vector2f(mousePos.x, mousePos.y);
	if (shape.getGlobalBounds().contains(mouseF) && click == false)
	{
		shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, 127));
	}
	else if (shape.getGlobalBounds().contains(mouseF) && click == true)
	{
		selected = true;
	}
	else shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, 255));


}

//reset the colours when we want to reset
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::resetColour()
{
	shape.setFillColor(sf::Color(0, 191, 255, 255));
	costSoFar.setColor(sf::Color(255, 0, 0, 0));
	heuristic.setColor(sf::Color(255, 0, 0, 0));
}


template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::draw(sf::RenderTarget& window)
{
	window.draw(shape);
	window.draw(name);
	//toggle these when path is being found
	window.draw(costSoFar);
	window.draw(heuristic);

	//if the node has been marked
	if (visited == true && selected == false)
	{
		shape.setFillColor(sf::Color::Yellow);
		costSoFar.setColor(sf::Color(255, 0, 0, 255));
		//heuristic.setColor(sf::Color(255, 0, 0, 255));
	}
	if (showHeuristic == true)
		heuristic.setColor(sf::Color(255, 0, 0, 255));
	//if the node has been selected
	if (selected == true)
	{
		shape.setFillColor(sf::Color::Green);
	}
	//if it is in the path and not the start or destination node
	if (inPath == true && selected == false)
	{
		shape.setFillColor(sf::Color::White);
	}
}

#include "GraphArc.h"

#endif
