#include<iostream>

using namespace std;

class Game
{
protected :
	int m_num_of_players;
	int m_cur_player{ 0 };
protected :
	virtual void start() = 0;
	virtual void take_turn() = 0;
	virtual bool have_winner() = 0;
	virtual int get_winner() = 0;
public :
	Game(const int& num_of_players):m_num_of_players(num_of_players){}
	void play()
	{
		start();
		while (!have_winner())
			take_turn();
		cout << "Player " << get_winner() << " win" << endl;
	}
};

class Chess : public Game
{
private :
	int m_cur_turn{ 0 };
	int m_max_turn = 10;
public :
	Chess(const int& num_of_players) : Game(num_of_players){}
	virtual void start()
	{
		cout << "Chess play with" << m_num_of_players << endl;
	}
	virtual void take_turn()
	{
		cout << "Turn " << m_cur_turn << " taken by " << m_cur_player << endl;
		++m_cur_turn;
		m_cur_player = (m_cur_player + 1) % m_num_of_players;
	}
	virtual bool have_winner()
	{
		return m_cur_turn == m_max_turn;
	}
	virtual int get_winner()
	{
		return m_cur_player;
	}
};

int main()
{
	Chess chess(2);
	chess.play();
	return 0;
}