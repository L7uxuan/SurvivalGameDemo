# SurvivalGameDemo

## 2D生存游戏 | 塔防 + 射击混合

[![GitHub stars](https://img.shields.io/github/stars/L7uxuan/SurvivalGameDemo.svg?style=flat-square)](https://github.com/L7uxuan/SurvivalGameDemo/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/L7uxuan/SurvivalGameDemo.svg?style=flat-square)](https://github.com/L7uxuan/SurvivalGameDemo/network)
[![Language](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=flat-square)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-2.6.1-green.svg?style=flat-square)](https://www.sfml-dev.org/)

> 一款融合了塔防与射击玩法的2D生存游戏  
> 控制步枪手，建造防御塔，在敌人潮中保护水晶核心

---

## 📖 目录

- [游戏特色](#-游戏特色)
- [操作说明](#-操作说明)
- [游戏截图](#-游戏截图)
- [快速开始](#-快速开始)
- [技术栈](#-技术栈)
- [项目结构](#-项目结构)
- [许可证](#-许可证)

---

## ✨ 游戏特色

 **混合玩法** —— 塔防 + 射击双核机制，策略与操作并重  
 **智能仇恨系统** —— 谁攻击敌人，敌人就会优先追击谁  
 **子弹反弹** —— 所有子弹碰到屏幕边缘可反弹，且最多反弹2次  
 **防御塔系统** —— 消耗能量建造自动攻击的防御塔，射速5发/秒  
 **复活机制** —— 玩家死亡3秒后在起点复活，清除仇恨  
 **波次预告** —— 每波来袭前屏幕中央显示预告，间隔5秒  
 **血包掉落** —— 敌人有5%几率掉落血包  
 **丰富特效** —— 近战挥砍、冲刺拖尾、伤害数字、血条系统  

---

## 🎮 操作说明

| 按键 | 功能 |
| :--- | :--- |
| `WASD` | 移动角色 |
| 鼠标左键 | 射击 |
| 鼠标右键 | 冲刺 |
| `X` | 建造防御塔/30能量 |
| `␣` 空格 | 暂停游戏 |
| `↵` 回车 | 推进剧情 |

---

## 📷 游戏截图

| 主菜单 | 剧情界面 | 游戏界面 |
| :---: | :---: | :---: |
| ![主菜单](screenshot1.png) | ![剧情界面](screenshot2.png) | ![游戏界面](screenshot3.png) |

---

## 🚀 快速开始

### 环境要求

 **操作系统**：Windows 10 / 11  
 **开发工具**：Visual Studio 2022  
 **图形库**：SFML 2.6.1  
 **编译器**：支持 C++17  

### 编译与运行

1. **克隆仓库**

   ```bash
   git clone https://github.com/L7uxuan/SurvivalGameDemo.git
2.**打开项目**

用 Visual Studio 2022 打开 SurvivalGame.sln

3.**配置 SFML**

确保 SFML 2.6.1 的 include 和 lib 路径已正确配置

4.**编译运行**

按 F5 编译并运行游戏
