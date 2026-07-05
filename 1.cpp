#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
using namespace std;

// ===================== 基类：Locker =====================
// 功能：储物柜基类，封装所有类型储物柜共有的属性和方法
class Locker {
private:
    int row, col;      // 储物柜的行号和列号，用于标识柜子位置
    bool isUsed;       // 柜子使用状态，true表示已占用，false表示空闲
    string user;       // 当前使用该柜子的用户名，空闲时为空字符串

public:
    // 功能：构造函数，初始化储物柜的基本信息
    // 参数r：柜子行号，参数c：柜子列号
    // 返回值：无
    // 实现细节：使用初始化列表将行号和列号赋值为传入参数，isUsed初始化为false表示空闲，user初始化为空字符串
    Locker(int r, int c) : row(r), col(c), isUsed(false), user("") {}

    // 功能：显示当前柜子的状态信息
    // 参数：无
    // 返回值：无
    // 实现细节：输出柜子编号（行号-列号），并根据isUsed的值输出“占用”或“空闲”
    void showStatus() {
        cout << "编号:" << row << "-" << col << " 状态:" << (isUsed ? "占用" : "空闲") << endl;
    }

    // 功能：获取柜子的使用状态
    // 参数：无
    // 返回值：bool类型，true表示已占用，false表示空闲
    // 实现细节：直接返回私有成员变量isUsed的值
    bool getIsUsed() {
        return isUsed;
    }

    // 功能：获取当前使用柜子的用户名
    // 参数：无
    // 返回值：string类型，返回使用者用户名，空闲时返回空字符串
    // 实现细节：直接返回私有成员变量user的值
    string getUser() {
        return user;
    }

    // 功能：设置柜子的使用者，并将柜子状态标记为已占用
    // 参数u：要设置的用户名
    // 返回值：无
    // 实现细节：将传入的用户名赋值给user，同时将isUsed设置为true
    void setUser(string u) {
        user = u;
        isUsed = true;
    }

    // 功能：清空柜子使用者信息，将柜子恢复为空闲状态
    // 参数：无
    // 返回值：无
    // 实现细节：将isUsed设置为false，user设置为空字符串
    void clearUser() {
        isUsed = false;
        user = "";
    }
};

// ===================== 普通柜：NormalLocker =====================
// 功能：普通储物柜类，继承自Locker，实现普通柜的存取逻辑（无需密码验证）
class NormalLocker : public Locker {
public:
    // 功能：构造函数，初始化普通柜
    // 参数r：行号，默认值为0；参数c：列号，默认值为0
    // 返回值：无
    // 实现细节：调用基类Locker的构造函数完成初始化
    NormalLocker(int r = 0, int c = 0) : Locker(r, c) {}

    // 功能：判断是否可以向该普通柜存入物品
    // 参数userName：尝试存物的用户名
    // 返回值：bool类型，柜子空闲返回true，已占用返回false
    // 实现细节：柜子已被占用且使用者不为空时返回false，否则返回true
    // 注意：该方法仅做条件判断，不修改柜子状态，状态修改由LockerManager调用setUser完成
    bool store(string userName) {
        if (getIsUsed() && getUser() != "")
            return false;
        return true;
    }

    // 功能：判断是否可以从该普通柜取出物品
    // 参数userName：尝试取物的用户名
    // 返回值：bool类型，柜子已被占用且用户名匹配返回true，否则返回false
    // 实现细节：柜子已被占用且传入用户名与柜子使用者相同时返回true，否则返回false
    // 注意：该方法仅做条件判断，不修改柜子状态，状态修改由LockerManager调用clearUser完成
    bool take(string userName) {
        if (getIsUsed() && userName == getUser())
            return true;
        return false;
    }
};

// ===================== VIP柜：VIPLocker =====================
// 功能：VIP储物柜类，继承自Locker，在普通柜基础上增加密码保护功能
class VIPLocker : public Locker {
private:
    string password;   // 柜子的密码，只有设置密码的用户知道，初始为空字符串

public:
    // 功能：构造函数，初始化VIP柜
    // 参数r：行号，默认值为0；参数c：列号，默认值为0
    // 返回值：无
    // 实现细节：调用基类Locker的构造函数，并将password初始化为空字符串
    VIPLocker(int r = 0, int c = 0) :Locker(r, c), password("") {}

    // 功能：设置或清空VIP柜密码
    // 参数pwd：要设置的密码，传入空字符串则清空密码
    // 返回值：无
    // 实现细节：
    //   1. 如果pwd为空字符串，则将password清空并返回
    //   2. 只有当当前密码为空且pwd长度为6且全部为数字字符时，才将pwd设为新密码
    //   3. 该设计确保密码只能被设置一次（从空变为非空），不能修改已存在的密码
    void setPassword(string pwd) {
        if (pwd == "")
        {
            password = "";
            return;
        }
        if (password == "" &&
            pwd.length() == 6 &&
            isdigit(pwd[0]) &&
            isdigit(pwd[1]) &&
            isdigit(pwd[2]) &&
            isdigit(pwd[3]) &&
            isdigit(pwd[4]) &&
            isdigit(pwd[5]))
            password = pwd;
    }

    // 功能：验证输入的密码是否正确
    // 参数pwd：用户输入的待验证密码
    // 返回值：bool类型，密码验证通过返回true，否则返回false
    // 实现细节：
    //   1. 首先检查pwd格式是否为6位纯数字，格式不正确则直接返回false
    //   2. 格式正确后，如果柜子当前密码为空（未设置密码），则直接返回true
    //   3. 如果柜子已有密码，则比较pwd与存储的password是否相等，相等返回true
    //   4. 这种设计允许空闲VIP柜可以被任意密码格式正确的用户使用
    bool verifyPassword(string pwd) {
        if (pwd.length() == 6 &&
            isdigit(pwd[0]) &&
            isdigit(pwd[1]) &&
            isdigit(pwd[2]) &&
            isdigit(pwd[3]) &&
            isdigit(pwd[4]) &&
            isdigit(pwd[5]))
        {
            if (password == "")
                return true;
            else if (password != "" && pwd == password)
                return true;
            else
                return false;
        }
        else
            return false;
    }

    // 功能：判断是否可以向该VIP柜存入物品
    // 参数userName：尝试存物的用户名，参数pwd：用户设置的密码
    // 返回值：bool类型，柜子空闲、用户名为空且密码验证通过返回true，否则返回false
    // 实现细节：柜子未被占用、使用者为空且密码验证通过时返回true，否则返回false
    // 注意：该方法仅做条件判断，不修改柜子状态，状态修改由LockerManager调用setUser和setPassword完成
    bool storeVIP(string userName, string pwd) {
        if (!getIsUsed() && getUser() == "" && verifyPassword(pwd))
            return true;
        return false;
    }

    // 功能：判断是否可以从该VIP柜取出物品
    // 参数userName：尝试取物的用户名，参数pwd：用户输入的密码
    // 返回值：bool类型，柜子已占用、用户名匹配且密码验证通过返回true，否则返回false
    // 实现细节：柜子已被占用、用户名匹配且密码验证通过时返回true，否则返回false
    // 注意：该方法仅做条件判断，不修改柜子状态，状态修改由LockerManager调用clearUser和setPassword完成
    bool takeVIP(string userName, string pwd) {
        if (getIsUsed() && userName == getUser() && verifyPassword(pwd))
            return true;
        return false;
    }
};

// ===================== 管理类：LockerManager =====================
// 功能：储物柜管理类，负责所有储物柜的集中管理和用户交互
class LockerManager {

private:
    // 普通柜二维数组，2行4列共8个柜子，编号对应1-1至2-4
    NormalLocker normalLockers[2][4];
    // VIP柜二维数组，1行4列共4个柜子，编号对应3-1至3-4
    VIPLocker vipLockers[1][4];

public:
    // 功能：构造函数，初始化所有储物柜对象
    // 参数：无
    // 返回值：无
    // 实现细节：
    //   1. 遍历normalLockers数组，为每个元素创建NormalLocker对象并赋予正确的行号列号
    //   2. 遍历vipLockers数组，为每个元素创建VIPLocker对象并赋予正确的行号列号
    //   3. 输出初始化完成提示信息
    LockerManager()
    {
        for (int i = 0;i < 2;i++)
            for (int j = 0;j < 4;j++)
                normalLockers[i][j] = NormalLocker(i + 1, j + 1);
        for (int i = 0;i < 1;i++)
            for (int j = 0;j < 4;j++)
                vipLockers[i][j] = VIPLocker(i + 3, j + 1);
        cout << "校园储物柜管理系统初始化完成！共12个柜子（普通柜8个、VIP柜4个），所有柜子处于空闲状态。" << endl;
    }

    // 功能：显示所有储物柜的状态信息
    // 参数：无
    // 返回值：无
    // 实现细节：
    //   1. 输出“【普通储物柜】”标题
    //   2. 遍历普通柜数组，输出每个柜子的编号和状态（根据getIsUsed返回值显示占用或空闲）
    //   3. 输出“【VIP储物柜】”标题
    //   4. 遍历VIP柜数组，输出每个柜子的编号和状态
    void showAllStatus() {
        cout << "【普通储物柜】" << endl;
        for (int i = 0;i < 2;i++)
            for (int j = 0;j < 4;j++)
                cout << "编号：" << i + 1 << "-" << j + 1 << " ,状态：" << (normalLockers[i][j].getIsUsed() ? "占用" : "空闲") << endl;
        cout << "【VIP储物柜】" << endl;
        for (int i = 0;i < 1;i++)
            for (int j = 0;j < 4;j++)
                cout << "编号：" << i + 3 << "-" << j + 1 << " ,状态：" << (vipLockers[i][j].getIsUsed() ? "占用" : "空闲") << endl;
    }

    // 功能：执行存物操作
    // 参数row：柜子行号，参数col：柜子列号，参数userName：用户名，参数pwd：VIP柜密码（普通柜可省略）
    // 返回值：bool类型，存物成功返回true，失败返回false
    // 实现细节：
    //   1. 检查行列号是否在有效范围内（行1-3，列1-4），无效则输出错误提示并返回false
    //   2. 检查用户名是否为空，为空则输出错误提示并返回false
    //   3. 根据行号判断柜子类型进行不同处理：
    //      a. 普通柜（row<=2）：检查柜子是否已被占用，已占用则提示失败；否则调用store判断，通过后调用setUser更新状态
    //      b. VIP柜（row==3）：检查柜子是否已被占用，已占用则提示失败；检查密码格式是否为6位纯数字；调用storeVIP判断，通过后调用setUser和setPassword更新状态
    //   4. 各种失败情况均有明确的错误提示
    bool storeItem(int row, int col, string userName, string pwd = "") {
        if (row < 1 || row > 3 || col < 1 || col > 4) {
            cout << "存物失败：柜子编号无效，行号范围为1-3，列号范围为1-4。" << endl;
            return false;
        }

        if (userName == "") {
            cout << "存物失败：用户名不能为空。" << endl;
            return false;
        }

        if (row <= 2) {
            if (normalLockers[row - 1][col - 1].getIsUsed()) {
                cout << "存物失败：该普通柜已被占用，当前用户为 " << normalLockers[row - 1][col - 1].getUser() << "。" << endl;
                return false;
            }

            if (normalLockers[row - 1][col - 1].store(userName)) {
                normalLockers[row - 1][col - 1].setUser(userName);
                return true;
            }
            else {
                return false;
            }
        }
        else if (row == 3) {
            if (vipLockers[row - 3][col - 1].getIsUsed()) {
                cout << "存物失败：该VIP柜已被占用，当前用户为 " << vipLockers[row - 3][col - 1].getUser() << "。" << endl;
                return false;
            }

            if (pwd.length() != 6 ||
                !isdigit(pwd[0]) || !isdigit(pwd[1]) || !isdigit(pwd[2]) ||
                !isdigit(pwd[3]) || !isdigit(pwd[4]) || !isdigit(pwd[5])) {
                cout << "存物失败：VIP密码必须为6位纯数字。" << endl;
                return false;
            }

            if (vipLockers[row - 3][col - 1].storeVIP(userName, pwd)) {
                vipLockers[row - 3][col - 1].setUser(userName);
                vipLockers[row - 3][col - 1].setPassword(pwd);
                return true;
            }
            else {
                return false;
            }
        }

        return false;
    }

    // 功能：执行取物操作
    // 参数row：柜子行号，参数col：柜子列号，参数userName：用户名，参数pwd：VIP柜密码（普通柜可省略）
    // 返回值：bool类型，取物成功返回true，失败返回false
    // 实现细节：
    //   1. 检查行列号是否在有效范围内（行1-3，列1-4），无效则输出错误提示并返回false
    //   2. 检查用户名是否为空，为空则输出错误提示并返回false
    //   3. 根据行号判断柜子类型进行不同处理：
    //      a. 普通柜（row<=2）：检查柜子是否空闲，空闲则提示失败；检查用户名是否匹配；调用take判断，通过后调用clearUser恢复空闲状态
    //      b. VIP柜（row==3）：检查柜子是否空闲；检查用户名是否匹配；检查密码格式；调用takeVIP判断，通过后调用clearUser和setPassword("")恢复空闲状态并清空密码
    //   4. 各种失败情况均有明确的错误提示
    bool takeItem(int row, int col, string userName, string pwd = "") {
        if (row < 1 || row > 3 || col < 1 || col > 4) {
            cout << "取物失败：柜子编号无效，行号范围为1-3，列号范围为1-4。" << endl;
            return false;
        }

        if (userName == "") {
            cout << "取物失败：用户名不能为空。" << endl;
            return false;
        }

        if (row <= 2) {
            if (!normalLockers[row - 1][col - 1].getIsUsed()) {
                cout << "取物失败：该普通柜当前为空闲状态，无需取物。" << endl;
                return false;
            }

            if (userName != normalLockers[row - 1][col - 1].getUser()) {
                cout << "取物失败：用户名不匹配，该柜子属于 " << normalLockers[row - 1][col - 1].getUser() << "。" << endl;
                return false;
            }

            if (normalLockers[row - 1][col - 1].take(userName)) {
                normalLockers[row - 1][col - 1].clearUser();
                return true;
            }
            else {
                return false;
            }
        }
        else if (row == 3) {
            if (!vipLockers[row - 3][col - 1].getIsUsed()) {
                cout << "取物失败：该VIP柜当前为空闲状态，无需取物。" << endl;
                return false;
            }

            if (userName != vipLockers[row - 3][col - 1].getUser()) {
                cout << "取物失败：用户名不匹配，该柜子属于 " << vipLockers[row - 3][col - 1].getUser() << "。" << endl;
                return false;
            }

            if (pwd.length() != 6 ||
                !isdigit(pwd[0]) || !isdigit(pwd[1]) || !isdigit(pwd[2]) ||
                !isdigit(pwd[3]) || !isdigit(pwd[4]) || !isdigit(pwd[5])) {
                cout << "取物失败：VIP密码必须为6位纯数字。" << endl;
                return false;
            }

            if (vipLockers[row - 3][col - 1].takeVIP(userName, pwd)) {
                vipLockers[row - 3][col - 1].clearUser();
                vipLockers[row - 3][col - 1].setPassword("");
                return true;
            }
            else {
                cout << "取物失败：密码错误。" << endl;
                return false;
            }
        }
    }

    // 功能：统计并显示空闲储物柜数量
    // 参数：无
    // 返回值：无
    // 实现细节：
    //   1. 定义count1、count2、count三个变量分别存储普通柜空闲数、VIP柜空闲数和总空闲数
    //   2. 遍历普通柜数组，对每个未被占用的柜子进行计数
    //   3. 遍历VIP柜数组，对每个未被占用的柜子进行计数
    //   4. 计算总空闲数并分类输出结果
    void countFree() {
        int count1{}, count2{}, count{};
        for (int i = 0;i < 2;i++)
            for (int j = 0;j < 4;j++)
                if (!normalLockers[i][j].getIsUsed())
                    count1++;
        for (int i = 0;i < 1;i++)
            for (int j = 0;j < 4;j++)
                if (!vipLockers[i][j].getIsUsed())
                    count2++;
        count = count1 + count2;
        cout << "普通柜空闲数量：" << count1 << endl;
        cout << "VIP柜空闲数量：" << count2 << endl;
        cout << "总计空闲数量：" << count << endl;
    }

    // 功能：显示系统主菜单
    // 参数：无
    // 返回值：无
    // 实现细节：
    //   1. 调用system("cls")清空控制台屏幕
    //   2. 输出菜单标题和各个选项（1查看状态、2存物、3取物、4统计、0退出）
    //   3. 提示用户输入操作编号
    void showMenu() {
        system("cls");
        cout << "\n===== 校园智能储物柜管理系统 =====" << endl;
        cout << "1. 查看所有储物柜状态" << endl;
        cout << "2. 存物品" << endl;
        cout << "3. 取物品" << endl;
        cout << "4. 统计空闲储物柜" << endl;
        cout << "0. 退出系统" << endl;
        cout << "请输入操作编号：";
    }
};

// ===================== 主函数 =====================
// 功能：程序入口，实现用户交互主循环
// 参数：无
// 返回值：int类型，程序正常结束时返回0
// 实现细节：
//   1. 创建LockerManager对象manager，自动完成所有储物柜的初始化
//   2. 定义choice、row、col整型变量存储用户输入，name和pwd字符串变量存储用户名和密码
//   3. 进入无限循环：
//      a. 调用manager.showMenu()显示菜单
//      b. 读取用户输入的choice
//      c. 根据choice进入不同分支：
//         - 1：调用showAllStatus()查看所有柜子状态
//         - 2：获取行列号和用户名，若为VIP柜则额外获取密码，调用storeItem执行存物，成功时输出“存物成功”
//         - 3：获取行列号和用户名，若为VIP柜则额外获取密码，调用takeItem执行取物，成功时输出“取物成功”
//         - 4：调用countFree()统计空闲柜子
//         - 0：return 0退出程序
//         - 其他：输出“输入无效，请重新选择”
//      d. 调用system("pause")暂停，等待用户按键后继续循环
int main() {
    LockerManager manager;
    int choice, row, col;
    string name, pwd;
    while (true) {
        manager.showMenu();
        cin >> choice;
        switch (choice) {
        case 1:
            manager.showAllStatus();
            break;

        case 2:
            cout << "请输入您存物柜子的行号，列号，以及您的用户名：";
            cin >> row >> col >> name;
            if (row == 3)
            {
                cout << "请输入VIP密码：";
                cin >> pwd;
            }

            if (manager.storeItem(row, col, name, pwd))
                cout << "存物成功！" << endl;

            break;

        case 3:
            cout << "请输入您取物所在柜子的行号，列号，以及您的用户名：";
            cin >> row >> col >> name;
            if (row == 3)
            {
                cout << "请输入密码：";
                cin >> pwd;
            }
            if (manager.takeItem(row, col, name, pwd))
                cout << "取物成功！" << endl;

            break;

        case 4:
            manager.countFree();
            break;

        case 0:
            return 0;

        default:
            cout << "输入无效，请重新选择" << endl;
            break;
        }
        system("pause");
    }
    return 0;
}