#ifndef SOCKET_DATA_H
#define SOCKET_DATA_H

enum Type {
    LOGIN_REQ, // request
    LOGIN_RES, // response
    USERS_REQ,
    USERS_RES,
};

struct Head
{
    int type;
    int length;
};

struct Data
{
    Head head;
    char body[0];
};

#endif // SOCKET_DATA_H
