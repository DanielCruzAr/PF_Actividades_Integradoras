#lang racket

#! Leer archivo
(require 2htdp/batch-io)

(define lst
  (string->list (read-file "/home/rubixs/Documents/ITC/ITC semestre 4/Implementacion de metodos computacionales/PF_Actividades_Integradoras/Act 3.4/prueba.cpp")))

(define escribe
  (lambda (destination html)
    (write-file destination html)))

(define destination
  "/home/rubixs/Documents/ITC/ITC semestre 4/Implementacion de metodos computacionales/PF_Actividades_Integradoras/Act 3.4/prueba.html")

(define head
  "<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Actividad Integradora 3.4 Resaltador de sintaxis</title>
    <link rel='stylesheet' href='css/style.css'>
</head>
<body>")

#! Palabras reservadas
(define rInt
  '(#\i #\n #\t #\space))

(define rChar
  '(#\c #\h #\a #\r #\space))

(define rFloat
    '(#\f #\l #\o #\a #\t #\space))

(define rString
    '(#\s #\t #\r #\i #\n #\g #\space))

(define rIf
  '(#\i #\f))

(define rElse
  '(#\e #\l #\s #\e))

(define rWhile
  '(#\w #\h #\i #\l #\e))

(define rFor
  '(#\f #\o #\r))

(define rVar
  '(#\v #\a #\r #\space))
  
(define rLet
  '(#\l #\e #\t #\space))

(define rConst
  '(#\c #\o #\n #\s #\t #\space))

(define rInclude
  '(#\# #\i #\n #\c #\l #\u #\d #\e #\space))

(define rStd
  '(#\s #\t #\d))

(define rReturn
  '(#\r #\e #\t #\u #\r #\n #\space))

(define RW
  (lambda (lst rwlst count)
  (if (equal? (car lst) (car rwlst))
      (RW2 (cdr lst) (cdr rwlst) (+ count 1)) 0)))

(define RW2
  (lambda (lst rwlst count)
    (cond
      [(empty? rwlst) count]
      [(empty? lst) 0]
      [(equal? (car lst) (car rwlst))
       (RW2 (cdr lst) (cdr rwlst) (+ count 1))]
      [else 0]
      )))

#! Comentarios
(define comment
  (lambda (lst count)
    (if (equal? (car lst) #\/)
        (comment2 (cdr lst) (+ count 1)) 0)))

(define comment2
  (lambda (lst count)
    (if (equal? (car lst) #\/)
        (comment3 (cdr lst) (+ count 1)) 0)))

(define comment3
  (lambda (lst count)
    (cond
        [(empty? lst) count]
        [(not (equal? (car lst) #\newline))
        (comment3 (cdr lst) (+ count 1))]
        [else count]
    )))

#! Variables
(define var
  (lambda (lst count)
    (if (char-alphabetic? (car lst))
      (var2 (cdr lst) (+ count 1))
    0)))

(define var2
  (lambda (lst count)
    (cond
      [(empty? lst) count]
      [(char-alphabetic? (car lst)) (var2 (cdr lst) (+ count 1))]
      [(equal? (car lst) #\_) (var2 (cdr lst) (+ count 1))]
      [(char-numeric? (car lst)) (var2 (cdr lst) (+ count 1))]
      [else count]
    )))

#! Enteros
(define integer
  (lambda (lst count)
    (if (or (char-numeric? (car lst)) (equal? (car lst) #\-))
        (integer2 (cdr lst) (+ count 1)) 0)))

(define integer2
  (lambda (lst count)
    (cond
      [(empty? lst) count]
      [(char-numeric? (car lst))
        (integer2 (cdr lst) (+ count 1))]
      [else count]
      )))

#! Flotantes
(define float
  (lambda (lst count)
    (if (or (char-numeric? (car lst)) (equal? (car lst) #\-))
        (float2 (cdr lst) (+ count 1)) 0)))

(define float2
  (lambda (lst count)
    (cond
      [(empty? lst) count]
      [(char-numeric? (car lst))
         (float2 (cdr lst) (+ count 1))]
      [(equal? (car lst) #\.)
         (float2 (cdr lst) (+ count 1))]
      [else count]
      )))

#! Strings
(define string
  (lambda (lst count)
    (cond
      [(equal? (car lst) #\")
       (comillas (cdr lst) (+ count 1) #\")]
      [(equal? (car lst) #\')
       (comillas (cdr lst) (+ count 1) #\')]
      [else 0]
      )))

(define comillas
  (lambda (lst count c)
    (cond
      [(empty? lst) 0]
      [(equal? (car lst) c) (+ count 1)]
      [else (comillas (cdr lst) (+ count 1) c)]
      )))

(define mqmq
  (lambda (lst count)
   (if (equal? (car lst) #\<)
       (mqmq2 (cdr lst) (+ count 1)) 0)))

(define mqmq2
  (lambda (lst count)
    (cond
      [(empty? lst) 0]
      [(equal? (car lst) #\>) (+ count 1)]
      [else (mqmq2 (cdr lst) (+ count 1))]
      )))

#! Leector
;determinar si es entero o flotante
(define isFloat
  (lambda (lst index)
    (cond
      [(equal? (car lst) #\newline) #f]
      [(equal? (car lst) #\.) #t]
      [else (isFloat (cdr lst) (+ index 1))]
      )
    ))

;quitar el primer elemento de la lista y devolver el resto
(define pop
  (lambda (lst n)
    (if (> n 0)
        (pop (cdr lst) (- n 1)) lst)))

;unir los tokens
(define token
  (lambda (tag n lst html)
    (string-append html
            (string-append tag
                     (string-append (list->string (lst2 lst '() n)) "</span>")))))

(define lst2
  (lambda (lst newToken n)
     (if (> n 0)
         (lst2 (cdr lst) (append newToken (list (car lst)))
               (- n 1)) newToken)))

;unir el encabezado con el cuerpo
(define headAppend
  (lambda (head html)
    (string-append head
                   (string-append html "</body></html>"))))

;analizar cada linea para determinar el tipo de token
(define dfa
  (lambda (lst html)
    (if (empty? lst)
        (headAppend head html)
    (cond
      ;comentarios
      [(not (equal? (comment lst 0) 0))
       (dfa (pop lst (comment lst 0)) (token "<span class='comment'>" (comment lst 0) lst html))]

      ;palabras reservadas
      ;int
      [(not (equal? (RW lst rInt 0) 0))
       (dfa (pop lst (RW lst rInt 0)) (token "<span class='reserved-word'>" (RW lst rInt 0) lst html))]
      ;char
      [(not (equal? (RW lst rChar 0) 0))
       (dfa (pop lst (RW lst rChar 0)) (token "<span class='reserved-word'>" (RW lst rChar 0) lst html))]
      ;float
      [(not (equal? (RW lst rFloat 0) 0))
       (dfa (pop lst (RW lst rFloat 0)) (token "<span class='reserved-word'>" (RW lst rFloat 0) lst html))]
      ;string
      [(not (equal? (RW lst rString 0) 0))
       (dfa (pop lst (RW lst rString 0)) (token "<span class='reserved-word'>" (RW lst rString 0) lst html))]
      ;if
      [(not (equal? (RW lst rIf 0) 0))
       (dfa (pop lst (RW lst rIf 0)) (token "<span class='reserved-word'>" (RW lst rIf 0) lst html))]
      ;else
      [(not (equal? (RW lst rElse 0) 0))
       (dfa (pop lst (RW lst rElse 0)) (token "<span class='reserved-word'>" (RW lst rElse 0) lst html))]
      ;while
      [(not (equal? (RW lst rWhile 0) 0))
       (dfa (pop lst (RW lst rWhile 0)) (token "<span class='reserved-word'>" (RW lst rWhile 0) lst html))]
      ;for
      [(not (equal? (RW lst rFor 0) 0))
       (dfa (pop lst (RW lst rFor 0)) (token "<span class='reserved-word'>" (RW lst rFor 0) lst html))]
      ;var
      [(not (equal? (RW lst rVar 0) 0))
       (dfa (pop lst (RW lst rVar 0)) (token "<span class='reserved-word'>" (RW lst rVar 0) lst html))]
      ;let
      [(not (equal? (RW lst rLet 0) 0))
       (dfa (pop lst (RW lst rLet 0)) (token "<span class='reserved-word'>" (RW lst rLet 0) lst html))]
      ;const
      [(not (equal? (RW lst rConst 0) 0))
       (dfa (pop lst (RW lst rConst 0)) (token "<span class='reserved-word'>" (RW lst rConst 0) lst html))]
      ;include
      [(not (equal? (RW lst rInclude 0) 0))
       (dfa (pop lst (RW lst rInclude 0)) (token "<span class='include'>" (RW lst rInclude 0) lst html))]
      ;std
      [(not (equal? (RW lst rStd 0) 0))
       (dfa (pop lst (RW lst rStd 0)) (token "<span class='std'>" (RW lst rStd 0) lst html))]
      ;return
      [(not (equal? (RW lst rReturn 0) 0))
       (dfa (pop lst (RW lst rReturn 0)) (token "<span class='reserved-word'>" (RW lst rReturn 0) lst html))]

      
      ;variables
      [(not (equal? (var lst 0) 0))
       (dfa (pop lst (var lst 0)) (token "<span class='var'>" (var lst 0) lst html))]

      ;enteros y flotantes
      [(char-numeric? (car lst))
           (cond
             ;Revisar si es entero o flotante
             [(equal? (isFloat lst 0) #t)
                      ;Flotante
                      (cond
                        [(not (equal? (float lst 0) 0))
                        (dfa (pop lst (float lst 0)) (token "<span class='float'>" (float lst 0) lst html))])]
             [(equal? (isFloat lst 0) #f)
                      ;Entero
                      (cond
                        [(not (equal? (integer lst 0) 0))
                        (dfa (pop lst (integer lst 0)) (token "<span class='int'>" (integer lst 0) lst html))])]
             )]

      ;strings
      [(not (equal? (string lst 0) 0))
       (dfa (pop lst (string lst 0)) (token "<span class='string'>" (string lst 0) lst html))]
      ;<>
      [(not (equal? (mqmq lst 0) 0))
       (dfa (pop lst (mqmq lst 0)) (token "<span class='string'>" (mqmq lst 0) lst html))]
      
      ;operadores
      [(equal? (car lst) #\=) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\+) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\-) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\*) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\/) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\^) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\() (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\)) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\{) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\}) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\[) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\]) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\;) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\:) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\<) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\>) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      
      ; Espacios
      [(equal? (car lst) #\space) (dfa (cdr lst) (string-append html "&nbsp;"))]
      ; Salto de lineas
      [(equal? (car lst) #\newline) (dfa (cdr lst) (string-append html "<br>"))]
      ; Tabs
      [(equal? (car lst) #\tab) (dfa (cdr lst) (string-append html "&nbsp;&nbsp;&nbsp;&nbsp;"))]
      
      [else
       (dfa (cdr lst) html)])
      )))
