(import (chicken io))

(define heatmap
  (call-with-input-file "input"
    (lambda (fp)
      (define (impl ret)
        (let ((l (read-line fp)))
  	(if (equal? l #!eof)
  	    (reverse ret)
  	    (impl (cons l ret)))))
      (impl '()))))

(set! heatmap (list->vector heatmap))

(define (heat x y)
  (string->number (substring (vector-ref heatmap y)
				    x
				    (+ 1 x))))

