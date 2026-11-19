#ifndef USER_H
#define USER_H

#include <string>

// 用户：可以报名活动的参与者。
// 编号在创建后保持不变（不提供 setId），姓名与联系方式允许修改。
class User {
public:
    User();
    User(const std::string& id, const std::string& name, const std::string& contact);

    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getContact() const;

    // 修改允许变更的信息；传入空字符串会被拒绝并返回 false。
    bool setName(const std::string& name);
    bool setContact(const std::string& contact);

    // 单行完整信息，用于单个用户显示和用户列表显示。
    std::string describe() const;

private:
    std::string id_;
    std::string name_;
    std::string contact_;
};

#endif
