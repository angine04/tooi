# Tooi 最小化语言规范

## 语法

### 基本结构

```tooi

let name : type => {
    let property : type -> value;
} @ {
    @do_something;
};

```

### 类型系统

#### 基本类型

- `int`: 整数 (32位)
- `float`: 浮点数 (32位)
- `string`: 字符串 (UTF-8)
- `bool`: 布尔值 (true/false)
- `byte`: 字节

#### 复合类型

- `array`: 动态数组
- `tuple`: 元组
- `assoc`: 键值对

#### 类型示例

```tooi
// 基本类型
let num : int -> 42;
let pi : float -> 3.14159;
let text : string -> "Hello";
let flag : bool -> true;
let data : byte -> [0x01, 0x02, 0x03];

// 数组
let numbers : [int] -> [1, 2, 3, 4, 5];
let strings : [string] -> ["a", "b", "c"];
let mixed : [proto] -> [1, "two", true];

// 元组
let point : (int, int) -> (10, 20);
let rgb : (int, int, int) -> (255, 128, 0);

// 关联数组
let config : [string -> proto] -> [
    "name" -> "Tooi",
    "version" -> "1.0",
    "debug" -> false
];
```

#### 类型操作

```tooi
// 数组操作
let arr : array -> [1, 2, 3];
let first -> arr[0];           // 访问元素
let len -> arr.length;         // 获取长度
arr.@push(4);                  // 添加元素
arr.@pop();                    // 移除最后一个元素
arr.@insert(1, 5);            // 在指定位置插入
arr.@remove(1);               // 移除指定位置的元素

// 数组遍历
for (item in arr) {
    io.@print(item);
}

// 元组访问
let x -> point[0];  // 第一个元素
let y -> point[1];  // 第二个元素

// 关联数组访问
let name -> config["name"];
let version -> config["version"];

// 类型转换
let str_num -> 42 as string;
let num_str -> "42" as int;
let float_int -> 3.14 as int;
```

### 3. 对象结构

Tooi中的对象由两个基本部分组成：

- **模式（mode）**：定义对象的属性和状态
- **行为（act）**：定义对象的执行逻辑

对象的基本结构如下：

```tooi
let obj => {
    // 模式：定义属性和状态
    let prop1: public type1 -> value1,
    let prop2: private type2 -> value2
} @ {
    // 行为：定义执行逻辑
    let prop1 + 1;
};
```

Mode 代表对象的基本形态：

- 定义对象的属性和状态
- 作为可复用的模板
- 可以通过原型链共享
- 描述"对象是什么"

Act 代表对象的行为：

- 定义对象的执行逻辑
- 实现对象的功能
- 描述"对象做什么"

#### 原型和引用

```tooi
// 定义一个原型
let editable : proto => {
    let content -> "";
    let can_edit -> true;
} @ {
    // act: 编辑行为
    if (can_edit) {
        let content -> @input().next;
    }
};

// 创建实例
let doc -> new editable_mode;  // 创建新实例
let same_doc -> doc;     // 引用原型
```

### 4. 引用和赋值

#### 引用绑定

```tooi
// 对象引用
let a -> b;          // a 引用 b
let a -> new b;      // 创建 b 的副本并绑定到 a

// 字面量引用（隐式实例化）
let x -> 42;         // 直接创建整数实例
let s -> "hello";    // 直接创建字符串实例
let f -> 3.14;       // 直接创建浮点数实例
let b -> true;       // 直接创建布尔值实例

// 复合字面量引用
let arr -> [1, 2, 3];    // 直接创建数组实例
let tup -> (1, 2);       // 直接创建元组实例

// 引用和修改
let obj => {
    // 属性中的字面量引用也会隐式实例化
    let x -> 1;          // 直接创建整数实例
    let y -> "test";     // 直接创建字符串实例
};
```

#### 属性操作

```tooi
// 设置属性（替换现有属性）
let obj => {
    let x : int -> 1;
    let y : int -> 2;
};

// 追加属性（保留现有属性）
let obj >> {
    let z : int -> 3;
};

// 设置执行体
let obj @ {
    io.@print(x + y + z);
};
```

#### 关联数组

```tooi
// 在关联数组中使用 -> 绑定键值对
let config => {
    "name" -> "value",
    "age" -> 42
};
```

### 5. 运算符

#### 算术运算符

```tooi
// 基本算术
let sum -> a + b;    // 加法
let diff -> a - b;   // 减法
let prod -> a * b;   // 乘法
let quot -> a / b;   // 除法
let mod -> a % b;    // 取模
let neg -> -a;       // 负号
```

#### 比较运算符

```tooi
// 比较运算
let eq -> a == b;    // 相等
let ne -> a != b;    // 不相等
let lt -> a < b;     // 小于
let le -> a <= b;    // 小于等于
let gt -> a > b;     // 大于
let ge -> a >= b;    // 大于等于
```

#### 逻辑运算符

```tooi
// 逻辑运算
let and_op -> a and b;   // 与
let or_op -> a or b;    // 或
let not_op -> not a;       // 非
```

#### 字符串运算符

```tooi
// 字符串连接
let str -> "Hello" + " " + "World";
```

### 6. 变量声明和赋值

```tooi
// 可变变量
let x : int -> 10;
let x -> 20;  // 重新赋值

// 不可变变量
set y : string -> "hello";
```

### 7. 控制流

```tooi
// if-else
if (condition) {
    // 代码
} else {
    // 代码
}

// while循环
while (condition) {
    // 代码
}

// for循环
for (item in collection) {
    // 代码
}
```

### 8. 对象和属性

```tooi
// 对象定义
let point => {
    let x : int -> 0;
    let y : int -> 0;
};

// 属性访问
let point.x -> 10;
```

### 9. 方法覆盖

```tooi
let obj @ {
    // 新的执行行为
};
```

## 最小化特性集

1. **闭包系统**
   - 基本闭包定义
   - 属性定义
   - 执行体

2. **类型系统**
   - 基本类型
   - 类型注解
   - 类型检查

3. **变量系统**
   - 可变变量（let）
   - 不可变变量（set）
   - 参数（param）

4. **控制流**
   - if-else
   - while循环
   - for循环

5. **对象系统**
   - 基本对象定义
   - 属性访问
   - 方法覆盖

## 示例程序

```tooi
// 简单的计数器
let counter => {
    let count : int -> 0;
} @ {
    let count + 1;
    io.@print("Count: " + count);
};

// 使用计数器
@counter;
@counter;
```
