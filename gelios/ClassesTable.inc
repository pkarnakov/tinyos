;===============================================================================
;                         Таблица описания функций
;===============================================================================

DriversTable:
TIMES 8000 dd 0               ; физические адреса пространств драйверов

DriverCall:                   ; процедура вызова драйвера
  

ExtensTable:
TIMES 8000 dd 0               ; физические адреса пространств расширений