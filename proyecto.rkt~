#lang racket

#! Leer archivo
(require 2htdp/batch-io)

(define lst
  (string->list (read-file "/home/rubixs/Documents/ITC/ITC semestre 4/Implementacion de metodos computacionales/proyecto/prueba.cpp")))

(define escribe
  (lambda (destination html)
    (write-file destination html)))

(define destination
  "/home/rubixs/Documents/ITC/ITC semestre 4/Implementacion de metodos computacionales/proyecto/prueba.html")

(define head
  "<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Actividad Integradora 3.4 Resaltador de sintaxis</title>
</head>
<body>")

#! Palabras reservadas
(define reserved
  '())

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

      ;operadores
      [(equal? (car lst) #\=) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\+) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\-) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\*) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\/) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\^) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\() (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]
      [(equal? (car lst) #\)) (dfa (cdr lst) (token "<span class='operador'>" 1 lst html))]

      ;
      [else
       (dfa (cdr lst) html)])
      )))
