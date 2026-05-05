# Flappy Bird Qt

基于 Qt6 开发的 Flappy Bird 游戏。

## 游戏截图

![Flappy Bird](resources/readme_img/start.png)

## 已实现功能

### 核心玩法
- [x] 小鸟飞行控制（空格键上升，自然下落）
- [x] 管道生成与碰撞检测
- [x] 分数计算与显示
- [x] 最高分本地持久化存储

### 视觉效果
- [x] 30 FPS 游戏循环
- [x] 小鸟翅膀动画（8帧循环）
- [x] 背景无限滚动
- [x] 云朵飘动效果
- [x] 三种管道类型：普通管道、悬浮管道（中部悬挂）
- [x] 游戏欢迎界面与结束界面

### 音效
- [x] 飞行音效
- [x] 碰撞音效
- [x] 得分音效

### 技术特性
- [x] Qt 框架（QWidget + QPainter）
- [x] 单例模式（计分器、对象池）
- [x] 对象池模式（管道复用，避免频繁内存分配）
- [x] 难度递增（分数越高，移动管道出现概率越大）

## 下一步计划

- [ ] 实现移动管道（上下移动的水管）
- [ ] 云朵随机缩放效果
- [ ] 添加开始菜单
- [ ] 添加鼠标/触摸操控支持
- [ ] 添加游戏暂停功能
- [ ] 添加再来一局按钮点击响应

## 如何下载游玩

### 方法一：下载源码自行编译

**环境要求**
- Qt 6.x
- MinGW 64-bit 编译器

**编译步骤**

```bash
# 1. 克隆仓库
git clone https://github.com/你的用户名/flappybird-qt.git
cd flappybird-qt

# 2. 使用 Qt Creator 打开项目
# 打开 flappybird-qt.pro 文件

# 3. 在 Qt Creator 中点击"运行"即可编译并运行
```

### 方法二：下载预编译版本

前往 [Releases](https://github.com/你的用户名/flappybird-qt/releases) 页面下载最新版本的 `flappybird-qt.exe`，双击即可运行。

### 游戏操作

| 按键 | 功能 |
|------|------|
| 空格键 | 上升/开始游戏/重新开始 |
| 点击窗口 | 获得键盘焦点 |

### 游戏规则

- 点击空格键让小鸟上升
- 小鸟会在重力作用下自然下落
- 避开所有管道，存活越久分数越高
- 碰撞管道或地面则游戏结束
- 最高分会自动保存

## 项目结构

```
flappybird-qt/
├── main.cpp                 # 程序入口
├── game.h / game.cpp       # 游戏主窗口
├── constant.h / constant.cpp # 常量定义
├── bird.h / bird.cpp        # 小鸟类
├── pipe.h / pipe.cpp        # 水管类
├── pipepool.h / pipepool.cpp # 水管对象池
├── gamebackground.h/cpp     # 游戏背景
├── gameforeground.h/cpp     # 游戏前景(云朵)
├── gameelementlayer.h/cpp   # 游戏元素层
├── scorecounter.h/cpp       # 计分器
├── welcomanimation.h/cpp    # 欢迎动画
├── gameoveranimation.h/cpp  # 结束动画
└── resources/               # 图片和音效资源
    ├── img/                  # 图片资源
    └── wav/                  # 音效资源
```

## 技术栈

- **语言**: C++
- **框架**: Qt 6.x
- **编译器**: MinGW 64-bit
- **构建工具**: qmake

## 作者

bravver

## 许可证

MIT License
