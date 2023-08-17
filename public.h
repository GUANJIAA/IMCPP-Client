#ifndef PUBLIC_H
#define PUBLIC_H

enum ServerTypeId
{
    LOGIN_MSG = 1,
    LOGOUT_MSG ,
    REGISTER_MSG ,
    RETRIEVE_MSG ,
    NEW_CONNECTION ,
    QUERY_ADMININFO ,
    UPDATE_ADMININFO ,
    ONE_CHAT_MSG ,           // 私聊消息
    DEL_CHAT_MSG ,           // 删除聊天消息
    QUERY_CHAT_MSG ,         // 查询聊天消息
    QUERY_OFFLINE_CHAT_MSG , // 查询离线聊天消息
    ADD_FRIEND_MSG ,         // 添加好友
    DEL_FRIEND_MSG ,         // 删除好友
    QUERY_FRIEND_MSG ,       // 查询好友列表
    GROUP_CHAT_MSG ,         // 群组聊天消息
    QUERY_GROUPCHAT_MSG,     // 查询群组聊天消息
    CREATE_GROUP_MSG ,       // 创建群组
    ADD_GROUP_MSG ,          // 添加群组
    QUERY_GROUP_MSG ,        // 查询所有群组及所有群友
    QUERY_GROUP_USERS_MSG ,  // 查询所在群所有群友
    DEL_GROUP_MSG ,          // 删除群组
    QUIT_GROUP_MSG ,         // 退出群组
    DEPART_CHAT_MSG ,        // 部门聊天消息
    CREATE_DEPART_MSG ,      // 创建部门
    ADD_DEPART_MSG ,         // 加入部门
    QUERY_DEPART_USERS_MSG , // 查询所在部门的所有同事
    DEL_DEPART_MSG ,         // 删除部门
    QUIT_DEPART_MSG ,        // 退出部门
    QUERY_DEPARTCHAT_MSG,    // 查询部门聊天信息
};

#endif // PUBLIC_H
