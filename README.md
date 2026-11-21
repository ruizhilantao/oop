# 校园活动组织与协同平台（项目方向 A · 第 1～5 次上机合并版）

面向对象程序设计与构造 C++ 上机实验作业。
本工程按**第 5 次上机（Lab05 不同活动规则）的终态**提交：一个可独立编译、可运行的完整工程，
包含 Lab01～Lab05 全部已验收功能；第 5 次新增的差异化报名规则通过**虚函数**实现。

- 语言标准：C++11 及以上（工程按 C++17 编译，代码本身只用到 C++11 特性）
- 编译器验证：MinGW-w64 g++ 16.2.0，`-std=c++17 -Wall -Wextra -Wpedantic` **零警告、零错误**
- 测试情况：T1～T4 四个验收用例 + 1 个健壮性用例全部通过，内存泄漏探针检测 `outstanding=0`

历史版本：仓库内用 git tag 保留各次终态（`lab04` 为第 4 次版本），随时可用
`git checkout lab04` 还原。

---

## 1. 目录结构

```
CampusActivityLab04/
├─ src/                      源码（.h 与 .cpp 分离）
│  ├─ main.cpp               菜单、输入提示、结果显示（界面层）
│  ├─ ActivityManager.h/.cpp 数据仓库与业务规则（用户、活动、报名协作）
│  ├─ Activity.h/.cpp        活动基类：公共字段 + 报名记录 + 虚函数接口
│  ├─ Lecture.h/.cpp         派生类：讲座（规则：剩余容量，免费）
│  ├─ Competition.h/.cpp     派生类：竞赛（规则：参赛等级 + 报名费）
│  ├─ Sport.h/.cpp           派生类：体育（规则：人数上限 + 固定费用）
│  ├─ User.h/.cpp            用户类（含参赛等级，竞赛资格检查的依据）
│  └─ Input.h/.cpp           控制台输入工具（读行、去空白、整数与区间解析）
├─ test/
│  ├─ T1_lecture_rule.in             T1：讲座报名与讲座规则
│  ├─ T2_competition_rule.in         T2：竞赛资格/费用与重复检查一致性
│  ├─ T3_polymorphic_collection.in   T3：三类对象放同一集合统一调用规则
│  ├─ T4_regression.in               T4：Lab01～Lab04 功能回归
│  ├─ robustness.in                  附加：非法输入 / 空输入 / 越界等级
│  └─ *.out                          上述用例的实际运行结果
├─ tools/leak_probe.cpp      本地自测用 new/delete 统计（不参与正式编译）
├─ build.bat                 Windows 一键编译
├─ run_tests.bat             批量运行测试并生成 test/*.out
├─ CMakeLists.txt            CLion / VS Code 等 IDE 使用
└─ README.md                 本文件（含类设计说明、变更说明、测试记录）
```

> 关于“保留 Lab01～Lab08 八个可区分版本”：本仓库用 **git tag** 区分各次终态
> （`lab04` = 第 4 次，`lab05` = 第 5 次），比复制目录更省空间且便于对比差异
> （`git diff lab04 lab05`）。各次增量内容在第 6 节逐条列出。

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
| Lab01 | 创建并显示用户、列表显示全部用户 | `User`、`ActivityManager::addUser`、`createUser/showUserList` |
| Lab01 | 创建并显示活动（编号/名称/时间/地点/容量/报名人数） | `Activity::describeCommon`、`createActivity` |
| Lab01 | 编号非空且唯一，容量为正整数 | `ActivityManager::addUser/addActivity`、`input::readPositiveInt` |
| Lab02 | 修改用户信息，编号不变 | `User::setName/setContact/setLevel`、`modifyUser` |
| Lab02 | 修改活动信息（名称/时间/地点/容量），编号不变 | `Activity::updateBasicInfo`、`modifyActivity` |
| Lab02 | 报名、重复报名拦截、满员拦截、人数 +1 | `registerForActivity`、`Activity::addRegistrant` |
| Lab02 | 取消报名、人数 -1、不能小于 0 | `Activity::unregisterUser`、`cancelRegistration` |
| Lab03 | 记录用户报了什么、活动有谁报名 | `Activity::registrants_`（唯一数据来源） |
| Lab03 | 双向查询，空时明确显示 | `findActivitiesOfUser`、`getRegistrants` |
| Lab03 | 人数与实际记录一致 | 人数由 `registrants_.size()` 计算，不另设计数器 |
| Lab03 | 有报名关系时拒绝删除并说明原因 | `removeUser`、`removeActivity` |
| Lab04 | 基类 `Activity` 提取公共字段，≥2 个派生类 | `Activity` + `Lecture`/`Competition`/`Sport` |
| Lab04 | 创建时可选类型，显示各类型特有信息 | `createActivityByType` + 虚函数 `specificInfo()` |
| Lab04 | 列表显示类型及完整信息 | `describeCommon()` + `specificInfo()` |
| Lab04 | 只查看某一类型，无该类型时提示 | 动态类型列表 + `findActivitiesByTypeName` |
| Lab04 | 各类型都能查询/报名/取消报名 | 逻辑在基类，派生类自动复用 |
| **Lab05** | 各类型有不同报名规则并能显示 | 虚函数 `ruleText()` / `checkRegistration()`（三类各自实现） |
| **Lab05** | 讲座检查剩余容量 | 基类 `checkRegistration()` 的容量检查（讲座直接复用） |
| **Lab05** | 竞赛设置参赛资格或费用 | `Competition`：`requiredLevel_` + `fee_`，资格检查用户等级 |
| **Lab05** | 体育设置人数限制或固定费用 | `Sport`：容量（人数上限）+ `fee_` |
| **Lab05** | 报名自动采用本类型规则并给出允许/拒绝与原因 | `registerForActivity` 调用基类指针的虚函数，拒绝时只返回结果 |
| **Lab05** | 显示费用与资格结果、重复检查结果一致 | `describeCheckResult()`；`previewRegistration()` 只读、确定性 |

**关于主菜单编号**：任务书“最终界面”的 6 是“保存/读取”（第 7 次上机内容）。
本版本按“后续实验只增加、不重排”的原则，把用户管理放在 1，第 7 次直接在末尾追加
`7. 保存 / 读取` 即可，不需要重新设计菜单：

```
1. 用户管理   2. 活动管理   3. 浏览活动
4. 报名       5. 取消报名   6. 查询 / 统计      0. 退出
```

---

## 4. 操作说明

**主菜单**：1 用户管理、2 活动管理、3 浏览活动、4 报名、5 取消报名、6 查询/统计、0 退出。
每个子菜单的 `0` 都是“返回上一级”，非法编号或空回车只提示错误、不退出程序、不改动数据。

| 菜单 | 子操作 | 说明 |
| --- | --- | --- |
| 1 用户管理 | 1 新建 / 2 修改 / 3 显示全部 / 4 删除 | 编号不可改；**参赛等级 1~10**（直接回车默认 1，修改时回车表示保留） |
| 2 活动管理 | 1 发布新活动 | 先选类型：讲座填主讲人；竞赛填参赛要求 + 最低等级 + 报名费；体育填项目 + 固定费用 |
| | 2 修改活动信息 | 可改名称/时间/地点/容量，编号不变；容量不能小于已报名人数 |
| | 3 删除活动 | 有报名记录时拒绝删除并说明原因 |
| 3 浏览活动 | 1 全部活动 | 每个活动显示公共信息 + 特有信息 + **本类型报名规则** |
| | 2 只查看某一类 | 类型列表由各活动的 `typeName()` 动态汇总，选编号即可 |
| 4 报名 | 用户编号 + 活动编号 | 自动套用该活动类型的规则，输出允许/拒绝、能力范围、资格、费用、原因 |
| 5 取消报名 | 用户编号 + 活动编号 | 没有报名记录时提示失败，集合不变 |
| 6 查询/统计 | 1 活动报名用户 / 2 用户报名活动 / 3 按类型统计 | 空结果明确显示“（空）” |
| | 4 **报名前试算** | 只做规则检查（资格、费用），**不写任何记录**，可反复调用核对一致性 |

数据在程序启动时为空，全部通过菜单录入（不预置写死的验收结果）。

---

## 5. 类设计说明

### 5.1 类与职责

| 类 | 职责 | 主要属性 | 主要成员函数 |
| --- | --- | --- | --- |
| `User` | 用户，保管编号/姓名/联系方式/参赛等级 | `id_` `name_` `contact_` `level_`（private） | `getId/getName/getContact/getLevel`、`setName/setContact/setLevel`、`describe` |
| `Activity` | 活动基类：公共字段 + 报名记录 + 公共容量规则 | `id_` `name_` `time_` `place_` `capacity_`（**protected**）、`registrants_`（private） | 纯虚 `typeName/specificInfo/ruleText`、虚 `checkRegistration`、`addRegistrant`、`unregisterUser`、`describeCommon`、`updateBasicInfo` |
| `Lecture` | 讲座：主讲人；规则=剩余容量、免费 | `speaker_` | 三个纯虚函数实现（**不重写** `checkRegistration`，复用基类容量检查） |
| `Competition` | 竞赛：参赛要求、最低等级、报名费 | `requirement_` `requiredLevel_` `fee_` | 三个纯虚函数实现 + **重写** `checkRegistration`（容量+资格+费用） |
| `Sport` | 体育：项目名、固定费用 | `sportName_` `fee_` | 三个纯虚函数实现 + **重写** `checkRegistration`（容量+费用） |
| `ActivityManager` | 持有全部对象，完成查找与用户↔活动协作，保证数据一致 | `users_`、`activities_`（`vector<Activity*>`） | 用户/活动增删改查、`registerForActivity`、`previewRegistration`、`cancelRegistration`、`findActivitiesByTypeName` |
| `RegistrationCheck` | 一次报名检查的结果（允许/拒绝、原因、费用、资格、规则） | `allowed` `fee` `rule` `qualification` `reason` | 由各派生类填充，`describeCheckResult()` 统一转成文本 |
| `input`（命名空间） | 统一控制台输入，保证格式错误不破坏数据 | — | `readLine/trim/isBlank/parseInt/readInt/readIntInRange/readPositiveInt` |

### 5.2 虚函数与多态（第 5 次重点）

| 虚函数 | 基类 `Activity` | `Lecture` | `Competition` | `Sport` |
| --- | --- | --- | --- | --- |
| `typeName()` | **纯虚** | `"讲座"` | `"竞赛"` | `"体育"` |
| `specificInfo()` | **纯虚** | 主讲人 | 参赛要求 + 最低等级 + 费用 | 体育项目 + 固定费用 |
| `ruleText()` | **纯虚** | 只检查剩余容量，免费 | 等级 ≥ N，报名费 M 元 | 人数上限 C 人，固定费用 M 元 |
| `checkRegistration(user)` | 实现：满员则拒绝，否则允许、费用 0 | 复用基类（只判容量） | **override**：容量 → 资格 → 费用 | **override**：容量 → 固定费用 |

- **多态怎么被触发**：`ActivityManager` 只持有 `vector<Activity*>`。报名时执行
  `activity->checkRegistration(*user)`，同一行代码对三种活动分别执行三套规则；
  显示时执行 `activity->typeName()/specificInfo()/ruleText()`，也不需要知道具体类型。
- **为什么规则不会重复实现**：`Competition`、`Sport` 的 `checkRegistration()` 第一句都是
  `RegistrationCheck check = Activity::checkRegistration(user);`，把“容量是否已满”这一公共规则
  留在基类，派生类只追加自己的部分。
- **为什么“无集中式类型判断”**：第 4 次版本里有一个 `switch (type)` 的 `activitySpecificInfo()`
  和 `ActivityType` 枚举；第 5 次全部删除，改为虚函数。现在整个 `src/` 里没有任何
  `switch/enum` 形式的类型分支，只有**创建活动时**按用户选择 `new` 对应派生类（工厂），
  那是“造对象”，不是“对已存在对象判断类型”。
- **规则检查是只读的**：`checkRegistration()` 声明为 `const`，只读取字段并返回结果，
  不修改任何数据；因此“相同输入重复检查结果一致”是天然的，拒绝报名时人数也必然不变。
- **析构必须是虚函数**：对象由基类指针统一 `delete`，否则派生类成员不会释放。

### 5.3 关系

```
ActivityManager ──组合──> User（vector<User>，值语义）
                ──组合──> Activity*（vector<Activity*>，多态持有并负责 delete）
Activity（基类，含 3 个纯虚函数 + 1 个虚函数）
   ├── Lecture      （规则：剩余容量，免费）
   ├── Competition  （规则：参赛等级 + 报名费）
   └── Sport        （规则：人数上限 + 固定费用）
```

报名记录只存用户编号（`vector<string>`），避免用户对象销毁后野指针，并让
“活动有谁报名”与“用户报了哪些活动”共用一份数据，人数与记录天然一致。

---

## 6. 五次迭代的变更说明（增量开发记录）

> 每次都在同一工程、同一批类上继续扩展，没有写成互相独立的两套程序。

### Lab01 → Lab02：封装、构造、const
`User` 增加 `setName/setContact`；`Activity` 增加 `capacity_`、`updateBasicInfo`、报名集合与
`addRegistrant/unregisterUser`；`ActivityManager` 增加 `modifyUser`、`modifyActivity`、
`registerForActivity`、`cancelRegistration`；菜单增加“修改用户/活动”“报名/取消报名”。
影响：字段全部 private；改容量新增“不得小于已报名人数”约束。验证：T4 回归用例。

### Lab02 → Lab03：组合、vector、对象协作
`Activity::registrants_` 成为报名关系唯一数据来源；新增 `getRegistrants`、`hasRegistrant`、
`findActivitiesOfUser`、`removeUser/removeActivity`；菜单增加“查询/统计”和两个删除。
影响：报名人数不再单独存储，改为由集合大小算出，消除了“人数与记录不符”的可能。
验证：T4 中“查询 A01 报名用户”“用户报名活动为空”“统计里人数与记录条数一致”“删除保护”。

### Lab03 → Lab04：继承、派生类、protected
把 `Activity` 拆成基类 + `Lecture`/`Competition`/`Sport`；公共字段上移到基类并设为
`protected`；新增 `ActivityType` 枚举、按类型浏览/统计；`createActivity` 改为“先选类型”。
影响：`activities_` 改为保存基类指针，基类析构函数改为 `virtual`。
验证：T2/T3（当时）与本次 T4 回归。

### Lab04 → Lab05：虚函数、多态（本次新增）
- **修改位置**：
  1. `Activity` 删除类型枚举与按类型 switch，改为 `typeName()`、`specificInfo()`、`ruleText()`
     三个纯虚函数，并新增虚函数 `checkRegistration(const User&)`；
  2. `Competition` 增加 `requiredLevel_`、`fee_` 并重写 `checkRegistration()`；
     `Sport` 增加 `fee_` 并重写 `checkRegistration()`；`Lecture` 复用基类容量规则；
  3. 新增 `RegistrationCheck` 结构与 `describeCheckResult()`，统一“允许/拒绝 + 原因 + 费用 + 资格”；
  4. `User` 增加 `level_`（1~10）与 `setLevel()`，作为竞赛资格检查依据；
  5. `ActivityManager` 新增 `previewRegistration()`（试算，只读）、`activityTypeNames()`、
     `findActivitiesByTypeName()`；报名流程改为“虚函数检查 → 通过才写记录”；
  6. 菜单：查询/统计增加“4. 报名前试算”；浏览活动显示“报名规则”；分类浏览改为动态类型列表。
- **原因**：第 5 次要求不同类型活动有不同报名规则，并把规则检查放到多态里，消除集中式类型判断。
- **影响范围**：报名判定从“管理器里写死容量判断”变成“各派生类自查”；`registerForActivity`
  在拒绝时不再修改任何数据；活动创建输入多了等级/费用；用户创建多了等级字段。
  原有查询、取消、删除、统计等逻辑不受影响（多态接口内部复用基类实现）。
- **验证方法**：T1（讲座规则与满员拒绝）、T2（竞赛资格不足拒绝 + 重复试算结果一致 +
  资格通过后成功并显示 50 元）、T3（三类对象同集合，同一函数输出三套规则）、T4（Lab01～04 全回归）、
  健壮性（非法类型/空字段/容量越界/等级越界）。

### 后续迭代预留
- 第 6 次（STL 算法、Lambda）：活动排序、组合查询，可基于现有 `vector` 与查询函数扩展；
  `activityTypeNames()` 里的手写去重可直接换成 `std::set`/`std::map`。
- 第 7 次（文件流、异常）：在 `ActivityManager` 增加保存/读取，菜单末尾追加 `7. 保存 / 读取`。

---

## 7. 测试记录

测试数据在 `test\*.in`，完整运行结果在 `test\*.out`（由 `run_tests.bat` 生成，可复现）。

### T1 讲座报名输出讲座规则结果 —— `test/T1_lecture_rule.in`

创建 2 个用户（等级 3 / 1）、2 个讲座（容量 2 / 1），U01 报名 A01、U02 报名 A02、U01 再报名 A02。

```
  [A01] AI 讲座 | 类型: 讲座 | ... | 容量: 2 | 已报名: 0 | 主讲人: 张老师
     报名规则: 讲座：只检查剩余容量，免费参加
报名成功：允许报名 | 规则: 讲座：只检查剩余容量，免费参加 | 资格: 无需资格检查 | 费用: 0 元 | 活动 A01 当前已报名 1/2 人。
报名成功：允许报名 | 规则: 讲座：只检查剩余容量，免费参加 | 资格: 无需资格检查 | 费用: 0 元 | 活动 A02 当前已报名 1/1 人。
报名失败：拒绝报名 | 规则: 讲座：只检查剩余容量，免费参加 | 资格: 无需资格检查 | 费用: 0 元 | 原因: 名额已满：活动 A02 已报名 1/1 人
```

结论：讲座按“剩余容量”规则判定；允许时显示规则与 0 元费用，拒绝时给出具体原因且人数不变。

### T2 竞赛报名输出不同的资格/费用规则结果 —— `test/T2_competition_rule.in`

U01（等级 1）、U02（等级 3）；竞赛 A02：最低等级 2、报名费 50 元。
先对 U01 试算两次，再让 U02 报名、U01 报名。

```
检查结果：拒绝报名 | 规则: 竞赛：参赛等级不低于 2 级，报名费 50 元 | 资格: 资格不通过 | 费用: 0 元 | 原因: 参赛等级不足：活动要求 2 级，用户 U01 为 1 级
检查结果：拒绝报名 | 规则: 竞赛：参赛等级不低于 2 级，报名费 50 元 | 资格: 资格不通过 | 费用: 0 元 | 原因: 参赛等级不足：活动要求 2 级，用户 U01 为 1 级
报名成功：允许报名 | 规则: 竞赛：参赛等级不低于 2 级，报名费 50 元 | 资格: 资格通过：用户等级 3 ≥ 要求 2 | 费用: 50 元 | 活动 A02 当前已报名 1/5 人。
报名失败：拒绝报名 | 规则: 竞赛：参赛等级不低于 2 级，报名费 50 元 | 资格: 资格不通过 | 费用: 0 元 | 原因: 参赛等级不足：活动要求 2 级，用户 U01 为 1 级
```

结论：资格不足被拒并给出具体差距；资格通过时显示费用 50 元；两次试算结果逐字相同（3(2) 要求）；
拒绝后浏览显示 `已报名: 1`（只有 U02 那一条），说明拒绝未改变数据。

### T3 两类（三类）对象放入同一集合统一调用规则函数 —— `test/T3_polymorphic_collection.in`

讲座 A01、竞赛 A02、体育 A03 依次创建，U01 依次报名，最后查 U01 的报名。

```
     报名规则: 讲座：只检查剩余容量，免费参加
     报名规则: 竞赛：参赛等级不低于 2 级，报名费 50 元
     报名规则: 体育：人数上限 8 人，固定费用 20 元
报名成功：... | 费用: 0 元 | 活动 A01 当前已报名 1/10 人。
报名成功：... | 资格: 资格通过：用户等级 3 ≥ 要求 2 | 费用: 50 元 | 活动 A02 当前已报名 1/5 人。
报名成功：... | 费用: 20 元 | 活动 A03 当前已报名 1/8 人。
用户 U01（张同学）已报名 3 个活动:
```

结论：三个对象存在同一个 `vector<Activity*>` 里，浏览与报名都通过基类指针调用虚函数，
同一套调用顺序得到三套不同规则结果（0 元 / 50 元资格检查 / 20 元固定费用）。

### T4 Lab01～Lab04 功能回归 —— `test/T4_regression.in`

覆盖：非法/空菜单输入、重复用户编号、容量非法重试（0、`abc`）、报名（成功/重复/满员/用户不存在）、
取消（成功/未报名/活动不存在）、双向查询、A99 不存在、统计、删除保护、修改用户（含等级不变）、
修改活动（容量改小被拒、改回成功）、分类浏览、删除。

```
请选择操作编号: 无效的菜单编号：，请重新输入。
创建失败：用户编号已存在：U02（编号必须唯一）。
活动容量: 输入无效：请输入 1 ~ 2147483647 之间的整数。
报名失败：该用户已经报名活动 A01，不能重复报名。
报名失败：用户不存在：U99
报名失败：拒绝报名 | 规则: 讲座：只检查剩余容量，免费参加 | ... | 原因: 名额已满：活动 A01 已报名 2/2 人
取消失败：用户 U02 没有报名活动 A01。
取消失败：活动不存在：A99
活动不存在：A99
删除失败：活动 A01 已有 1 人报名，请先取消报名。
删除失败：用户 U03 已报名活动 A01，请先取消报名。
修改成功：[U02] 李同学 | 联系方式: 13811112222 | 等级: 2
用户 U02 已删除。
修改失败：容量不能小于已报名人数 2（未做任何修改）。
修改成功：[A02] 编程竞赛（调整） | 类型: 竞赛 | ... | 容量: 3 | 已报名: 2
删除失败：活动 A02 已有 2 人报名，请先取消报名。
活动 A02 已删除。
```

统计与分类：

```
---- 活动统计 ----
  讲座类活动: 1 个
  竞赛类活动: 1 个
  活动总数: 2 个；用户总数: 3 人
---- 讲座类活动（共 1 个）----
```

### 附加：健壮性用例 —— `test/robustness.in`

```
输入无效：活动编号不能为空，本次操作已取消。
无效的活动类型编号：9，本次操作已取消。
输入格式错误：请输入一个整数。
输入无效：请输入 1 ~ 2147483647 之间的整数。
输入无效：主讲人不能为空，本次操作已取消。
输入无效：参赛等级必须是 1 ~ 10 的整数，本次操作已取消。
当前没有活动记录。
```

结论：非法菜单项、空输入、格式错误、越界等级都不会让程序退出，也不会留下半成品数据。

### 静态检查与内存检查

| 检查项 | 方法 | 结果 |
| --- | --- | --- |
| 编译告警 | `g++ -std=c++17 -Wall -Wextra -Wpedantic` | 0 warning / 0 error |
| 容器越界 | `-D_GLIBCXX_ASSERTIONS` 重新编译并跑全部用例 | 输出与正式版本逐字节一致 |
| 内存泄漏 | `tools/leak_probe.cpp` 统计 new/delete 后仍存活的对象 | T1～T4 + 健壮性 5 次运行 `outstanding=0` |

---

## 8. 已知限制

1. 数据只保存在内存中，退出即丢失（第 7 次上机实现文件保存/读取）。
2. 报名与取消按“用户编号 + 活动编号”操作，没有登录与权限区分（任务书不要求）。
3. 控制台文本界面，未使用图形界面（任务书允许）。
4. 活动的**特有信息**（主讲人 / 参赛要求 / 最低等级 / 费用 / 体育项目）在发布时确定，本版本不支持修改；
   “修改活动信息”指名称、举办时间、地点、容量这些公共字段。
5. 报名费用只做展示，没有累计余额或支付流程（任务书只要求“显示本次报名费用”）。
6. 分类浏览的类型列表来自已有活动，因此不会出现“某类型一个活动都没有”的空列表；
   完全没有活动时提示“当前没有活动记录。”

---

## 9. 大模型使用记录

| 项目 | 内容 |
| --- | --- |
| 是否使用 | 使用 |
| 工具 | DeepSeek Harness（AI 编程助手，本机 Web 界面） |
| 用途 | ① 梳理第 5 次要求与验收用例（差异化规则、无集中式类型判断）；② 设计虚函数接口与 `RegistrationCheck` 结果结构；③ 改造代码、生成 T1～T4 与健壮性测试脚本 |
| 关键提示词 | “把第 4 次里按类型 switch 的 activitySpecificInfo 改成纯虚函数，代码里不能再有集中式类型判断”；“报名规则检查必须是 const、只读的，拒绝时不能改报名记录”；“三类活动放同一 vector<Activity*>，用同一行调用 checkRegistration 得到不同结果”；“生成测试脚本，覆盖资格不足、重复试算一致、满员拒绝后人数不变” |
| 采纳与修改 | 采纳“基类留容量检查 + 派生类追加规则”的分层方式与 `RegistrationCheck` 结构；把最初的 `std::function` 方案改回纯虚函数（符合本次知识点）；删除多余的 `ActivityType` 枚举；`previewRegistration()` 用 `const` 保证试算无副作用；测试脚本按实际提示词顺序逐行核对 |
| 自行验证 | 编译至零告警；逐条对照功能点与验收用例；运行 T1～T4 与健壮性用例核对输出；用泄漏探针确认 `outstanding=0`；用 `git diff lab04 lab05` 复核改动范围 |

> 提交前请按自己的实际情况核对上表（工具名、提示词、改动点），验收时可能要求解释：
> 为什么 `checkRegistration()` 要声明 `const`、派生类为什么先调 `Activity::checkRegistration()`、
> `RegistrationCheck` 里每个字段的含义、`vector<Activity*>` 如何触发多态。
