#include<iostream>
#include<vector>
#include<string>

using namespace std;

class Client;
// Mediator
class Chat
{
	vector<Client*> m_Clients;
public :
	void addClient(Client* client);
	void chat(Client* client, const string &message);
	~Chat()
	{
		vector<Client*>::iterator iter    = m_Clients.begin();
		vector<Client*>::iterator iterEnd = m_Clients.end();
		for (; iter != iterEnd; ++iter)
			delete* iter;
		cout << "chat deleted" << endl;
	}
};

// Clinet
class Client
{
private :
	string m_Name;
	Chat* m_Chat;
public :
	Client(Chat* chat, const string& message);
	void sendMessage(const string &message);
	void receiveMessage(Client* client, const string &message);
	~Client() { cout << m_Name << " deleted" << endl; }
};

void Chat::addClient(Client* client)
{
	m_Clients.push_back(client);
}

void Chat::chat(Client* client, const string &message)
{
	vector<Client*>::iterator iter    = m_Clients.begin();
	vector<Client*>::iterator iterEnd = m_Clients.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) != client)
		{
			(*iter)->receiveMessage(client, message);
		}
	}
}

Client::Client(Chat* chat, const string& message)
{
	m_Chat = chat;
	m_Name = message;
	chat->addClient(this);
}

void Client::sendMessage(const string& message)
{
	cout << m_Name << "mis sending message : " << message << endl;
	m_Chat->chat(this, message);
}
void Client::receiveMessage(Client* client, const string& message)
{
	cout << "From " << client->m_Name << " To " << m_Name << " : " << message << endl;
}

int main()
{
	Chat* chat = new Chat();

	Client* cl1 = new Client(chat, "cl1");
	Client* cl2 = new Client(chat, "cl2");
	Client* cl3 = new Client(chat, "cl3");
	Client* cl4 = new Client(chat, "cl4");

	cl1->sendMessage("hello I am cl1");
	cl2->sendMessage("hello I am cl2");
	cl3->sendMessage("hello I am cl3");
	cl4->sendMessage("hello I am cl4");ㅁ

	delete chat;
	// delete cl1;
	// delete cl2;
	// delete cl3;
	// delete cl4;

	// vector는 메모리 해제가 어떻게 이루어지는 거지 ?

	return 0;
}