(import (chicken string))
(import (chicken io))
(import (chicken sort))
(import matchable)
(import srfi-1)
(import regex)

(define input
  (with-input-from-file "input"
    (lambda ()
      (let loop ((ret '()))
        (let ((line (read-line)))
          (if (eof-object? line)
              (reverse ret)
              (loop (cons line ret))))))))

(define (transpose in)
  (define cols (string-length (car in)))
  (let loop ((i 0) (ret '()))
    (if (>= i cols)
        (reverse ret)
        (loop (+ i 1) (cons (list->string (map (lambda (x) (string-ref x i))
                                                in))
                            ret)))))

(define (flip in)
  (define (flip-row row)
    (list->string (reverse (string->list row))))
  (map flip-row in))

(define (char-at in x y)
  (string-ref (list-ref in y) x))

(define (check-xmas mat x y)
  (and (equal? #\M (char-at mat x y))
       (equal? #\M (char-at mat x (+ 2 y)))
       (equal? #\S (char-at mat (+ x 2) y))
       (equal? #\S (char-at mat (+ x 2) (+ y 2)))
       (equal? #\A (char-at mat (+ x 1) (+ y 1)))))

(define (count-mat mat)
  (define count 0)
  (define width (string-length (car mat)))
  (define height (length mat))
  (do ((x 0 (+ 1 x)))
      ((>= x (- width 2)) #f)
    (do ((y 0 (+ 1 y)))
        ((>= y (- height 2)) #f)
      (if (check-xmas mat x y)
          (set! count (+ 1 count))
          #f)))
  count)

(define mat-list
  (list input (flip input)
        (transpose input) (flip (transpose input))))

(display (apply + (map count-mat mat-list)))
(newline)
