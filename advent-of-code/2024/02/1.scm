(import (chicken string))
(import (chicken io))
(import (chicken sort))
(import matchable)
(import srfi-1)

(define input
  (with-input-from-file "input"
    (lambda ()
      (let loop ((ret '()))
        (let ((line (read-line)))
          (if (eof-object? line)
              (reverse ret)
              (loop (cons line ret))))))))

(set! input (map string-split input))
(set! input (map (lambda (lst) (map string->number lst)) input))

(define (diff in)
  (let loop ((lst in) (ret '()))
    (match lst
      ((a b . r) (loop (cdr lst) (cons (- a b) ret)))
      ((a) (reverse ret)))))

(define ladder (map diff input))

(define (same-sign in)
  (define prod (map (lambda (x) (* x (car in))) in))
  (foldl (lambda (a b) (and a b)) #t (map (lambda (x) (> x 0)) prod)))

(define (safe in)
  (not (any (lambda (x) (> (abs x) 3)) in)))

(define result
  (map (lambda (a b) (and a b))
       (map same-sign ladder)
       (map safe ladder)))

(display (length (filter (lambda (x) x) result)))
(newline)

