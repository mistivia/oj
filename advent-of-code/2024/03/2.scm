(import (chicken io))
(import regex)
(import matchable)
(import (chicken string))

(define in-str
  (with-input-from-file "input" read-string))

;; (display in-str)
(define re
  "mul\\([0-9]+,[0-9]+\\)|do\\(\\)|don't\\(\\)")

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

(define (process-instr str)
  (cond ((string-search "mul" str) (extract-numbers str))
        ((string-search "do\\(" str) 'do)
        (else 'dont)))

(define (calculate str)
  (define vals (extract-numbers str))
  (* (car vals) (cadr vals)))

(define sum
  (let loop ((is-do #t)
             (lst (map process-instr matched))
             (ret 0))
    (match lst
      (() ret)
      (('do . r) (loop #t r ret))
      (('dont . r) (loop #f r ret))
      (((a b) . r) (if is-do
                  (loop is-do r (+ ret (* a b)))
                  (loop is-do r ret))))))

(display sum)
