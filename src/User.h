#ifndef USER_H
#define USER_H

#include <string>

// 用户：可以报名活动的参与者。
// 编号创建后保持不变（不提供 setId），姓名、联系方式和参赛等级允许修改。
class User {
public:
    // 参赛等级的取值范围（第 5 次上机新增：竞赛类活动用它做资格检查）
    enum { kMinLevel = 1, kMaxLevel = 10 };

    User();
    User(const std::string& id, const std::string& name, const std::string& contact, int level);

    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getContact() const;
    int getLevel() const;  // 参赛等级，1~10

    // 修改允许变更的信息；空字符串或越界等级会被拒绝并返回 false。
    bool setName(const std::string& name);
    bool setContact(const std::string& contact);
    bool setLevel(int level);

    // 单行完整信息，用于单个用户显示和用户列表显示。
    std::string describe() const;

private:
    std::string id_;
    std::string name_;
    std::string contact_;
    int level_;
};

#endif
