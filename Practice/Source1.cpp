#include<iostream>
#include<string>

using namespace std;

class Game
{
protected :
	int m_cur_player{ 0 };
	int m_num_of_players;
protected :
	virtual void start() = 0;
	virtual bool have_winner() = 0;
	virtual void take_turn() = 0;
	virtual int get_winner() = 0;
public :
	explicit Game(int num_of_players) :
		m_num_of_players(num_of_players){}
	void run()
	{
		start();
		while (!have_winner())
			take_turn();
		cout << "Player " << get_winner() << " wins\n"  << endl;
	}
};

class Chess : public Game
{
private:
	int turns{ 0 }, max_turns{ 10 };
public :
	explicit Chess() :Game(2){}
protected :
	void start() override
	{
		cout << "start chess with " << m_num_of_players << endl;
	}
	bool have_winner() override
	{
		return turns == max_turns;
	}
	void take_turn() override
	{
		cout << "Turn " << turns << " taken by player " << m_cur_player << endl;
		turns++;
		m_cur_player = (m_cur_player + 1) % m_num_of_players;
	}
	int get_winner() override
	{
		return m_cur_player;
	}
};

int main()
{
	Chess chess;
	chess.run();
	getchar();
	return 0;
}











