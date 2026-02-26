(import (chicken io))
(import regex)
(import (chicken string))

(define in-str
  (with-input-from-file "input" read-string))

;; (display in-str)
(define re "mul\\([0-9]+,[0-9]+\\)")

(define matched
  (let loop ((start 0) (result '()))
    (define search-ret (string-search re in-str start))
    (if (not search-ret)
        (reverse result)
        (loop (cadar (string-search-positions re in-str start))
              (cons (car search-ret) result)))))

(define (extract-numbers str)
  (define nums-str (substring str 4 (- (string-length str) 1)))
  (map string->number (string-split nums-str ",")))

(define (calculate str)
  (define vals (extract-numbers str))
  (* (car vals) (cadr vals)))

(display (apply + (map calculate matched)))
