#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <string>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "irc_bot.h"

using std::string;

IrcBot::IrcBot(char *h, char *p) : hostname(h), port(p)
{
	conn_to_irc();
	reader = std::thread(&IrcBot::read_data, this, sock);
}

void IrcBot::conn_to_irc()
{
	struct addrinfo hints, *addresses, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(hostname, port, &hints, &addresses);
	for(rp = addresses; rp != NULL; rp = rp->ai_next)
	{
		sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if(sock == -1) continue;

		if(connect(sock, rp->ai_addr, rp->ai_addrlen) != -1)
		{
			struct sockaddr_in *host_addr = (struct sockaddr_in*)rp->ai_addr;
			struct in_addr *host_ip = &host_addr->sin_addr;
			char ip_str[20];
			memset(ip_str, 0, 20);
			inet_ntop(rp->ai_family, host_ip, ip_str, 20);

			std::cout << "Connected to " << hostname
					  << ", ip: " << ip_str << std::endl;
			break;
		}

		fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK);

		freeaddrinfo(addresses);
	}
}

IrcBot::~IrcBot()
{
    should_read = false;
    reader.join();
}

void IrcBot::conn(const string &nick, const string &user)
{
    std::string n = "NICK " + nick + "\r\n";
	write_data(n);

	std::string u = "USER " + user + "\r\n";
	write_data(u);
}

void IrcBot::join_channel(const std::string &channel)
{
    std::string join = "JOIN " + channel + "\r\n";
	write_data(join);
}

void IrcBot::write_data(const string &data)
{
    std::cout << "Sent: " + data << std::endl;
	write(sock, data.c_str(), data.length());
}

void IrcBot::read_data(int sock)
{
	std::cout << "Reading..." << std::endl;
	char buffer[BUFFER_SIZE];
    should_read = true;
	while (should_read)
	{
		read(sock, buffer, BUFFER_SIZE);
        string received = buffer;
		std::cout << received;
        if(received.substr(0, 4) == "PING") 
        {
            int pos = received.find("\r\n");
            string pong_response = "PONG " + received.substr(4, pos) + "\r\n";
            std::cout << pong_response << std::endl;
            write_data(pong_response);
        }
	}
}
