(not EXPR): 如果 EXPR 是空表，返回 t，否则返回空表。
``
(setq not (lambda (e) (if e (list) t)))
''
(last LIST): 返回 LIST 中最后一个元素。
``
(setq last (lambda (l)
             (if (cdr l)
                 (last (cdr l))
               (car l))))''
(nth LIST NUM): 获取 LIST 中第 NUM 个元素。采用了递归。
``(setq nth (lambda (l n)
              (if (< n 1) (car l)
                (nth (cdr l) (- n 1)))))''
(list-len LIST): 获取 LIST 中元素的个数。采用了递归。
``(setq list-len (lambda (l)
                   (if (cdr l)
                       (+ 1 (list-len (cdr l)))
                     (if l
                         1
                       0))))''
(reverse LIST): 将 LIST 倒序返回。采用了递归。
``
(setq reverse (lambda (l &optional n)
                (if (not n)
                    (setq n (list-len l)))
                (if (< n 2)
                    (if l (list (car l)) (list))
                    (cons
                     (nth l (- n 1))
                     (reverse l (- n 1))))))
''
比较函数。!=无法采用链式比较。
``
(setq >=-list (lambda (l)
           (if (< (list-len l) 2)
               t
             (if (< (car l)
                    (car (cdr l)))
                 (list)
               (>=-list (cdr l))))))
(setq >= (lambda (&rest l) (>=-list l)))
(setq <= (lambda (&rest l) (>=-list (reverse l))))
(setq != (lambda (a b) (not (= a b))))
(setq ne (lambda (a b) (not (eq a b))))
''
(map FUNC LIST): 对 LIST 的每一个元素，分别执行 FUNC。返回所有结果构成的列表。
FUNC 不需要被引用。
``
(setq map (lambda (f l)
            (if (not l)
                (list)
              (cons
               (f (car l))
               (map f (cdr l))))))
''
(say &rest LIST): 打印 LIST，并附带换行。
``
(setq say (lambda (&rest l)
            (map print l)
            (print {\n})))
''
``1
