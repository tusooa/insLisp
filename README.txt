== 几个基本概念 ==
不带参数执行 insLisp，会进入 Read, Eval, Print-Loop (REPL)。
带参数执行 insLisp，第一个参数为文件名，以后的参数会传给 ARGV 变量。

在 `` 和 '' 之间的是代码部分。之外的视作字符串字面量。
在代码中，形如 VARNAME 的，表示取 VARNAME 这个变量的值。
形如 (FUNC EXPR ...) 的，表示以 EXPR ... 作为参数调用 FUNC 这个函数。
在函数定义里，&optional 代表接下来所有的参数都是可选的，&rest LIST 代表剩下来所有的参数都会进入 LIST 这个列表里；如果没有剩下参数，那么 LIST 将会是空表。

== 函数的用法 ==
(q EXPR)
(quote EXPR)
照原样返回。

(+ &rest LIST)
(* &rest LIST)
(- &rest LIST)
(/ &rest LIST)
数字运算。对于减法和除法，在只有一个操作数的情况下，(- ARG)会返回ARG的相反数，(/ ARG)会返回ARG的倒数。

(> &rest LIST)
(< &rest LIST)
(= &rest LIST)
数字比较运算。可以链式比较。(> 3 2 1)返回t。

(concat &rest LIST)
字符串连接。

(# &rest IGNORE)
什么都不做，然后返回一个空表。可以把注释写在这里面。
但是不要把这个作为别的函数的参数。

(list &rest LIST)
返回 LIST。例如(list 1 2)返回(1 2)。

(dumper EXPR)
返回 EXPR 的人话形式。数字返回其值，符号返回其名，字串返回形式为 ''STR``，而列表返回形式为(...)。
试图 Dump 内置函数会返回<func>。而 Dump 一个 Lambda 会指明其定义的位置。

(length STR)
返回字符串 STR 的长度。

(if COND THEN &rest ELSE)
条件判断。如果 COND 不是空表，就执行 THEN，否则执行 ELSE。
THEN 只能有一条。这个限制可以通过 progn 来改变，例如(if t (progn (print 1) (print 2)))。

(progn &rest LIST)
依次执行 LIST 里的语句，返回最后一个表达式的结果。

(lambda ARGS &rest LIST)
创建一个匿名函数，其参数列表为 ARGS，函数体为 LIST。
例如:
(lambda (a b) (print a {\n}) (print b {\n}))
在创建之后可以赋值给一个变量：
(setq foo (lambda (a b) (print a {\n}) (print b {\n})))
然后进行调用：
(foo 1 2)
或者在创建的同时一次性调用：
((lambda (x) (* 2 x)) 1)

(print &rest LIST)
依次打印 LIST 中的内容。如果是字符串则原样打印，否则打印 (dumper EXPR)。

(car LIST)
返回 LIST 的第一个元素。如果 LIST 是空表，返回空表。

(cdr LIST)
返回 LIST 除开第一个元素之外的所有元素构成的列表。如果 LIST 含有的元素数目小于 2，返回空表。

(cons ARG LIST)
返回一个列表，其第一个元素为 ARG，剩余元素为 LIST 中每一个元素。

(set &rest LIST)
==> (set SYMBOL VALUE ...)
LIST 必须是含有偶数个元素的列表，其奇数项必须是 Symbol。
将 Symbol 对应的变量设为其后一项的值。
例如:
(set (q a) 1 (q b) 2)
将变量 a 的值设为 1，而将 b 的值设为 2。

(setq &rest LIST)
与 set 类似，但是不必明写(q)。
例如:
(setq a 1 b 2)

(eq &rest LIST)
判断Symbol是否相等。
例如：
(eq (q a) (q a)) ==> t
(eq (q b) (q a)) ==> ()

(string= &rest LIST)
(string> &rest LIST)
(string< &rest LIST)
比较字符串。

(load FILE-NAME)
加载 FILE-NAME 文件。

== func.il ==
src 目录下有文件 func.il。定义了几个由内建函数衍生出的基本的操作。同时也是几个基本的例子。可以通过在 REPL 中输入
``(load ''PATH\TO\func.il``)
来加载。其中PATH\TO\表示存放func.il的目录。

将 func.il 作为文本文件打开，可以看到各个函数的定义和用法。

