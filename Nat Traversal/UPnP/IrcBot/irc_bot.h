#include <string>
#include <thread>

const int BUFFER_SIZE = 8192;

class IrcBot
{
public:
	IrcBot(char*, char*);
	~IrcBot();

    void conn(const std::string&, const std::string&);
    void join_channel(const std::string&);
	void read_data(int);
	void write_data(const std::string&);

private:
    void conn_to_irc();

    bool should_read;
	std::thread reader;
	int sock;
	char *hostname, *port;
};
