## Syntax
Zaba's syntax borrows heavily from the C family of languages, while being simpler and concise. All Zaba scripts are stored in files with the .zaba extension, being translated to Wren before execution.

## Reserved words
The extensions to the Wren language are ```func``` and ```struct```.
```
as break class construct continue else false func for foreign if import in is null return static struct super this true var while
```

## Identifiers
The naming rules are as follows:
- All names are case sensitive.
- A name may contain alphanumeric, digits and underscores.
- Identifiers may start with a letter or an underscore.
    - Names starting with an underscore are fields of classes and structs.

## Newlines
Newlines are used to separate statements, omitting the need for semicolons. Newlines that exist in the middle of a statement are ignored if the following token can't end the statement.
```wren
System.print("separation by newline")
System.print(
    "newline ignored")
```

## Comments
Comments can be of one of two types:
- Line comment: Starts with a **//** and ends in a newline).
```wren
// This is a short comment.
```

- Block comment: Starts with a **/\*** and ends in **\*/**, spanning multiple lines).
```wren
/* This is
   a long
   comment. */
```

## Blocks
Blocks are defined by curly braces. They can be used anywhere a statement is allowed, including control flow, slices, dictionaries and the bodies of functions and methods.
```wren
if (true) {
  System.print("Very true")
} else System.print("Very false")

```

If there is no newline following a left brace, the block can only contain a single expression and return it's value automatically.
```wren
{ true }
```

## Values
Built-in values are instances of primitive classes, being in immutable, never changing after creation.
- Booleans: They represent truth or falsehood, either ```true``` or ```false```.
- Numbers: Double-precision floating point numbers. 
- Strings: Array of UTF-8 encoded characters. They are surrounded in double quotes and can span multiple lines.
    - Strings can be concatenated with the plus operator.
    - Strings can be interpolated with the percent sign followed by an expression inside of a parenthesis. 
- Null: Indicates the absence of a value. If a method or function doesn't return anything and it gets called, ```null``` is returned.
```wren
var bool = true
var number = 3.141516
var string = "a string\n"
var concatenation = "con" + "catenation"
var interpolation = "interpolation <- %(true)"
var nothing = null
```

## Control Flow
Control flow determines the execution of instructions, including branches and loops.

### If statements
A branching statement. Conditionally skips a chunk of code. If the condition is evaluated as true, the first block executes, else it is skipped and the second block continues.
```wren
var good = true
if (good) System.print("Nice")

if (good) {
    System.print("Cool")
}

if (good) {
    System.print("Great")
} else {
    System.print("Awful")
}
```
```bash
$ Nice
$ Cool
$ Great
```
### Ternary operator
The ternary operator is the if statement in the form of an expression, following the structure ```condition ? true : false```.
```wren
System.print(1 != 0 ? "Binary rules!" : "Can't happen!")
```
```bash
Binary rules!
```


### Logical operators
- An ```and``` expression evaluates the left-hand side argument. If false, return the value. Else, evaluate the right-hand argument and return it.
```wren
System.print(true and true)
System.print(true and false)
```
```bash
$ true
$ false
```

- An ```or``` expression reverses ```and```; If the left-hand argument is true, it's returned. Else, evaluate the right-hand argument and return it.
```wren
System.print(true or true)
System.print(true or false)
```
```bash
$ true
$ true
```

### While loop
The while loop executes a chunk of code as long as the condition evaluates to true.
```wren
var paperClips = 0
while (paperClips < 5) {
    System.print("Count of paperclips: %(paperClips)")
    paperClips = paperClips + 1
}
```
```bash
$ Count of paperclips: 0
$ Count of paperclips: 1
$ Count of paperclips: 2
$ Count of paperclips: 3
$ Count of paperclips: 4
```

### For loop
The for loop executes a chunk of code according to a complex condition, like a range of numbers, looping though a slice or another sequence. It has the three components:
- Variable name: A variable that is created for the loop and whose scope is localized to it.
- Sequence expression: It es evaluated once and indicates what the **variable name** is looping over.
- Body: It gets executed once for each iteration of the loop. 
```wren
for (fruit in ["orange", "apple", "banana"]) {
  System.print(fruit)
}
```
```bash
$ orange
$ apple
$ banana
```

### Break statements
The ```break``` keyword by itself immediately exits out of the enclosing ```while``` or ```for``` loop. 
```wren
for (i in [1, 2, 3, 4]) {
    System.print(i)
    if (i == 3) break
}  
```
```bash
$ 1
$ 2
$ 3
```

### Continue statements
The ```continue``` keyword will jump to the beginning of the loop of the next iteration and check the loop's condition.
```wren
for (i in [1, 2, 3, 4]) {
    if (i == 2) continue
    System.print(i)
}  
```
```bash
$ 1
$ 3
$ 4
```

### Numeric ranges
Numeric ranges are a sequence of numbers represented in the form ```A..B``` or ```A...B``` (inclusive and not inclusive).
```wren
for (i in 1..3) {
  System.print(i)
}

for (i in 1...3) {
  System.print(i)
}
```
```bash
$ 1
$ 2
$ 3
$ 1
$ 2
```

## Slices
A sequence of elements identified and access by an integer index. Slices can hold elements of different values. Slices are created by placing a list of expressions inside of a square brackets. They are instances of the List class.
```wren
var chars = ["A", "B", "C", "D"]
chars[1] = "Z"
System.print(chars[1])
System.print(chars.at(1))
System.print(chars.back)
chars.insert(2, "X")
System.print(chars.size)
chars.append("Y")
System.print(chars.toString)
System.print(chars.empty)
chars.erase(1)
chars.clear
System.print(chars.toString)
```
```bash
$ Z
$ Z
$ D
$ 4
$ [A, Z, X, D, Y]
$ false
$ []
```

## Dictionaries
An associative collection. It holds a set of entries, contining pairs of keys and values, being separated by a colon. Keys must be one of the built-in values or a class type. Dictionaries are instances of the Map class.
```wren
var office = Dict.new({
    "Alice":"Boss",
    "Bob":"Engineer"
})
office["Dan"] = "H. R."
System.print(office["Dan"])
System.print(office.at("Dan"))
office.insert("Frank", "Receptionist")
System.print(office.toString)
office.delete("Frank")
System.print(office.toString)
System.print(office.size)
System.print(office.empty)
System.print(office.find("Alice"))
office.clear
System.print(office.toString)
```
```bash
$ H. R.
$ H. R.
$ {Frank: Receptionist, Bob: Engineer, Alice: Boss, Dan: H. R.}
$ {Bob: Engineer, Alice: Boss, Dan: H. R.}
$ 3
$ false
$ true
$ {}
```

## Classes
Classes define the attributes and methods that an object may and can possess. Every object of the same class supports the same methods and state.

Instances of a class can only be made if a constructor method is defined. All constructors have a name, new being a common name. There is no default constructor for objects.
```wren
class Book {
    construct new(title) {
        _title = title
    }

    title{ _title }
}

var novel = Book.new("New book")
System.print(novel.title)
```
```bash
$ New book
```

## Method calls
A method is the equivalent of a function that is tied to a particular class. Each method has a signature, their name and number of arguments it takes. A class can have multiple signatures with the same name, making it possible to overload by arity.
```wren
class Task {
    construct new(id, desc) {
        _id = id
        _desc = desc
    }

    construct new(id, desc, dueDate) {
        _id = id
        _desc = desc
        _dueDate = dueDate
    }
}
```

## Fields
The fields of an object serve to store data. Each field has a name that starts with an underscore. Fields can't be used outside of the instance of the class (they are private by default).
```wren
class Spy {
    construct new(name) {
        _name = name
    }
}

var agent = Spy.new("Bruce")
System.print(agent.name)
```
```bash
$ Spy does not implement 'name'. [compile line 10] in (script)
```

## Getters
A getter is a method that makes the field readable from outside of the class. It follows the structure ```field{_field}```.
```wren
class Spy {
    construct new(name) {
        _name = name
    }
    name{_name}
}

var agent = Spy.new("Bruce")
System.print(agent.name)
```
```bash
$ Bruce
```

## Setters
A setter is a method that exposes the field to changes outside of the class. It follows the structure of ```field=(value){_field=value}```. The parameter can receive any name, value being a common name.
```wren
class Spy {
    construct new(name) {
        _name = name
    }
    name{_name}
    name=(value){_name=value}
}

var agent = Spy.new("Bruce")
agent.name = "Michael"
System.print(agent.name)
```
```bash
$ Michael
```

## Structs
Structs act as classes where each of their fields are public. The translator **drop** adds the setters and getters automatically if a field is found inside any of the constructors, methods or functions.

```wren
struct Date {
    construct new(day, month, year) {
        _day = day
        _month = month
        _year = year
    }
}

var endOfTheYear = Date.new(31, 12, 1999)
System.print(endOfTheYear.day)
```
```bash
$ 31
```

## Functions
Functions can live outside of classes, being stored in variables and invoking ```.call()``` for executing them. A parameter list can be supplied in between two pipe characters followed by the function body.
```wren
var isWeekend = func { | day |
    return (day == "Saturday" or day == "Sunday")
}

System.print(isWeekend.call("Monday"))
```

Under the hood functions are instances of the ```Fn``` class.

