(defun find-palindrome-impl (i j current-max)
    (cond ((> j 999) current-max)
	  ((> i 999) (find-palindrome-impl 100 (+ j 1) current-max))
	  ((is-palindrome (* i j))
	   (if (> (* i j) current-max)
	       (find-palindrome-impl (+ i 1) j (* i j))
	       (find-palindrome-impl (+ i 1) j current-max)))
	  (t (find-palindrome-impl (+ i 1) j current-max))))

(defun is-palindrome (x)
  (equal (reverse (write-to-string x)) (write-to-string x)))


(format t "~&~S~%" (find-palindrome-impl 100 100 0))
