;;;;
;;;; client.lsp
;;;; By: tougafgc
;;;; Date: 22 November 2025
;;;;
;;;; Minimal client instance to connect to
;;;; a running server inside of MBAA.exe.
;;;; Everything is contained within the server
;;;; instance, so nothing extra is needed here.
;;;;

(define (start-client)
  (setq socket (net-connect "localhost" 25565))
  (setq quit? nil)

  (while (not (or (net-error) quit?))
    (print "\ncrsc> ")
    (setq input (read-line))
    (if (= input "")
      (setq quit? true)
      (begin
        (net-send socket (append input "\n"))
        (net-receive socket buffer 1024 "\n")
        (print buffer))))

  (net-close socket))

;; Automatically start the client on startup.
(start-client)
(exit 0)
