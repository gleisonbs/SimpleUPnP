#include <map>
#include <string>

const int BUFFER_SIZE = 4096;
const std::string BROADCAST_ADDRESS = "239.255.255.250";
const unsigned short BROADCAST_PORT = 1900;

const std::string SEARCH_REQUEST_STRING = \
    "M-SEARCH * HTTP/1.1\r\n"                   \
    "ST:UPnP:rootdevice\r\n"                    \
    "MX: 2\r\n"                                 \
    "MAN:\"ssdp:discover\"\r\n"                 \
    "HOST: 239.255.255.250:1900\r\n"            \
    "\r\n";

const std::string HTTP_HEADER_ACTION = \
    "POST %s HTTP/1.1\r\n"                                      \
    "Host: %s:%u\r\n"                                           \
    "User-Agent: Linux/3.14.4-1-ARCH, UPnP/1.0, MiniUPnPc/\r\n" \
    "Content-Length: %d\r\n"                                    \
    "Content-Type: text/xml\r\n"                                \
    "SOAPAction: \"%s#%s\"\r\n"                                 \
    "Connection: Close\r\n"                                     \
    "Cache-Control: no-cache\r\n"                               \
    "Pragma: no-cache\r\n"                                      \
    "\r\n";
            
const std::string SOAP_ACTION = \
    "<?xml version=\"1.0\"?>\r\n"                       \
    "<s:Envelope xmlns:s="                              \
    "\"http://schemas.xmlsoap.org/soap/envelope/\" "    \
    "s:encodingStyle="                                  \
    "\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n"\
    "<s:Body>\r\n"                                      \
    "<u:%s xmlns:u=\"%s\">\r\n%s\r\n"                   \
    "</u:%s>\r\n"                                       \
    "</s:Body>\r\n"                                     \
    "</s:Envelope>\r\n"                                 \
    "\r\n";

static const std::map<std::string, std::string> control_parameters = {
{ 
    "AddPortMapping", 
    "<NewRemoteHost></NewRemoteHost>\r\n"                           \
    "<NewExternalPort>%u</NewExternalPort>\r\n"                     \
    "<NewProtocol>%s</NewProtocol>\r\n"                             \
    "<NewInternalPort>%u</NewInternalPort>\r\n"                     \
    "<NewInternalClient>%s</NewInternalClient>\r\n"                 \
    "<NewEnabled>1</NewEnabled>\r\n"                                \
    "<NewPortMappingDescription>%s</NewPortMappingDescription>\r\n" \
    "<NewLeaseDuration>%d</NewLeaseDuration>"
}, 
{
    "DeletePortMapping", 
    "<NewRemoteHost></NewRemoteHost>\r\n"       \
    "<NewExternalPort>%u</NewExternalPort>\r\n" \
    "<NewProtocol>%s</NewProtocol>"
},
{
    "GetExternalIPAddress",
    "<NewExternalIPAddress></NewExternalIPAddress>"
}};


class SimpleUPnP
{
public:
    SimpleUPnP(int=4);

    void get_igd_location();
    void prepare_tcp_socket();//const std::string&, unsigned short);
    void exec_command(const std::string&, const std::string="", unsigned short=0, unsigned short=0, const std::string="", int=60, const std::string="");
    std::string make_request(const std::string&, const std::string="", unsigned short=0, unsigned short=0, const std::string="", int=60, const std::string="");
    void get_description();
    void parse_igd_location(std::string&);
    std::string read_response(int, const std::string&);
    std::string parse_response(const std::string, const std::string&);

private:
    unsigned short igd_port;
    std::string igd_host;
    std::string igd_path;
    
    int tcp_sock;
    int m_time_out;
};




