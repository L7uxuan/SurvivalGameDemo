# SurvivalGameDemo

<p align="center">
  <a href="https://github.com/L7uxuan/SurvivalGameDemo">L7uxuan/SurvivalGameDemo</a>
  ·
  <a href="#构建">构建</a>
  ·
  <a href="#运行">运行</a>
  ·
  <a href="#技术">技术</a>
</p>

**2D塔防**  
控制角色，通过能量建造防御塔，抵御敌人，保护水晶核心。

> 独立项目


## 构建

**环境要求**
- Windows 10+
- Visual Studio 2022
- SFML 2.x（C++17）

```bash
git clone https://github.com/L7uxuan/SurvivalGameDemo
cd SurvivalGameDemo
# 用 VS2022 打开 SurvivalGame.sln
# 在 .vcxproj 中更新 SFML 的 include/lib 路径
# 编译（F5）
```



## 运行

编译成功后，在 Visual Studio 中运行。

如需手动运行，二进制文件位于：
- `x64/Debug/SurvivalGame.exe`
- `x64/Release/SurvivalGame.exe`

> 注意：运行前需要将 SFML 的 `.dll` 文件（如 `sfml-graphics-2.dll`）复制到 `.exe` 所在目录。


## 技术

| 层级 | 技术 |
|------|------|
| 语言 | C++17 |
| 图形 | SFML 2.6.1 |
| 构建 | VS2022 |
| 版本 | Git |



## 核心机制

| 系统 | 实现 |
|------|------|
| 仇恨系统 | 敌人记录攻击者，优先级：玩家 > 最近塔 > 水晶 |
| 子弹反弹 | `V' = V - 2*(V·N)*N`，最多2次 |
| 碰撞检测 | AABB，每帧 `intersects()` |
| 对象管理 | `vector` + `erase-remove_if` |
| 波次生成 | 基于时间，每波独立配置 |
| 无敌帧 | `sf::Clock` + 冷却标志 |


## 项目结构

```
SurvivalGame/
├── main.cpp          # 游戏主循环、状态机、波次管理
├── Player.h/cpp      # 移动、攻击、动画
├── Enemy.h/cpp       # AI、仇恨系统
├── Bullet.h          # 反弹、碰撞、伤害
├── Tower.h           # 自动索敌
├── HealthPack.h      # 血包掉落
├── Crystal.h         # 水晶核心
├── DamageNumber.h    # 伤害数字特效
├── TrailParticle.h   # 冲刺拖尾粒子
├── SlashEffect.h     # 近战特效
├── HealthBar.h       # 玩家、水晶血条
└── EnemyHealthBar.h  # 敌人血条
```




## 许可证

MIT
