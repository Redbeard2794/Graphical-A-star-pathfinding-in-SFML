#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>
//#include"aStarInfoStorage.h"
#include <utility>
#include <string>
#include <map>
#include "stdafx.h"


using namespace std;

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

template <class NodeType, class ArcType>
class NodeCostComparer {
public:
	bool operator()(GraphNode<NodeType, ArcType> * n1, GraphNode<NodeType, ArcType> * n2) {
		aStarInfoStorage p1 = n1->data();
		aStarInfoStorage p2 = n2->data();
		return p1.getCostSoFar()+p1.getHeuristicValue() > p2.getCostSoFar()+p2.getHeuristicValue();
	}

};


// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    This is the graph class, it contains all the
//                  nodes.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph {
private:

	// typedef the classes to make our lives easier.
	typedef GraphArc<NodeType, ArcType> Arc;
	typedef GraphNode<NodeType, ArcType> Node;

	// ----------------------------------------------------------------
	//  Description:    An array of all the nodes in the graph.
	// ----------------------------------------------------------------
	Node** m_pNodes;

	// ----------------------------------------------------------------
	//  Description:    The maximum number of nodes in the graph.
	// ----------------------------------------------------------------
	int m_maxNodes;


	// ----------------------------------------------------------------
	//  Description:    The actual number of nodes in the graph.
	// ----------------------------------------------------------------
	int m_count;
	//the two points for the arcs
	sf::Vector2f arc1Pos;
	sf::Vector2f arc2Pos;
	std::vector<sf::Vector2f>arcPositions;//stores the positions of each arc
	std::vector<sf::Text>arcWeights;//stores the sf::Text used to draw the arc weights
	sf::Text t;//used to draw the name of each node
	sf::Font font;

public:
	// Constructor and destructor functions
	Graph(int size);
	~Graph();
	map<string, int> pathCosts;
	string key;
	int value;
	std::vector<int>selectedNodeIndices;
	// Accessors
	Node** nodeArray() const {
		return m_pNodes;
	}

	// Public member functions.
	bool addNode(NodeType data, int index);
	void removeNode(int index);
	bool addArc(int from, int to, ArcType weight, int arcX, int arcY, int arc2X, int arc2Y);
	void removeArc(int from, int to);
	Arc* getArc(int from, int to);
	void clearMarks();
#pragma region declarations for old pathfinding stuff from labs and part 1 of this project
	void depthFirst(Node* pNode, void(*pProcess)(Node*));
	void breadthFirst(Node* pNode, void(*pProcess)(Node*));

	//adapted breadthfirst
	void adaptedBreadthFirst(Node* pNode, void(*pProcess)(Node*), Node* target);
	void UCS(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node *>& path);

	void generateHeuristics(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node *>& path);
#pragma endregion


	void aStar(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node*>& path);

	void outputPath(std::vector<Node*>& path);

	int getCount(){ return m_count; }

	void highlightNodes(sf::Vector2i mousePos, bool click);

	void findSelectedIndices();

	void draw(sf::RenderTarget& window);

};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph(int size) : m_maxNodes(size) {
	int i;
	m_pNodes = new Node *[m_maxNodes];
	// go through every index and clear it to null (0)
	for (i = 0; i < m_maxNodes; i++) {
		m_pNodes[i] = 0;
	}

	// set the node count to 0.
	m_count = 0;
	//draw the name of each node
	if (font.loadFromFile("assets/JingJing.TTF"));
	t.setFont(font);
	t.setStyle(sf::Text::Italic | sf::Text::Bold);
	t.setCharacterSize(10);
	t.setColor(sf::Color::Magenta);
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
	int index;
	for (index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0) {
			delete m_pNodes[index];
		}
	}
	// Delete the actual array
	delete m_pNodes;
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode(NodeType data, int index) {
	bool nodeNotPresent = false;
	// find out if a node does not exist at that index.
	if (m_pNodes[index] == 0) {
		nodeNotPresent = true;
		// create a new node, put the data in it, and unmark it.
		m_pNodes[index] = new Node;
		m_pNodes[index]->setData(data);
		m_pNodes[index]->setMarked(false);
		sf::Vector2f p = data.getPosition();
		sf::String name = data.getNodeName();

		std::string cost = std::to_string(data.getCostSoFar());
		std::string heuristic = std::to_string(data.getHeuristicValue());

		m_pNodes[index]->setCostProps(cost, p);
		m_pNodes[index]->setHeuristicProps(heuristic, p);

		m_pNodes[index]->setPosOfCircle(p);
		m_pNodes[index]->setNameProps(name, sf::Vector2f(p.x+20,p.y+10));

		// increase the count and return success.
		m_count++;
	}

	return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode(int index) {
	// Only proceed if node does exist.
	if (m_pNodes[index] != 0) {
		// now find every arc that points to the node that
		// is being removed and remove it.
		int node;
		Arc* arc;

		// loop through every node
		for (node = 0; node < m_maxNodes; node++) {
			// if the node is valid...
			if (m_pNodes[node] != 0) {
				// see if the node has an arc pointing to the current node.
				arc = m_pNodes[node]->getArc(m_pNodes[index]);
			}
			// if it has an arc pointing to the current node, then
			// remove the arc.
			if (arc != 0) {
				removeArc(node, index);
			}
		}


		// now that every arc pointing to the current node has been removed,
		// the node can be deleted.
		delete m_pNodes[index];
		m_pNodes[index] = 0;
		m_count--;
	}
}

// ----------------------------------------------------------------
//  Name:           addArc
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc(int from, int to, ArcType weight, int arc1X, int arc1Y, int arc2X, int arc2Y) {
	bool proceed = true;
	// make sure both nodes exist.
	if (m_pNodes[from] == 0 || m_pNodes[to] == 0) {
		proceed = false;
	}

	// if an arc already exists we should not proceed
	if (m_pNodes[from]->getArc(m_pNodes[to]) != 0) {
		proceed = false;
	}

	if (proceed == true) {
		// add the arc to the "from" node.
		m_pNodes[from]->addArc(m_pNodes[to], weight);
		//set arc positions
		arc1Pos = sf::Vector2f(arc1X+30, arc1Y+30);
		arc2Pos = sf::Vector2f(arc2X+30, arc2Y+30);
		//store the arc positions so that we can draw them later
		arcPositions.push_back(arc1Pos);
		arcPositions.push_back(arc2Pos);


		//midpoint of the 2 ends of the arc. used to draw the weights for each arc
		sf::Vector2f midPoint = sf::Vector2f(((arc1Pos.x + arc2Pos.x) / 2), ((arc1Pos.y + arc2Pos.y) / 2));

		//had to modify the midpoint value a little bit to make it look right
		t.setPosition(sf::Vector2f(midPoint.x-10,midPoint.y-15));
		std::string w = std::to_string(weight);
		t.setString(w);

		//list of sf::Text. Each is used to store the text that is drawing the value
		arcWeights.push_back(t);
	}

	return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc(int from, int to) {
	// Make sure that the node exists before trying to remove
	// an arc from it.
	bool nodeExists = true;
	if (m_pNodes[from] == 0 || m_pNodes[to] == 0) {
		nodeExists = false;
	}

	if (nodeExists == true) {
		// remove the arc.
		m_pNodes[from]->removeArc(m_pNodes[to]);
	}
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
// Dev-CPP doesn't like Arc* as the (typedef'd) return type?
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc(int from, int to) {
	Arc* pArc = 0;
	// make sure the to and from nodes exist
	if (m_pNodes[from] != 0 && m_pNodes[to] != 0) {
		pArc = m_pNodes[from]->getArc(m_pNodes[to]);
	}

	return pArc;
}

// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node. It also resets the entire graph
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks() {
	int index;
	for (index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0) {
			m_pNodes[index]->setMarked(false);//marked = false
			m_pNodes[index]->setSelected(false);//selected = false
			m_pNodes[index]->setInPath(false);//inPath = false
			m_pNodes[index]->setVisited(false);//visited = false
			m_pNodes[index]->resetColour();//reset the colours of the nodes back to their default colour
		}
	}
}


#pragma region old pathfinding stuff from labs and also from part 1 of this project
// ----------------------------------------------------------------
//  Name:           depthFirst
//  Description:    Performs a depth-first traversal on the specified 
//                  node.
//  Arguments:      The first argument is the starting node
//                  The second argument is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::depthFirst(Node* pNode, void(*pProcess)(Node*)) {
	if (pNode != 0) {
		// process the current node and mark it
		pProcess(pNode);
		pNode->setMarked(true);

		// go through each connecting node
		list<Arc>::iterator iter = pNode->arcList().begin();
		list<Arc>::iterator endIter = pNode->arcList().end();

		for (; iter != endIter; ++iter) {
			// process the linked node if it isn't already marked.
			if ((*iter).node()->marked() == false) {
				depthFirst((*iter).node(), pProcess);
			}
		}
	}
}


// ----------------------------------------------------------------
//  Name:           breadthFirst
//  Description:    Performs a depth-first traversal the starting node
//                  specified as an input parameter.
//  Arguments:      The first parameter is the starting node
//                  The second parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirst(Node* pNode, void(*pProcess)(Node*)) {
	if (pNode != 0) {
		queue<Node*> nodeQueue;
		// place the first node on the queue, and mark it.
		nodeQueue.push(pNode);
		pNode->setMarked(true);

		// loop through the queue while there are nodes in it.
		while (nodeQueue.size() != 0) {
			// process the node at the front of the queue.
			pProcess(nodeQueue.front());

			// add all of the child nodes that have not been 
			// marked into the queue
			list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
			list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();

			for (; iter != endIter; iter++) {
				if ((*iter).node()->marked() == false) {
					// mark the node and add it to the queue.
					(*iter).node()->setMarked(true);
					nodeQueue.push((*iter).node());
				}
			}

			// dequeue the current node.
			nodeQueue.pop();
		}
	}
}

//Adapted breadth first
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::adaptedBreadthFirst(Node* pNode, void(*pProcess)(Node*), Node* target) {

	bool goalReached = false;

	if (pNode != 0) {
		queue<Node*> nodeQueue;
		// place the first node on the queue, and mark it.
		nodeQueue.push(pNode);
		pNode->setMarked(true);

		// loop through the queue while there are nodes in it and target not reached
		while (nodeQueue.size() != 0 && goalReached == false) {
			// process the node at the front of the queue.
			pProcess(nodeQueue.front());

			// add all of the child nodes that have not been 
			// marked into the queue
			list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
			list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();

			for (; iter != endIter; iter++) {
				if ((*iter).node() == target)
				{
					goalReached = true;
					target->setPrevNode((*iter).node());
				}
				if ((*iter).node()->marked() == false) {
					(*iter).node()->setPrevNode(nodeQueue.front());
					// mark the node and add it to the queue.
					(*iter).node()->setMarked(true);
					nodeQueue.push((*iter).node());
				}
			}

			// dequeue the current node.
			nodeQueue.pop();
		}
	}
}
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::UCS(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node*>& path)
{
	priority_queue<Node*, vector<Node*>, NodeCostComparer<NodeType, ArcType>> pq;
	bool goalReached = false;

	for (int i = 0; i < m_count; i++)
	{
		NodeType d = m_pNodes[i]->data();
		m_pNodes[i]->setData(make_pair(d.first, INT_MAX));
		m_pNodes[i]->setPrevNode(0);
	}
	NodeType data = pStart->data();
	pStart->setData(make_pair(data.first, 0));
	pq.push(pStart);
	pStart->setMarked(true);

	while (pq.size() != 0 && pq.top() != pDest)
	{
		list<Arc>::const_iterator iter = pq.top()->arcList().begin();
		list<Arc>::const_iterator endIter = pq.top()->arcList().end();
		pProcess(pq.top());
		for (; iter != endIter; iter++)
		{
			if ((*iter).node() != (pq.top())->getPrevNode())
			{
				//distC = weight of arc from pq.top to the child + distance from top
				float distC = (*iter).weight() + pq.top()->data().second;
				if (distC < (*iter).node()->data().second)
				{
					NodeType data = (*iter).node()->data();
					(*iter).node()->setData(make_pair(data.first, distC));
					(*iter).node()->setPrevNode(pq.top());
				}
				if ((*iter).node()->marked() == false)
				{
					pq.push((*iter).node());
					(*iter).node()->setMarked(true);
				}
			}
		}
		pq.pop();
	}
	//found target. put path into path vector(traverse the previous nodes)
	Node* target = pDest;
	while (target->getPrevNode() != NULL)
	{
		path.push_back(target);
		target = target->getPrevNode();
	}
	if (target->getPrevNode() == NULL)
	{
		path.push_back(target);
	}
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::generateHeuristics(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node *>& path)
{
	priority_queue<Node*, vector<Node*>, NodeCostComparer<NodeType, ArcType>> pq;


	for (int i = 0; i < m_count; i++)
	{
		NodeType d = m_pNodes[i]->data();
		m_pNodes[i]->setData(aStarInfoStorage(d.getNodeName(), INT_MAX - 10000, INT_MAX - 10000));
		m_pNodes[i]->setPrevNode(0);
	}
	NodeType data = pStart->data();
	pStart->setData(aStarInfoStorage(data.getNodeName(), 0, 0));
	pq.push(pStart);
	pStart->setMarked(true);

	NodeType dt = pDest->data();

	key = data.getNodeName() + "-" + dt.getNodeName();


	while (pq.size() != 0 && pq.top() != pDest)
	{
		list<Arc>::const_iterator iter = pq.top()->arcList().begin();
		list<Arc>::const_iterator endIter = pq.top()->arcList().end();
		pProcess(pq.top());
		for (; iter != endIter; iter++)
		{
			if ((*iter).node() != (pq.top())->getPrevNode())
			{
				NodeType dataTop = pq.top()->data();
				NodeType data = (*iter).node()->data();
				//distC = weight of arc from pq.top to the child + distance from top
				float distC = (*iter).weight() + dataTop.getCostSoFar();//meybe data instead
				if (distC < data.getCostSoFar())
				{
					NodeType data = (*iter).node()->data();
					(*iter).node()->setData(aStarInfoStorage(data.getNodeName(), distC, INT_MAX - 10000));
					(*iter).node()->setPrevNode(pq.top());
				}
				if ((*iter).node()->marked() == false)
				{
					pq.push((*iter).node());
					(*iter).node()->setMarked(true);
				}
			}
		}
		pq.pop();
	}
	//found target. put path into path vector(traverse the previous nodes)
	Node* target = pDest;
	NodeType asd = target->data();
	value = asd.getCostSoFar();
	while (target->getPrevNode() != NULL)
	{
		path.push_back(target);
		target = target->getPrevNode();
	}
	if (target->getPrevNode() == NULL)
	{
		path.push_back(target);
	}

}
#pragma endregion


template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::aStar(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node*>& path)
{
	priority_queue<Node*, vector<Node*>, NodeCostComparer<NodeType, ArcType>> pq;

	for (int i = 0; i<m_count; i++)
	{
		NodeType d = m_pNodes[i]->data();

		NodeType destData = pDest->data();

		//heuristic = squareRoot of (x1-x2)squared + (y1-y2)squared
		sf::Vector2f dPos = d.getPosition();
		sf::Vector2f destPos = destData.getPosition();

		//euclidean distance as the heuristic
		float heuristic = sqrt(((dPos.x - destPos.x)*(dPos.x - destPos.x)) +
			((dPos.y - destPos.y)*(dPos.y - destPos.y)));


		m_pNodes[i]->setData(aStarInfoStorage(d.getNodeName(), FLT_MAX, ((heuristic/100)*60), d.getPosition()));//70%
		m_pNodes[i]->setHeuristicProps("H: " + std::to_string(int(((heuristic / 100) * 60))), d.getPosition());
		m_pNodes[i]->setShowHeuristic(true);
		m_pNodes[i]->setPrevNode(0);
	}
	NodeType startdata = pStart->data();
	NodeType endData = pDest->data();

	sf::Vector2f startPos = startdata.getPosition();
	sf::Vector2f endPos = endData.getPosition();

	//euclidean distance as the heuristic
	//heuristic = squareRoot of (x1-x2)squared + (y1-y2)squared
	float h = sqrt(((startPos.x - endPos.x)*(startPos.x - endPos.x)) +
		((startPos.y - endPos.y)*(startPos.y - endPos.y)));

	pStart->setData(aStarInfoStorage(startdata.getNodeName(), 0, 0,startdata.getPosition()));
	pq.push(pStart);
	pStart->setMarked(true);

	while (pq.size() != 0 && pq.top() != pDest)
	{
		list<Arc>::iterator iter = pq.top()->arcList().begin();
		list<Arc>::iterator endIter = pq.top()->arcList().end();
		pProcess(pq.top());
		for (; iter != endIter; iter++)
		{
			
			if ((*iter).node() != (pq.top())->getPrevNode())
			{
				NodeType & data = (*iter).node()->data();
				NodeType topData = pq.top()->data();
				//set up the evaluation function
				int weight = (*iter).weight();
				int distC = data.getHeuristicValue() + topData.getCostSoFar() + weight;

				if (distC < data.getCostSoFar())
				{
					//set the costSoFar to be the topNodes costSoFar + the weight of the arc
					data.setCostSoFar(topData.getCostSoFar() + weight);
					//set previous pointers
					(*iter).node()->setPrevNode(pq.top());
					//set the strings and positions for the text displaying the costSoFar and the heuristic
					(*iter).node()->setCostProps("G: "+std::to_string(int(topData.getCostSoFar() + weight)), data.getPosition());
					(*iter).node()->setHeuristicProps("H: "+std::to_string(int(data.getHeuristicValue())), data.getPosition());
				}
				if ((*iter).node()->marked() == false)
				{
					pq.push((*iter).node());
					(*iter).node()->setMarked(true);
				}
			}
		}
		
		make_heap(const_cast<Node**>(&pq.top()), const_cast<Node**>(&pq.top()) + pq.size(),
			NodeCostComparer<NodeType, ArcType>());
		pq.pop();
		
		
	}
	//found target. put path into path vector(traverse the previous nodes)
	Node* target = pDest;
	while (target->getPrevNode() != NULL)
	{
		target->setInPath(true);
		path.push_back(target);
		target = target->getPrevNode();
	}
	if (target->getPrevNode() == NULL)
	{
		path.push_back(target);
	}

}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::outputPath(std::vector<Node*>& path)
{
	for (int i = 0; i < path.size(); i++)
	{
		NodeType data = path.at(i)->data();
		if (i == 0)
		{
			cout << "end of path" << endl;
		}
		cout << "name: " << data.getNodeName() << " " << "cost: " << data.getCostSoFar() << " " << "heuristic: " << data.getHeuristicValue() << " " << "position" << data.getPosition().x << "," << data.getPosition().y<<endl;
		if (i == path.size() - 1)
		{
			cout << "start of path" << endl;
		}

	}
}

//passes the mouse co-ords to each node to see if the mouse is hovering over them
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::highlightNodes(sf::Vector2i mousePos, bool click)
{
	for (int index = 0; index < m_maxNodes; index++) 
	{
		m_pNodes[index]->highlight(mousePos, click);
	}
}

//find the indices of the 2 selecred nodes
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::findSelectedIndices()
{
	for (int index = 0; index < m_maxNodes; index++)
	{
		if (m_pNodes[index]->getSelected() == true)
		{
			selectedNodeIndices.push_back(index);
		}
	}
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::draw(sf::RenderTarget& window)
{
	//draws the arcs. i+=2 because the arcs are named in the arcs file twice(undirected)
	for (int i = 0; i < arcPositions.size(); i += 2)
	{
		sf::Vertex vertices[2] =
		{
			sf::Vertex(arcPositions.at(i), sf::Color(0,255,255,255), sf::Vector2f(0, 0)),
			sf::Vertex(arcPositions.at(i + 1), sf::Color(0, 255, 255, 255), sf::Vector2f(0, 0))
		};

		window.draw(vertices, 2, sf::Lines);
	}

	//draw the arc weights
	for (int i = 0; i < arcWeights.size(); i+=2)
	{
		sf::Text s = arcWeights.at(i);
		window.draw(s);
	}

	//draw each node
	int index;
	for (index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0) {
			m_pNodes[index]->draw(window);
		}
	}
}

#include "GraphNode.h"
#include "GraphArc.h"


#endif
