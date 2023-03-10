#include "Channel.h"
#include "EventLoop.h"



Channel::Channel(EventLoop* eloop, int fd) : eloop_(eloop), fd_(fd), listen_events_(0), ready_events_(0), in_epoll_(false) {}



Channel::~Channel() {
    if(fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}



int Channel::GetFd() const {
    return fd_;
}



uint32_t Channel::GetListenEvents() const {
    return listen_events_;
}



uint32_t Channel::GetReadyEvents() const {
    return ready_events_;
}



bool Channel::CheckInEpoll() const {
    return in_epoll_;
}



void Channel::SetInEpoll(bool in = true) {
    in_epoll_ = in;
}



void Channel::EnableRead() {
    listen_events_ = EPOLLIN | EPOLLPRI;
    eloop_->UpdateChannel(this);
}



void Channel::EnableWrite() {
    listen_events_ |= EPOLLOUT;
    eloop_->UpdateChannel(this);
}



void Channel::SetReadyEvents(uint32_t ev) {
    ready_events_ |= ev;
}



void Channel::HandleEvent() {
    if (ready_events_ & (EPOLLIN | EPOLLPRI)) {
        read_callback_();
    }
    if (ready_events_ & EPOLLOUT) {
        write_callback_();
    }
}




