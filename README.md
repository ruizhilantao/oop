# 校园活动组织与协同平台（项目方向 A · 第 1～4 次上机合并版）

面向对象程序设计与构造 C++ 上机实验作业。
本工程按**第 4 次上机（Lab04 多类型活动）的终态**提交：一个可独立编译、可运行的完整工程，
包含 Lab01～Lab04 全部已验收功能，第 4 次新增的继承体系在此基础上增量扩展。

- 语言标准：C++11 及以上（工程按 C++17 编译，代码本身只用到 C++11 特性）
- 编译器验证：MinGW-w64 g++ 16.2.0，`-std=c++17 -Wall -Wextra -Wpedantic` **零警告、零错误**
- 测试情况：T1/T2/T3 三个验收用例 + 1 个健壮性用例全部通过，内存泄漏探针检测 `outstanding=0`

---

## 1. 目录结构

```
CampusActivityLab04/
├─ src/                      源码（.h 与 .cpp 分离）
│  ├─ main.cpp               菜单、输入提示、结果显示（界面层）
│  ├─ ActivityManager.h/.cpp 数据仓库与业务规则（用户、活动、报名协作）
│  ├─ Activity.h/.cpp        活动基类：公共字段 + 报名规则 + 公共信息显示
│  ├─ Lecture.h/.cpp         派生类：讲座（特有信息：主讲人）
│  ├─ Competition.h/.cpp     派生类：竞赛（特有信息：参赛要求）
│  ├─ Sport.h/.cpp           派生类：体育（特有信息：体育项目）
│  ├─ User.h/.cpp            用户类
│  └─ Input.h/.cpp           控制台输入工具（读行、去空白、整数解析）
├─ test/
│  ├─ T1_types.in            验收用例 T1 输入
│  ├─ T2_base_interface.in   验收用例 T2 输入
│  ├─ T3_regression.in       验收用例 T3 输入（前三次功能回归）
│  ├─ robustness.in          附加健壮性用例（非法输入 / 空输入 / 容量 / 重复编号）
│  └─ *.out                  上述用例的实际运行结果
├─ tools/leak_probe.cpp      本地自测用 new/delete 统计（不参与正式编译）
├─ build.bat                 Windows 一键编译
├─ run_tests.bat             批量运行测试并生成 test/*.out
├─ CMakeLists.txt            CLion / VS Code 等 IDE 使用
└─ README.md                 本文件（含类设计说明、变更说明、测试记录）
```

> 关于“保留 Lab01～Lab08 八个可区分版本”：本次按“只交第 4 次终态完整工程”的方式提交，
> 各次迭代的差异已在第 6 节逐条列出（修改位置、原因、影响范围、验证方法），
> 需要留版时可按该节内容还原 Lab01/Lab02/Lab03 三个历史版本。

---

## 2. 编译与运行

### 2.1 一键编译（Windows + MinGW-w64）

```bat
build.bat
build.bat "C:\mingw64\bin\g++.exe"    :: 指定编译器路径
build\CampusActivity.exe
```

### 2.2 手动编译

```bat
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -I src ^
    src\Input.cpp src\User.cpp src\Activity.cpp src\Lecture.cpp ^
    src\Competition.cpp src\Sport.cpp src\ActivityManager.cpp src\main.cpp ^
    -o build\CampusActivity.exe
```

### 2.3 CMake

```bat
cmake -S . -B build-cmake
cmake --build build-cmake
```

### 2.4 运行测试

```bat
run_tests.bat
```

会把 `test\*.in` 逐个喂给程序，并把控制台输出保存到 `test\*.out`。

> 源码文件为 UTF-8 编码。若在旧版 cmd 窗口里中文显示乱码，先执行 `chcp 65001`
> （`build.bat` 与 `run_tests.bat` 已自动处理）。

---

## 3. 功能实现对照

| 次数 | 要求 | 本工程实现位置 |
| --- | --- | --- |
| Lab01 | 创建并显示用户、列表显示全部用户 | `User`、`ActivityManager::addUser`、`main.cpp` `createUser/showUserList` |
| Lab01 | 创建并显示活动（编号/名称/时间/地点/容量/报名人数） | `Activity::describeCommon`、`createActivity` |
| Lab01 | 编号非空且唯一，容量为正整数 | `ActivityManager::addUser/addActivity`、`input::readPositiveInt` |
| Lab02 | 修改用户信息，编号不变 | `User::setName/setContact`、`ActivityManager::modifyUser` |
| Lab02 | 修改活动信息（名称/时间/地点/容量），编号不变 | `Activity::updateBasicInfo`、`ActivityManager::modifyActivity` |
| Lab02 | 报名、重复报名拦截、满员拦截、人数 +1 | `Activity::registerUser`、`RegisterResult` |
| Lab02 | 取消报名、人数 -1、不能小于 0 | `Activity::unregisterUser` |
| Lab03 | 记录用户报了什么、活动有谁报名 | `Activity::registrants_`（唯一数据来源） |
| Lab03 | 按用户查活动、按活动查用户，空时明确显示 | `findActivitiesOfUser`、`getRegistrants`、`showRegistrantsOfActivity` |
| Lab03 | 人数与实际记录一致 | 人数由 `registrants_.size()` 计算，不另设计数器 |
| Lab03 | 有报名关系时拒绝删除并说明原因 | `removeUser`、`removeActivity` |
| Lab04 | 基类 `Activity` 提取公共字段，≥2 个派生类 | `Activity` + `Lecture`/`Competition`/`Sport` |
| Lab04 | 创建时可选类型，保存并显示各类型特有信息 | `createActivity` 分支 + `activitySpecificInfo` |
| Lab04 | 列表显示类型及完整信息 | `describeCommon()` + `activitySpecificInfo()` |
| Lab04 | 只查看某一类型，无该类型时提示 | `findActivitiesByType`、`showActivitiesByType` |
| Lab04 | 各类型都能查询/报名/取消报名 | 报名逻辑在基类 `Activity`，派生类自动复用 |

**关于主菜单编号**：任务书“最终界面”的 6 是“保存/读取”（第 7 次上机内容）。
本版本按“后续实验只增加、不重排”的原则，把用户管理放在 1，其余顺序不变，
第 7 次上机直接在末尾追加 `7. 保存 / 读取` 即可，不需要重新设计菜单：

```
1. 用户管理   2. 活动管理   3. 浏览活动
4. 报名       5. 取消报名         6. 查询 / 统计      0. 退出
```

---

## 4. 操作说明

**主菜单**：1 用户管理、2 活动管理、3 浏览活动、4 报名、5 取消报名、6 查询/统计、0 退出。
每个子菜单的 `0` 都是“返回上一级”，非法编号或空回车只提示错误、不退出程序、不改动数据。

| 菜单 | 子操作 | 说明 |
| --- | --- | --- |
| 1 用户管理 | 1 新建 / 2 修改 / 3 显示全部 / 4 删除 | 编号不可改；修改时直接回车表示保留原值 |
| 2 活动管理 | 1 发布新活动 | 先选类型（1 讲座 / 2 竞赛 / 3 体育），再填公共字段，最后填本类型特有信息 |
| | 2 修改活动信息 | 可改名称/时间/地点/容量，编号不变；容量不能改到小于已报名人数 |
| | 3 删除活动 | 有报名记录时拒绝删除并说明原因 |
| 3 浏览活动 | 1 全部活动 / 2 只查看某一类 | 列表显示编号、名称、类型、时间、地点、容量、已报名人数、特有信息 |
| 4 报名 | 输入用户编号 + 活动编号 | 依次校验用户存在 → 活动存在 → 未重复报名 → 未满员 |
| 5 取消报名 | 输入用户编号 + 活动编号 | 没有报名记录时提示失败，集合不变 |
| 6 查询/统计 | 1 活动的报名用户 / 2 用户的报名活动 / 3 按类型统计 | 空结果明确显示“（空）”或“当前没有…类活动” |

数据在程序启动时为空，全部通过菜单录入（不预置写死的验收结果）。

---

## 5. 类设计说明

### 5.1 类与职责

| 类 | 职责 | 主要属性 | 主要成员函数 |
| --- | --- | --- | --- |
| `User` | 表示一个用户，保管编号/姓名/联系方式 | `id_` `name_` `contact_`（private） | `getId/getName/getContact`、`setName/setContact`、`describe` |
| `Activity` | 活动基类：公共字段 + 全部报名规则 | `id_` `name_` `time_` `place_` `capacity_` `type_`（**protected**）、`registrants_`（private） | `isFull`、`hasRegistrant`、`updateBasicInfo`、`registerUser`、`unregisterUser`、`describeCommon` |
| `Lecture` | 讲座，补充主讲人 | `speaker_`（private） | `getSpeaker` |
| `Competition` | 竞赛，补充参赛要求 | `requirement_`（private） | `getRequirement` |
| `Sport` | 体育类活动，补充体育项目 | `sportName_`（private） | `getSportName` |
| `ActivityManager` | 持有全部对象，完成按编号查找与用户↔活动协作，保证数据一致 | `users_`（`vector<User>`）、`activities_`（`vector<Activity*>`） | `addUser/modifyUser/removeUser`、`addActivity/modifyActivity/removeActivity`、`registerForActivity/cancelRegistration`、`findUser/findActivity/findActivitiesByType` |
| `input`（命名空间） | 统一处理控制台输入，保证格式错误不破坏数据 | — | `readLine/trim/isBlank/parseInt/readInt/readPositiveInt` |

### 5.2 关系

```
ActivityManager  ──组合──>  User（vector<User>，值语义）
                 ──组合──>  Activity*（vector<Activity*>，多态持有并负责 delete）
Activity（基类）
   ├── Lecture      （public 继承，复用公共字段与报名逻辑）
   ├── Competition
   └── Sport
```

- **组合**：`ActivityManager` 是 `User`/`Activity` 的唯一所有者，析构时统一 `delete`，并禁用拷贝构造与赋值，避免重复释放。
- **继承**：`Lecture`/`Competition`/`Sport` 通过 `public` 继承得到编号、名称、时间、地点、容量以及**全部报名/取消/人数统计逻辑**，只增加一个特有字段。
- **为什么用 `protected`**：公共字段放在 `protected`，派生类构造函数可以直接读取/传递，而外部代码仍只能通过 `public` 的 `const` 成员函数访问，封装不被破坏。
- **为什么是基类指针数组**：三种活动需要放进同一份列表、同一套报名/查询/删除逻辑里处理，只能用 `vector<Activity*>`；析构函数因此声明为 `virtual`（通过基类指针释放派生对象时的必要技术手段，不是为展示语法而加）。
- **为什么报名记录只存用户编号**：`Activity::registrants_` 存 `string` 编号而非 `User*`，避免用户对象销毁后出现野指针；同时“活动有谁报名”和“用户报了哪些活动”都从这一份数据反查，天然保证一致。

### 5.3 关键规则集中在哪里

| 规则 | 唯一实现位置 |
| --- | --- |
| 重复报名拦截、满员拦截 | `Activity::registerUser`（返回 `RegisterResult`），菜单层不重复判断 |
| 报名人数 | `Activity::getRegisteredCount()` = `registrants_.size()`，无独立计数器 |
| 编号唯一 | `ActivityManager::addUser/addActivity` 里按编号查找后拒绝 |
| 容量为正整数 | `input::readPositiveInt`（循环重试）+ `addActivity` 兜底校验 |
| 改容量不能小于已报名人数 | `ActivityManager::modifyActivity` 先判并给出具体原因，`Activity::updateBasicInfo` 再兜底 |
| 有报名时禁止删除 | `removeUser`、`removeActivity` |
| 用户/活动不存在 | 报名、取消、修改、查询各自先按编号查找，找不到就提示且不改数据 |

---

## 6. 四次迭代的变更说明（增量开发记录）

> 每次都在同一工程、同一批类上继续扩展，没有写成互相独立的两套程序。

### Lab01 → Lab02：封装、构造、const

- **修改位置**：`User` 增加 `setName/setContact`；`Activity` 增加 `capacity_`、`updateBasicInfo`、报名集合与 `registerUser/unregisterUser`；`ActivityManager` 增加 `modifyUser`、`modifyActivity`、`registerForActivity`、`cancelRegistration`；`main.cpp` 增加“修改用户/活动”和“报名/取消报名”菜单。
- **原因**：第 2 次要求“能改用户和活动信息、能报名、能取消”，并且关键字段私有。
- **影响范围**：字段全部改为 private，外部只经 const 成员函数读取；原有“创建 + 显示”功能不受影响；改容量新增“不得小于已报名人数”的约束，避免人数与容量矛盾。
- **验证方法**：T3 回归用例中“报名 1/2 → 报名 2/2 → 重复报名被拒 → 取消回 1/2 → 重复取消被拒”，以及“容量改小到 1 被拒绝、改回 3 成功且编号不变”。

### Lab02 → Lab03：组合、vector、对象协作

- **修改位置**：`Activity::registrants_`（`vector<string>`）成为报名关系唯一数据来源；新增 `getRegistrants`、`hasRegistrant`、`ActivityManager::findActivitiesOfUser`、`removeUser/removeActivity`；菜单增加“查询/统计”和两个删除操作。
- **原因**：第 3 次要求记录双向报名关系、支持双向查询并保证一致性。
- **影响范围**：报名人数不再单独存储，改为由集合大小算出，删除了“人数可能与记录不符”的可能；删除操作新增“有报名则拒绝”的约束。
- **验证方法**：T3 中“查询 A01 报名用户 → U03”“查询 U02 报名活动 → 空”“统计里人数 1/2 与记录 1 条一致”“删除有报名的活动/用户被拒绝”。

### Lab03 → Lab04：继承、派生类、protected（本次新增）

- **修改位置**：把原 `Activity` 拆成基类 + 三个派生类 `src/Lecture.*`、`src/Competition.*`、`src/Sport.*`；
  基类新增 `ActivityType type_`（protected）、`getType()`、`activityTypeName()`、`activitySpecificInfo()`；
  `createActivity` 改为“先选类型再录入”并按类型 `new` 对应派生类；新增“按类型浏览”和“按类型统计”。
- **原因**：第 4 次要求支持多种活动类型，既保存公共信息也保存各类型特有信息，并能分类显示、兼容已有功能。
- **影响范围**：公共字段与报名逻辑从“只有一个活动类”上移到基类，三种活动共用一份实现；
  因为 `activities_` 改为保存基类指针，基类析构函数改为 `virtual`；显示层增加“按类型分支取特有信息”。
- **验证方法**：T1（分别创建讲座/竞赛并显示实际类型）、T2（基类接口显示两类公共信息）、T3（前三次功能全部回归）、健壮性用例（重复编号、非法类型、空字段）。

### 后续迭代预留

- 第 5 次（虚函数、多态）：把 `activitySpecificInfo()` 的类型分支改为基类虚函数 `virtual std::string specificInfo() const`，由派生类 `override`；报名规则差异（费用、资格）也用虚函数实现。
- 第 6 次（STL 算法、Lambda）：活动排序与组合查询，可基于现有 `vector` 与查询函数直接扩展。
- 第 7 次（文件流、异常）：在 `ActivityManager` 中增加保存/读取，菜单末尾追加 `7. 保存 / 读取`。

---

## 7. 测试记录

测试数据在 `test\*.in`，完整运行结果在 `test\*.out`（由 `run_tests.bat` 生成，可复现）。

### T1 分别创建 Lecture 和 Competition 并显示实际类型 —— `test/T1_types.in`

操作路径：发布活动 → 类型 1 讲座（A01）→ 类型 2 竞赛（A02）→ 浏览全部活动。

实际输出（节选）：

```
  [A01] AI 讲座 | 类型: 讲座 | 时间: 2026-03-01 14:00 | 地点: 一教 101 | 容量: 30 | 已报名: 0 | 主讲人: 张老师
  [A02] 校园篮球赛 | 类型: 竞赛 | 时间: 2026-03-05 09:00 | 地点: 体育馆 | 容量: 20 | 已报名: 0 | 参赛要求: 限身高 175cm 以上，需自备球鞋
```

结论：两个活动被创建为不同的派生类型，列表能显示实际类型名（讲座 / 竞赛）与各自的特有信息。

### T2 用基类接口显示两类活动的公共信息 —— `test/T2_base_interface.in`

先创建 2 个用户，再分别创建讲座、竞赛、体育 3 个活动，用“浏览全部活动”统一显示。
该路径上程序遍历的是 `vector<Activity*>`，统一调用基类的 `describeCommon()`：

```
---- 活动列表（共 3 个）----
  [A01] AI 讲座 | 类型: 讲座 | 时间: 2026-03-01 14:00 | 地点: 一教 101 | 容量: 2 | 已报名: 0 | 主讲人: 张老师
  [A02] 校园篮球赛 | 类型: 竞赛 | 时间: 2026-03-05 09:00 | 地点: 体育馆 | 容量: 5 | 已报名: 0 | 参赛要求: 限身高 175cm 以上
  [A03] 羽毛球训练 | 类型: 体育 | 时间: 2026-03-08 16:00 | 地点: 风雨操场 | 容量: 10 | 已报名: 0 | 体育项目: 羽毛球
```

结论：同一基类接口可以显示所有类型的公共字段（编号、类型、名称、时间、地点、容量、已报名人数）。

### T3 原有普通活动的创建、查询仍可使用（前三次功能回归）—— `test/T3_regression.in`

一个连续会话覆盖：非法/空菜单输入、创建用户（含重复编号）、容量非法重试（0、`abc`）、
报名（成功/重复/满员/用户不存在）、取消报名（成功/未报名/活动不存在）、
双向查询（含 A99 不存在）、按类型浏览（无该类型提示）、统计一致性、删除保护（活动/用户）、
修改用户（编号不变）。关键实际输出：

```
请选择操作编号: 无效的菜单编号：abc，请重新输入。
创建失败：用户编号已存在：U02（编号必须唯一）。
活动容量: 输入无效：容量必须是正整数（大于 0）。
活动容量: 输入格式错误：请输入一个正整数。
报名成功：活动 A01 当前已报名 1/2 人。
报名成功：活动 A01 当前已报名 2/2 人。
报名失败：该用户已经报名活动 A01，不能重复报名。
报名失败：用户不存在：U99
报名成功：活动 A02 当前已报名 1/3 人。
报名失败：活动 A01 已满员（容量 2）。
取消成功：活动 A01 当前已报名 1/2 人。
取消失败：用户 U02 没有报名活动 A01。
取消失败：活动不存在：A99
活动 A01（AI 讲座）的报名用户共 1 人: U03(王同学)
活动不存在：A99
用户 U03（王同学）已报名 2 个活动:
用户 U02 目前没有报名任何活动（空）。
  讲座类活动: 1 个
  竞赛类活动: 1 个
  体育类活动: 0 个
  活动总数: 2 个；用户总数: 3 人
    [A01] AI 讲座 已报名 1/2，报名记录 1 条
    [A02] 编程竞赛 已报名 1/3，报名记录 1 条
当前没有体育类活动。
删除失败：活动 A01 已有 1 人报名，请先取消报名。
删除失败：用户 U03 已报名活动 A01，请先取消报名。
修改成功：[U02] 李同学 | 联系方式: 13811112222
用户 U02 已删除。
修改失败：容量不能小于已报名人数 2（未做任何修改）。
修改成功：[A02] 编程竞赛（调整） | 类型: 竞赛 | 时间: 2026-03-10 09:00 | 地点: 实验楼 201 | 容量: 3 | 已报名: 2
删除失败：活动 A02 已有 2 人报名，请先取消报名。
取消成功：活动 A02 当前已报名 1/3 人。
取消成功：活动 A02 当前已报名 0/3 人。
活动 A02 已删除。
```

结论：Lab01～Lab03 的创建、显示、修改（用户 + 活动，编号不变）、报名、取消、双向查询、删除保护全部正常，
第 4 次的改法没有破坏原有功能；统计里“已报名人数”与“报名记录条数”始终一致。

### 附加：健壮性用例 —— `test/robustness.in`

覆盖：空活动编号、非法类型编号、容量输入 `abc`/`0`、主讲人留空、重复活动编号、空活动列表。

```
输入无效：活动编号不能为空，本次操作已取消。
无效的活动类型编号：9，本次操作已取消。
输入格式错误：请输入一个正整数。
输入无效：容量必须是正整数（大于 0）。
输入无效：主讲人不能为空，本次操作已取消。
当前没有活动记录。
创建失败：活动编号已存在：A10（编号必须唯一）。
```

结论：非法菜单项、空输入、格式错误都不会让程序退出，也不会留下半成品数据。

### 静态检查与内存检查

| 检查项 | 方法 | 结果 |
| --- | --- | --- |
| 编译告警 | `g++ -std=c++17 -Wall -Wextra -Wpedantic` | 0 warning / 0 error |
| 容器越界 | `-D_GLIBCXX_ASSERTIONS` 重新编译并跑全部用例 | 输出与正式版本逐字节一致 |
| 内存泄漏 | `tools/leak_probe.cpp` 统计 new/delete 后仍存活的对象 | T1/T2/T3/健壮性 4 次运行 `outstanding=0` |

---

## 8. 已知限制

1. 数据只保存在内存中，退出即丢失（第 7 次上机实现文件保存/读取）。
2. 报名与取消报名都按“用户编号 + 活动编号”操作，没有做用户登录与权限区分（任务书不要求）。
3. 显示为控制台文本界面，未使用图形界面（任务书允许）。
4. 活动的**特有信息**（主讲人 / 参赛要求 / 体育项目）在发布时确定，本版本不支持修改；
   第 2 次要求的“修改活动信息”指名称、举办时间、地点、容量这些公共字段。
5. 活动类型判断目前用“类型标识 + 向下转型”实现，第 5 次上机会改为虚函数以体现运行时多态。

---

## 9. 大模型使用记录

| 项目 | 内容 |
| --- | --- |
| 是否使用 | 使用 |
| 工具 | DeepSeek Harness（AI 编程助手，本机 Web 界面） |
| 用途 | ① 帮助梳理任务书四次上机的功能点与验收用例；② 生成类设计初稿和工程骨架（`User`/`Activity` 体系、`ActivityManager`、输入工具）；③ 生成 T1～T3 测试脚本与健壮性用例 |
| 关键提示词 | “按任务书方向 A，用基类 Activity 提取编号/名称/时间/地点/容量，至少两个派生类保存特有信息，报名规则必须在基类”；“所有控制台输入用统一工具解析，非法输入不得退出程序或写入非法数据”；“为第 4 次上机写 T1/T2/T3 测试脚本，覆盖重复报名、满员、不存在编号” |
| 采纳与修改 | 采纳基类/派生类划分与“报名记录只存用户编号”的设计；删除了最初建议的 `Result` 结果类，改为 `bool + std::string& message` 的简单返回；删除成员函数中的多余向上转型；把 `virtual` 只保留在析构函数上，避免提前使用第 5 次的虚函数知识点；补充“有报名记录禁止删除”的规则 |
| 自行验证 | 逐条对照任务书功能点；用 `-Wall -Wextra -Wpedantic` 编译至零告警；运行 T1/T2/T3 与健壮性用例核对输出；用 `tools/leak_probe.cpp` 确认 `outstanding=0`；现场验收前自己重跑 `run_tests.bat` 并复核 `test/*.out` |

> 提交前请按自己的实际情况核对上表（工具名、提示词、改动点），验收时会要求解释代码，
> 建议重点准备：`Activity::registerUser` 的三个返回值、`registrants_` 为什么存编号、
> `describeCommon()` 与 `activitySpecificInfo()` 的分工、`ActivityManager` 的所有权与析构。
