# mlata

Коллоквиум по МЛиТА 
Выводимость логических формул из аксиом

## Постановка задачи

Известны аксиомы исчисления высказываний:
* A1.   (A→(B→A))
* A2.   ((A→(B→C))→((A→B)→(A→C)))
* A3.   ((⅂B→⅂A)→((⅂B→A)→B))
Из этих аксиом и правила вывода Modus ponens можно вывести все тождественные формулы исчисления высказываний, например, закон Моргана и пр.
Modus ponens:   A, A→B ⊢  B
Конъюнкция и дизъюнкция выражаются через отрицание и импликацию (выразите самостоятельно).
#### Задание 1.
###### Нужно написать программу, которая выведет все следующие тождества (которые часто рассматриваются как аксиомы).  При ускорения вывода полезно использовать теорему о дедукции ( Аксиомы и правила вывода теорема 7 на стр. 5).
* A4:   A∧B→A
* A5:   A∧B→B
* A6:   A→(B→(A∧B))
* A7:   A→(A∨B)
* A8:   B→(A∨B)
* A9:   (A→C)→((B→C)→((A∨B)→C))
* A10:   ¬A→(A→B)
* A11:   A∨¬A
#### Задание 2.
Расширим набор правил вывода:
- Modus ponens:   			P, P→Q ⊢ Q
- Modus tollens:    			P→Q, ⅂Q ⊢ ⅂P
- Дизъюнктивный силлогизм		⅂P, P∨Q ⊢ Q
- Гипотетический силлогизм		P→Q, Q→R ⊢ P→R
- Разделительный силлогизм		P, P xor Q ⊢ ⅂Q
- Простая конструктивная дилемма	P→R, Q→R, P∨Q ⊢ R
- Сложная конструктивная дилемма	P→R, Q→T, P∨Q ⊢ R∨T
- Простая деструктивная дилемма	P→R, P→Q, ⅂R∨⅂Q ⊢ ⅂P
- Сложная деструктивная дилемма	P→R, Q→T, ⅂R∨⅂T ⊢ ⅂P∨⅂Q 
 ###### Нужно написать программу, которая использует большее число силлогизмов (правил вывода). Чем короче вывод, тем лучше. Чем больше правил вывода тем лучше. Правила вывода можно применять к уже полученным в ходе вывода теоремам из списка A4-A11. В качестве красивой, но не обязательной добавки можно добавить модуль, который печатает (или даже озвучивает) вывод на русском языке так, чтобы вывод был понятен человеку.
#### Задание 3
###### Разработать аналог метода резолюций, если посылки имеют вид формул, включающих только импликации и отрицания (или аналог метода Дэвиса-Патнема о противоречивости записанного таким образом набора посылок). Привести примеры применения метода, подобрав примеры, на которых предложенный алгоритм будет работать эффективно. Например, если одна посылка A→B, а другая B, то первую посылку можно убрать. Действительно, если B ложно, то противоречивость есть, если же B истинно, то A→B истинно при любых  A и посылка не играет роли для доказательства противоречивости.


#### Задание 4. 
###### Придумать максимально сложное тождество, которое написанная командой программа решит за разумное время (не более 1 минуты) и которое предположительно другие команды не смогут доказать за указанное время. Тождество может содержать не более 26 переменных, обозначаемых строчными буквами латинского алфавита.
Для унификации ввода будем использовать следующие односимвольные обозначения операций:


- Отрицание !
- Дизъюнкция |
- Конъюнкция *
- Импликация >
- XOR +
- Эквиваленция =

###### Значок вывода не пишется, каждое утверждение занимает одну строку. Строки нумеруются. Каждая строка с выводом сопровождается комментарием - номер аксиомы или строки с выведенным тождеством, которая использовалась для вывода и подстановка которая была сделана (в форме “буква -> формула”).

## Реализация
- Созданы классы операций и переменных
- Написан парсер, позволяющий считывать строку из файла и из консоли ввода в программу
- Реализованы правила вывода формул из задания 2
- Реализован метод дедукции для решения задания 1
#### Задание 1
A8:   B→(A∨B)
deduction.txt
curr formula: (B > (!A > B))	{B }
curr formula: (!A > B)	{B !A }
{B !A B }

result.txt
(B > (B > B))
(B > (!A > B))
(!A > (B > !A))
(!A > (!A > !A))
((B > (B > B)) > ((B > B) > (B > B)))
((B > (B > !A)) > ((B > B) > (B > !A)))
((B > (!A > B)) > ((B > !A) > (B > B)))
((B > (!A > !A)) > ((B > !A) > (B > !A)))
((!A > (B > B)) > ((!A > B) > (!A > B)))
((!A > (B > !A)) > ((!A > B) > (!A > !A)))
((!A > (!A > B)) > ((!A > !A) > (!A > B)))
((!A > (!A > !A)) > ((!A > !A) > (!A > !A)))
((!B > !B) > ((!B > B) > B))
((!!A > !B) > ((!!A > B) > !A))
((!B > !!A) > ((!B > !A) > B))
((!!A > !!A) > ((!!A > !A) > !A))
B
!A
((B > B) > (B > B))
((B > !A) > (B > B))
((!A > B) > (!A > !A))
((!A > !A) > (!A > !A))
(B > B)
(!A > B)
(B > !A)
(!A > !A)
((B > B) > (B > B))
((B > !A) > (B > B))
((!A > B) > (!A > !A))
((!A > !A) > (!A > !A))
(B > B)
(!A > B)
B

Записи из терминала:
julia@julia-ZenBook:~/3 курс/mlata/project/build$ ./mlata 
(B > (!A > B))
Go to deduction...
Deduction ready.
Done!
