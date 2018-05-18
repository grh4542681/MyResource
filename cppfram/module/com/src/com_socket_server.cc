#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>

#include <com_exception.h>
#include <com_socket_server.h>
#include <com_sockfd_op.h>
#include <thread_safe_queue.h>

namespace COM {

ComSocketServer::ComSocketServer(SockInfo* info, int max_clients):ComBaseSocket(info)
{
    this->max_clients = max_clients;
    this->cur_clients = 0;
    this->status = STATUS::S_UNUSE;
}

ComSocketServer::~ComSocketServer()
{

}

void ComSocketServer::s_open() throw(ComException)
{
    if (this->status != STATUS::S_UNUSE) {
        throw ComException(ComErrCode["BAD_STATUS"], ComErrMark::BAD_STATUS);
    }
    this->status = STATUS::S_INIT;

    int ret = 0;
    struct sockaddr_in addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->sp_sockargs->info.port);
    addr.sin_addr.s_addr = inet_addr((this->sp_sockargs->info.addr).c_str());

    switch(this->sp_sockargs->info.protocol) {
        case COM::PROTOCOL::TCP:
            this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (this->sockfd < 0) {
                throw ComException(ComErrCode["SOCK_INI_ERR"], ComErrMark::SOCK_INI_ERR);
            }
            ret = bind(this->sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
            if (ret < 0) {
                throw ComException(ComErrCode["BIND_INI_ERR"], ComErrMark::BIND_INI_ERR);
            }
            ret = listen(this->sockfd, this->max_clients);
            if (ret < 0) {
                throw ComException(ComErrCode["LISTEN_INI_ERR"], ComErrMark::LISTEN_INI_ERR);
            }
            this->status = STATUS::S_READY;
            break;
        case COM::PROTOCOL::UDP:
            break;
        default:
            throw ComException(ComErrCode["UNKNOW_PROTOCOL"], ComErrMark::UNKNOW_PROTOCOL);
    }
}

ComSockFdOp* ComSocketServer::s_accept() throw(ComException)
{
    if (this->status != STATUS::S_READY) {
        throw ComException(ComErrCode["BAD_STATUS"], ComErrMark::BAD_STATUS);
    }
    this->status = STATUS::S_ACCEPT;
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len;
    int apt_fd = accept(this->sockfd, (struct sockaddr*)(&cli_addr), &cli_addr_len);
    if (apt_fd < 0) {
        throw ComException(ComErrCode["SOCK_ACCEPT_ERR"], ComErrMark::SOCK_ACCEPT_ERR);
    }
    this->status = STATUS::S_READY;

    std::shared_ptr<ComSockFdOp> sp_client = std::make_shared<ComSockFdOp>(apt_fd);
    sp_client->ip = inet_ntoa(cli_addr.sin_addr);
    sp_client->port = ntohs(cli_addr.sin_port);
    sp_client->index = apt_fd;

    clients.push_back(*sp_client);
    sp_client->status = STATUS::F_READY;
    this->cur_clients++;
    return (sp_client.get());
}

void ComSocketServer::c_close(ComSockFdOp* client) throw(ComException)
{
    if (this->status != STATUS::S_INIT || this->status != STATUS::S_UNUSE) {
        throw ComException(ComErrCode["BAD_STATUS"], ComErrMark::BAD_STATUS);
    }
    std::list<ComSockFdOp>::iterator it = this->clients.begin();
    while (it !=clients.end()) {
        if (it->index == client->index) {
            break;
        }
    }
    if (it == this->clients.end()) {
        throw ComException(ComErrCode["UNKNOW_CLIENT"], ComErrMark::UNKNOW_CLIENT);
    }
    if (client->status != STATUS::F_READY) {
        client->status = STATUS::F_CLOSE;
        return;
    }
    close(client->fd);
    client->status = STATUS::F_CLOSE;
    (this->cur_clients)--;
}

void ComSocketServer::s_close() throw(ComException)
{
    if (this->status != STATUS::S_READY) {
        throw ComException(ComErrCode["BAD_STATUS"], ComErrMark::BAD_STATUS);
    }
    std::list<ComSockFdOp>::iterator it;
    it = this->clients.begin();
    while (it != clients.end()) {
        if (it->status != STATUS::F_CLOSE) {
            close(it->fd);
            it->status = STATUS::F_CLOSE;
            (this->cur_clients)--;
        }
    }
    clients.clear();
}

void ComSocketServer::start() throw(ComException)
{
    try {
        this->s_open();
    } catch (ComException &e) {
        throw e;
    }
}

}
