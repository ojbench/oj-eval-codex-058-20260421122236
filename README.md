# Problem 058 - 宝可梦图鉴 Pokedex

**ACMOJ Problem ID**: 1277

## Table of Contents

- [Problem 058 - 宝可梦图鉴 Pokedex](#problem-058-宝可梦图鉴-pokedex)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
    - [Background](#background)
  - [Assignment Description](#assignment-description)
    - [Grade Composition](#grade-composition)
  - [Assignment Requirements](#assignment-requirements)
    - [Input Format](#input-format)
    - [Output Format](#output-format)
    - [Samples](#samples)
    - [Data Constraints](#data-constraints)
  - [Per-Testcase Resource Limits](#per-testcase-resource-limits)
  - [Test Data](#test-data)
  - [Submission Requirements](#submission-requirements)
    - [Submission Format](#submission-format)
    - [Submission Guidelines](#submission-guidelines)
    - [Evaluation Notes](#evaluation-notes)
    - [Academic Integrity](#academic-integrity)

## Introduction

### Background

Implement Pokemon Pokedex system with template classes, inheritance hierarchy, search/sort functionality.

## Assignment Description

### Grade Composition

| Grading Component | Percentage |
| :--: | :--: |
| Pass **1277. 宝可梦图鉴 Pokedex** | 80% |
| Code Review | 20% |

Here are several points that need clarification:

- In the Code Review, we will **strictly examine your code style and repository organization structure, etc.**

- This assignment provides some sample data for testing, stored in the `/workspace/data/058/data_test/` directory. Note that these are not the test cases on the Online Judge. Passing all local test cases does not guarantee that you will pass the OJ tests.

- Besides the provided sample data, we also encourage you to design your own test data based on your program logic to assist debugging.

## Assignment Requirements

### Problem Description

<!-- 注意，此题为 “多合一” 小作业，
此题在作业安排上大致相当于 5~6 题普通小作业。

--->




#### 背景

作为《精灵宝可梦》世界中的一名训练师，**图鉴（Pokedex）**在你的旅行过程中是不可或缺的。遇到新宝可梦时，图鉴自动添加新宝可梦信息；当你捕捉到一只宝可梦时，图鉴会添加上更详细的信息。对于有收集爱好的训练师，“全图鉴” 一直是心中最远大的目标。

很遗憾，因为神秘力量的扰动，原本的图鉴系统出了点问题。于是大木博士找到了你，让熟练掌握 C++ 的你帮忙写一个简易的精灵宝可梦图鉴。

本题中如果对题目背景有疑惑，可以查询浏览 [神奇宝贝百科](https://wiki.52poke.com/wiki/%E4%B8%BB%E9%A1%B5) ，或者实在弄不清楚可以询问助教 [SiriusNEO](https://github.com/siriusneo) 。鼓励大家优先认真看题面与样例来理解题意，阅读文档、理解题目的能力也是非常重要的！

---



#### 用语解释
<br>

##### 图鉴

本题中你需要实现的一个容器。容器中存放的数据类型为一条宝可梦记录。

大致地，你需要支持以下功能：

- 图鉴中加入宝可梦。
- 图鉴中删除宝可梦。
- 查找指定 id 的宝可梦。
- 查找指定属性的宝可梦。
- 给定一个属性的攻击，询问指定 id 宝可梦的伤害倍率。
- 进行一次全图鉴捕获尝试，询问最多能捕捉到的宝可梦数量。

<br>

##### 宝可梦

一个独立的宝可梦精灵个体。


- 例：编号 007 的 杰尼龟 就是一只宝可梦。在本题中其信息表示为，
`name=Squirtle, id=7, types=water`


本题中，经过简化，每个宝可梦具有如下信息：

- `name` ，宝可梦的名字。为一个字符串，内容仅包含大小写英文字母。保证不同宝可梦的名字各不相同。
- `id`，宝可梦在图鉴中的唯一编号。为一个 `int` 范围内的正整数。
- `types`，宝可梦的属性。一个宝可梦可能有 1~7 种属性（与原作设定不同噢），以 `#` 分隔。每个属性为一个英文字符串，详情见下文。

<br>

##### 属性

每个宝可梦都有属性，如 [杰尼龟](https://wiki.52poke.com/wiki/%E6%9D%B0%E5%B0%BC%E9%BE%9F) 是水属性。为了防止编码差异，属性名称统一用**英文**表示。

原版的属性有 18 种，本题中简化为如下 7 种属性：

<br>

<img src="https://i.loli.net/2021/08/24/3IzxA6KeLudr4XR.png" style="width:80%">

<br>

<br>

属性之间存在三种**单向关系**：克制为 2 倍伤害，抵抗为 0.5 倍伤害，无效为 0 倍伤害。


- 如：水作为攻击方，火作为防御方，伤害倍率是 2。

需要注意的是，**攻击方是永远一个单属性技能，防御方是一个宝可梦，可能有多种属性**。


- 例：假设 A 宝可梦为 水+龙 系，它受到火系技能攻击，只会受到 0.25 倍的伤害。

注意表格中两种的 0 倍伤害情况：地面系作为攻击方，飞行系作为防御方；电系作为攻击方，地面系作为防御方。

<br>

<img src="https://i.loli.net/2021/08/24/MWhnGjlrSd5FTks.png" style="width:80%">

<br>

<br>

##### 捕获

利用自己现有的宝可梦与野生宝可梦战斗，将其攻击至虚弱状态然后捕捉。

本题中，我们约定一个宝可梦可被捕捉，当且仅当你当前拥有的宝可梦能对其造成**至少 2 倍的伤害**，并且我们假定一只宝可梦会且仅会自己属性的招式。


- 例如，我现在拥有 烈咬陆鲨（地面+龙） ，我试图捕捉 快龙（龙+飞行）。

  因为烈咬陆鲨可以使用地面系技能或者龙系技能，所以我们可以选择用龙系技能攻击快龙，由表可知为 2 倍伤害，因此可以捕捉。


图鉴中要求的 ”全图鉴捕获尝试“ 是指这样一个过程：

- 一开始，你拥有图鉴上 `id` 最小的宝可梦。
- 你使用目前拥有的宝可梦对图鉴中所有宝可梦进行捕捉尝试。
- 当新的宝可梦被捕获后，其视作你拥有的宝可梦，并重复上一步，直到没有再能捕获的宝可梦。
- 返回你拥有的宝可梦个数。

**注意，每次这种询问都是独立的，也就是下一次询问时还是从 "拥有 `id` 最小宝可梦" 这一步开始，不会继承之前捕获结果。**

---



#### 任务要求

本题共有四个模块要求，且如果要完成当前模块，需要先完成此模块之前的模块。

你需要实现图鉴类，请将写好类放在一起直接提交到 OJ 中，下文会提供一份本题接口的模板。

你可以自己定义需要的成员变量、函数等。
<br>

**Subtask 1: 基本功能与文件读写（30pts）**

实现图鉴类基本功能。注意数据需要在启动时读入内存，退出时写回文件。我们会多次开关程序来判断你是否完成了文件读写。
<br>

**Subtask 2: 迭代器 （20pts）**

实现图鉴类的迭代器。迭代器是图鉴的内嵌类。

迭代器在图鉴内的顺序按 `id` 从大到小。例如，`begin()` 返回指向图鉴中 `id` 最小的宝可梦的迭代器。

本部分测试不会出现迭代器异常的情况，相关测试在 Subtask3。
<br>
**Subtask 3: 异常处理（20pts）**

测试过程中，会出现各种异常现象。当异常发生时，你只需要 `throw` 对应异常类，并忽视本次调用即可。

我们会检查你 `throw` 的异常类型是否正确以及错误信息是否正确。

你需要实现两个异常类，处理这两种异常。这两个异常类均继承于基础异常类。

你需要处理以下五种错误情况：

<br>

<img src="https://i.loli.net/2021/08/26/tVrfevinA9FLgU5.png" style="width:80%">

<br>

<br>

不用关心抛出异常的顺序，评测时保证每次调用唯一发生一种异常。但需要保证异常发生后此次执行不能造成任何效果。并且异常的优先级应该高于其它情况（例如找不到 id 等情况）。



- 例如，试图 `pokeAdd("pm1", 1, "water")` 会抛出异常
  显示错误信息为 `Argument Error: PM Name Invalid (pm1)`
  
  试图 `typeFind("water#ice#block")`
  显示错误信息为 `Argument Error: PM Type Invalid (ice)`
  
  即一遇到错误信息就停止，不用再对后面的错误属性 block 报错


<br>

**Subtask 4: 综合测试（30pts）**

包含上述所有情况的测试，数据范围更大。

最后提供提交代码的模板，包含了需要实现的所有接口。

```C++
//可以自己定义一些全局变量、引用头文件
#include <iostream>

class BasicException {
protected:
    //可以自己定义所需要的成员
    const char *message;

public:
    explicit BasicException(const char *_message) {
        //用错误信息初始化
    }

    virtual const char *what() const {
        //return: 错误信息字符串
        return message;
    }
};

class ArgumentException: public BasicException {
};

class IteratorException: public BasicException {
};

struct Pokemon {
    //设计你自己的宝可梦类！
    //唯一要求：一定要有名为name id的两个public成员变量，评测时需要用到
    char name[12];
    int id;
    //...
};

class Pokedex {
private:
    //可以自己定义需要的成员

public:
    explicit Pokedex(const char *_fileName) {
        //TO DO: 构造函数，从名为_fileName的文件中读取数据（没有则创建）
    }

    ~Pokedex() {
        //TO DO: 析构函数
    }

    bool pokeAdd(const char *name, int id, const char *types) {
        //TO DO: 添加宝可梦
        //返回是否添加成功
    }

    bool pokeDel(int id) {
        //TO DO: 删除对应宝可梦
        //return: 返回是否删除成功
    }

    std::string pokeFind(int id) const {
        //TO DO: 查询id对应的宝可梦
        //return: 如果找到，返回宝可梦的name。否则返回字符串 "None"
    }

    std::string typeFind(const char *types) const {
        //TO DO: 查询属性包含了types中的属性的宝可梦
        //return: 如果找到，返回多行字符串。第一行为满足的宝可梦数量，
        //		  接下来每一行为宝可梦的name，按id从小到大。
        //        如果找不到，返回字符串 "None"
        //注意，types可能为多种属性，同样用 # 号分隔
    }

    float attack(const char *type, int id) const {
        //TO DO: 伤害倍率计算
        //return: 返回id对应的宝可梦受到属性type的攻击的伤害倍率。
        //		  如果id对应不存在宝可梦，返回 -1
        //这里type保证为单属性
    }

    int catchTry() const {
        //TO DO: 全图鉴捕获尝试
        //return: 按照前文过程最后能拥有的宝可梦数量。
    }

    struct iterator {
        //可以自己定义所需要的成员

        iterator &operator++() {
            //TO DO: ++迭代器
        }
        iterator &operator--() {
            //TO DO: --迭代器
        }
        iterator operator++(int) {
            //TO DO: 迭代器++
        }
        iterator operator--(int) {
            //TO DO: 迭代器--
        }
        iterator & operator = (const iterator &rhs) {
            //TO DO: 迭代器赋值
        }
        bool operator == (const iterator &rhs) const {
            //TO DO: 判断迭代器是否指向同一位置
        }
        bool operator != (const iterator &rhs) const {
            //TO DO: 不等号
        }
        Pokemon & operator*() const {
            //TO DO: * 运算符
        }
        /**
		 * for the support of it->first.
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
        Pokemon *operator->() const {
            //TO DO: -> 运算符
        }
    };

    iterator begin() {
        //return: 返回指向图鉴头部的迭代器
    }

    iterator end() {
        //return: 返回指向图鉴尾部的迭代器。
        //准确的说，是“最后的一个元素”的后面，指向的内容为空，详情可以去了解标准库容器的 end()
    }
};
```

### Input Format

无，请实现图鉴类然后提交 `.hpp` 文件的内容即可。

### Output Format

无，请实现图鉴类然后提交 `.hpp` 文件的内容即可。

### Samples

#### Sample 1

### Data Constraints

本题共有 4 个 Subtask，每个 Subtask 有 5 个测试点，**测试点之间会开关你的程序，你需要通过文件读写的方式需要保存上一个测试点的信息**。

保证宝可梦总个数 $\le 2000$，保证每个测试点调用函数次数 $\le 5000$。

<img src="https://i.loli.net/2021/08/24/q5mzvNBFexKjPo1.png" style="width:80%">

宝可梦名称 `name` 长度不超过 10。

宝可梦 `id` 为一个在 `1e9` 范围内的正整数。

属性为英文字符串，每种属性字符串的长度不超过 10，一只宝可梦的属性个数为 $[1, 7]$。

**注：以上数据范围即使在异常测试中也仍然保证。异常测试中会出现的错误情况请参见上文。**

## Per-Testcase Resource Limits

- **Time Limit (per test case)**: 5000 ms
- **Memory Limit (per test case)**: 256 MiB
- **Disk Usage**: No disk usage is permitted.

## Test Data

The test data for this problem is located at `/workspace/data/058/data_test/`.

## Submission Requirements

### Submission Format

Submit a single C++ source file to the Online Judge. The OJ provides its own `main.cpp` which `#include "src.hpp"`. Your submission will be written to `src.hpp` and compiled together with the OJ's driver code.

### Submission Guidelines

- The submitted code must be able to compile successfully through the above compilation process
- The program needs to be able to read data from standard input and write results to standard output
- Please ensure the code runs correctly within the given time and space limits
- **You must use C++ or C language** to implement this assignment

### Evaluation Notes

- The evaluation system will test your program using the provided test data
- The program output must exactly match the expected output (including format)
- Exceeding time or memory limits will be judged as the corresponding error type
- Please pay attention to the overall time performance of your code and the time complexity of each part of your algorithm.

### Academic Integrity

If any violations are found during evaluation or code review (including but not limited to using unconventional methods to pass test cases), your final score may be significantly reduced or become **0 points**.
