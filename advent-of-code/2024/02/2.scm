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

(define (same-sign? in)
  (define prod (map (lambda (x) (* x (car in))) in))
  (foldl (lambda (a b) (and a b)) #t (map (lambda (x) (> x 0)) prod)))

(define (extend in)
  (let loop ((lst in) (head '()) (ret '()))
    (if (null? lst)
        (cons in ret)
        (loop (cdr lst)
              (cons (car lst) head)
              (cons (append (reverse head) (cdr lst)) ret)))))

(define (ladder-safe? in)
  (and (same-sign? in)
       (not (any (lambda (x) (> (abs x) 3)) in))))

(define (safe? in)
  (any ladder-safe? (map diff (extend in))))

(display (length (filter safe? input)))
(newline)

