(import (chicken io))
(import (chicken string))
(import srfi-69)
(import srfi-1)

(define (read-input)
  (with-input-from-file "input"
    (lambda ()
      (define part1
        (let loop ((ret '()))
          (let ((line (read-line)))
            (if (= 0 (string-length line))
                (reverse ret)
                (loop (cons line ret))))))
      (define part2
        (let loop ((ret '()))
          (let ((line (read-line)))
            (if (eof-object? line)
                (reverse ret)
                (loop (cons line ret))))))
      (values part1 part2))))

(define-values (part1 part2) (read-input))

(set! part1 (map (lambda (x) (string-split x "|")) part1))
(set! part1 (map (lambda (x) (map string->number x)) part1))

(define rules (make-hash-table))

(map (lambda (x)
       (hash-table-set! rules x #t) '())
     part1)

(define updates (map (lambda (x) (string-split x ",")) part2))
(set! updates (map (lambda (x) (map string->number x)) updates))

(define (check-rules x lst)
  (if (null? lst)
      #t
      (and (not (hash-table-ref/default rules (list (car lst) x) #f))
           (check-rules x (cdr lst)))))

(define (check-update update)
  (if (null? update)
      #t
      (and (check-rules (car update) (cdr update))
           (check-update (cdr update)))))

(define (median lst)
  (list-ref lst (quotient (length lst) 2)))

(display (apply + (map median (filter check-update updates))))
