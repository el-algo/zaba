
![logo](/img/file.svg)
# Zaba
The small scripting language that *flows*
---
Zaba is a class based scripting language with concise syntax that translates into **Wren**. It extends the Wren programming language with *struct* and function calls, adding **Go**-style *slices* and *dictionaries*.

Zaba can be either translated directly into Wren and run at a later time or can be dynamically translated and executed with **drop**.

## Overview
```wren
/// Classical Person OOP example
struct Person {
    construct new(name, age) {
        _name = name
        _age = age
    }

    haveBirthday() {
        age = age + 1
    }
}

var Worker = Person.new("Gary", 30)
System.print(Worker.name)
Worker.haveBirthday()
System.print(Worker.age)
```

``` bash
$ Gary
$ 31
```

## Installation
- Clone the Zaba repo.
- Compile for C++ 17 or later. 
- Download the latest Wren CLI:
    - https://github.com/wren-lang/wren-cli/releases
