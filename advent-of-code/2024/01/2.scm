(import (chicken string))
(import (chicken io))
(import (chicken sort))
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

(define first-lst (map string->number (map car input)))
(define second-lst (map string->number (map cadr input)))

(set! first-lst (sort first-lst <))
(set! second-lst (sort second-lst <))

(define (times x)
  (length (filter (lambda (a) (= a x))
                  second-lst)))

(define times-lst (map times first-lst))

(display (apply + (map * first-lst times-lst)))
(newline)
