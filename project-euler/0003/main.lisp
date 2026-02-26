(defun my-reduce (x y)
    (if (= 0 (mod x y))
	(my-reduce (/ x y) y)
	x))

(defun max-factor-impl (x cur)
  (let ((next-x (my-reduce x cur)))
    (if (= 1 next-x)
	cur
	(max-factor-impl next-x (+ 1 cur)))))

(defun max-factor (x)
  (max-factor-impl x 2))

(format t "~&~S~%" (max-factor 600851475143))

