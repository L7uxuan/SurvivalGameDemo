<!-- 顶部徽章区域 -->
<div align="center">

  <!-- 项目Logo/标题区域 -->
  <img src="https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++17">
  <img src="https://img.shields.io/badge/SFML-2.6.1-8CC445?style=for-the-badge&logo=sfml&logoColor=white" alt="SFML 2.6.1">
  <img src="https://img.shields.io/badge/VS-2022-5C2D91?style=for-the-badge&logo=visualstudio&logoColor=white" alt="VS2022">
  <img src="https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Windows">

  <h1>🎮 生存游戏 - 守护水晶</h1>
  <p><strong>使用 C++17 与 SFML 开发的 2D 俯视角生存射击游戏</strong></p>

  <!-- 演示GIF占位 - 替换为你的实际游戏截图 -->
  <img src="docs/gameplay.gif" alt="游戏演示" width="600" style="border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.3);">
  <p><i>🎬 将游戏录屏或截图放入 <code>docs/</code> 文件夹替换此占位图</i></p>

  <br>

  <!-- 快速链接 -->
  <a href="#-功能特性">功能特性</a> •
  <a href="#-快速开始">快速开始</a> •
  <a href="#-游戏操作">游戏操作</a> •
  <a href="#-项目结构">项目结构</a> •
  <a href="#-技术栈">技术栈</a>

</div>

---

## ✨ 功能特性

<table>
<tr>
<td width="50%">

### 🎯 核心玩法
- **三波敌人攻势**：15 → 25 → 500，难度指数级递增
- **双类型敌人系统**：
  - 💗 **粉色近战兵**：高速追击，贴身攻击
  - ❤️ **红色自爆兵**：范围爆炸，群体伤害
- **智能仇恨机制**：攻击者成为敌人优先目标（支持玩家与防御塔）

</td>
<td width="50%">

### 🏗️ 策略系统
- **防御塔建造**：消耗 30 能量（蓝条）建造自动攻击塔
- **塔防协同**：防御塔拥有独立血条，可被敌人摧毁
- **子弹反弹系统**：玩家与防御塔子弹可弹射 2 次，提升战术深度
- **能量管理**：建造与技能消耗资源，考验决策时机

</td>
</tr>
</table>

### 📖 完整游戏流程
主菜单 → 角色选择 → 姓名输入 → 剧情引导 → 三波战斗 → 胜利/失败 → 返回菜单
plain
复制

---

## 🚀 快速开始

### 环境要求

| 项目 | 要求 |
|:---:|:---|
| **操作系统** | Windows 7 及以上 |
| **编译器** | Visual Studio 2022（或支持 C++17 的编译器）|
| **依赖库** | SFML 2.6.1（已包含在项目中）|

### 安装步骤

#### 1️⃣ 克隆仓库
```bash```
git clone https://github.com/你的用户名/survival-game.git
cd survival-game
#### 2️⃣ 打开项目
```bash```
# 使用 Visual Studio 2022 打开解决方案
SurvivalGame.sln
#### 3️⃣ 配置与编译
确保项目配置为 Release x64
按 Ctrl + Shift + B 编译
#### 4️⃣ 运行游戏
按 F5 启动游戏
⚠️ 提示：如果缺少 SFML 的 .dll 文件，请从 SFML 官网 下载，并将 bin 目录下的 sfml-*.dll 复制到生成目录（如 x64/Release/）。
🎮 游戏操作
### 操作指南

| 按键 | 功能说明 | 备注 |

|:---:|:---|:---|

| W A S D | 角色移动 | 八方向移动 |

| 鼠标左键 | 射击/攻击 | 步枪手专用 |

| 鼠标右键 | 冲锋位移 | 短暂无敌，可穿怪 |

| X | 建造防御塔 | 消耗30能量，有建造CD |

| 空格 | 暂停游戏 | 可查看当前状态 |

| Enter | 确认/继续 | 剧情推进、返回菜单 |
🛠️ 技术栈
<div align="center">
表格
类别	技术
编程语言	C++17
图形/音频/窗口	SFML 2.6.1 (Graphics, Window, Audio, System)
开发环境	Visual Studio 2022
版本控制	Git
</div>
📂 项目结构
plain
复制
SurvivalGame/
├── 📁 assets/                  # 游戏资源文件
│   ├── background.png         # 游戏战斗背景
│   ├── menu_bg.png            # 主菜单背景
│   ├── story_bg.png           # 剧情界面背景
│   ├── tower.png              # 防御塔贴图
│   ├── crystal.png            # 核心水晶贴图
│   ├── 📁 Rifleman/           # 玩家角色动画 (12帧 × 4方向)
│   ├── 📁 landmaster/         # 近战敌人动画 (18帧)
│   └── 📁 flyingmaster/       # 自爆敌人动画 (18帧)
├── 📁 src/                     # 源代码 (.h 头文件 与 .cpp 实现)
├── 📁 docs/                    # 文档与演示素材
├── SurvivalGame.sln           # Visual Studio 解决方案
└── README.md                  # 本文件 (项目说明)
📸 截图展示
<div align="center">
<!-- 在这里添加游戏截图，建议尺寸 800x450 -->
<img src="docs/screenshot1.png" alt="战斗画面" width="400">
<img src="docs/screenshot2.png" alt="菜单界面" width="400">
<p><i>📷 将截图放入 <code>docs/</code> 文件夹并更新上方路径</i></p>
</div>
📝 更新日志
表格
日期	版本	更新内容
2025.03	v1.0	完成核心玩法：三波敌人、防御塔系统、子弹反弹机制
2025.03	v1.1	新增完整 UI 流程：主菜单、剧情引导、姓名输入系统
2025.03	v1.2	优化敌人 AI 与仇恨系统，提升战斗流畅度
2025.03	v1.3	添加背景贴图与视觉特效，增强游戏表现力
🤝 贡献指南
欢迎提交 Issue 和 Pull Request！如果您想贡献代码，请遵循以下步骤：
Fork 本仓库
创建您的特性分支 (git checkout -b feature/AmazingFeature)
提交您的修改 (git commit -m 'Add some AmazingFeature')
推送到分支 (git push origin feature/AmazingFeature)
打开一个 Pull Request
📜 许可证
本项目基于 MIT License 开源。您可以自由使用、修改和分发，但请保留原始许可证声明。
🙏 致谢
SFML - 简洁高效的 C++ 多媒体库
游戏动画素材由本人绘制或来源于 OpenGameArt
感谢 CSDN 和 Stack Overflow 开发社区的技术分享
<div align="center">
⭐ 如果这个项目对您有帮助，欢迎点亮 Star 支持！
https://github.com/你的用户名/survival-game/stargazers
Made with ❤️ and C++
</div>
```
