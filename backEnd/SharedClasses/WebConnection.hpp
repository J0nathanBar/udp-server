#ifndef WEBCONNECTION_HPP
#define WEBCONNECTION_HPP

#pragma once
#include<iostream>
#include<boost/beast.hpp>
#include<boost/asio.hpp>


class WebConnection
{
public:
    WebConnection();
    ~WebConnection();
    void Run();

private:

};

#endif