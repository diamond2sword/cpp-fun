#include <vector>
#include <iostream>

struct Character {
	int x;
	int y;
	void move(int _x, int _y) {
		x = x + _x;
		y = y + _y;
	}
	void position() {
		std::cout << "I am at " << x << ", " << y << std::endl;	
	}
};

class Command {
public:	
	~Command() {};
	virtual void execute(Character& c) = 0;
	virtual void undo(Character& c) = 0;
};

template<int x, int y> class Move: public Command {
public:
	void execute(Character& c) {
		c.move(x, y);
	}
	void undo(Character& c) {
		c.move(-x, -y);
	}
};

template<int y> Command* vMove() {return new Move<0, y>();}
template<int x> class hMove: public Move<x, 0> {};

int main() {
	Character ginuel;
	std::vector<Command*> history;

	Command* c1 = new hMove< -1>();
	history.push_back(c1);
	Command* c2 = new vMove<1>();
	history.push_back(c2);
	Command* c3 = new hMove<1>();
	history.push_back(c3);
	Command* c4 = new vMove< -1>();
	history.push_back(c4);

	for (int i=0; i < history.size(); i++) {
		history[i]->execute(ginuel);
		ginuel.position();
	}

	for (int i=0; i < history.size(); i++) {
		history[i]->undo(ginuel);
		ginuel.position();
	}
	return 0;
}
