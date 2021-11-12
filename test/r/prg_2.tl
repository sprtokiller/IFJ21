D:Token at [2, 1]: require
D:Token at [2, 9]: string literal
D: Val: "ifj21"
D:Token at [4, 1]: function
D:Token at [4, 10]: identifier
D: Val: factorial
D:Token at [4, 19]: left parenthese
D:Token at [4, 20]: identifier
D: Val: n
D:Token at [4, 22]: colon
D:Token at [4, 24]: integer
D:Token at [4, 31]: right parenthese
D:Token at [4, 33]: colon
D:Token at [4, 35]: integer
D:Token at [5, 2]: local
D:Token at [5, 8]: identifier
D: Val: n1
D:Token at [5, 11]: colon
D:Token at [5, 13]: integer
D:Token at [5, 21]: assignment
D: Val: =
D:Token at [5, 23]: identifier
D: Val: n
D:Token at [5, 25]: integer literal
D: Val: -1
D:Token at [6, 2]: if
D:Token at [6, 5]: identifier
D: Val: n
D:Token at [6, 7]: less then comparison
D: Val: <
D:Token at [6, 9]: integer literal
D: Val: 2
D:Token at [6, 11]: then
D:Token at [7, 3]: return
D:Token at [7, 10]: integer literal
D: Val: 1
D:Token at [8, 2]: else
D:Token at [9, 3]: local
D:Token at [9, 9]: identifier
D: Val: tmp
D:Token at [9, 13]: colon
D:Token at [9, 15]: integer
D:Token at [9, 23]: assignment
D: Val: =
D:Token at [9, 25]: identifier
D: Val: factorial
D:Token at [9, 34]: left parenthese
D:Token at [9, 35]: identifier
D: Val: n1
D:Token at [9, 37]: right parenthese
D:Token at [10, 3]: return
D:Token at [10, 10]: identifier
D: Val: n
D:Token at [10, 12]: multiply
D:Token at [10, 14]: identifier
D: Val: tmp
D:Token at [11, 2]: end
D:Token at [12, 1]: end
D:Token at [14, 1]: function
D:Token at [14, 10]: identifier
D: Val: main
D:Token at [14, 14]: left parenthese
D:Token at [14, 15]: right parenthese
D:Token at [15, 2]: identifier
D: Val: write
D:Token at [15, 7]: left parenthese
D:Token at [15, 8]: string literal
D: Val: "Zadejte cislo pro vypocet faktorialu: "
D:Token at [15, 48]: right parenthese
D:Token at [16, 2]: local
D:Token at [16, 8]: identifier
D: Val: a
D:Token at [16, 10]: colon
D:Token at [16, 12]: integer
D:Token at [16, 20]: assignment
D: Val: =
D:Token at [16, 22]: identifier
D: Val: readi
D:Token at [16, 27]: left parenthese
D:Token at [16, 28]: right parenthese
D:Token at [17, 2]: if
D:Token at [17, 5]: identifier
D: Val: a
D:Token at [17, 7]: not equality
D:Token at [17, 8]: assignment
D: Val: =
D:Token at [17, 10]: nil
D:Token at [17, 14]: then
D:Token at [18, 3]: if
D:Token at [18, 6]: identifier
D: Val: a
D:Token at [18, 8]: less then comparison
D: Val: <
D:Token at [18, 10]: integer literal
D: Val: 0
D:Token at [18, 12]: then
D:Token at [19, 4]: identifier
D: Val: write
D:Token at [19, 9]: left parenthese
D:Token at [19, 10]: string literal
D: Val: "Faktorial nejde spocitat!"
D:Token at [19, 37]: comma
D:Token at [19, 39]: string literal
D: Val: "\n"
D:Token at [19, 43]: right parenthese
D:Token at [20, 3]: else
D:Token at [21, 4]: local
D:Token at [21, 10]: identifier
D: Val: vysl
D:Token at [21, 15]: colon
D:Token at [21, 17]: integer
D:Token at [21, 25]: assignment
D: Val: =
D:Token at [21, 27]: identifier
D: Val: factorial
D:Token at [21, 36]: left parenthese
D:Token at [21, 37]: identifier
D: Val: a
D:Token at [21, 38]: right parenthese
D:Token at [22, 4]: identifier
D: Val: write
D:Token at [22, 9]: left parenthese
D:Token at [22, 10]: string literal
D: Val: "Vysledek je "
D:Token at [22, 24]: comma
D:Token at [22, 26]: identifier
D: Val: vysl
D:Token at [22, 30]: comma
D:Token at [22, 32]: string literal
D: Val: "\n"
D:Token at [22, 36]: right parenthese
D:Token at [23, 3]: end
D:Token at [24, 2]: else
D:Token at [25, 3]: identifier
D: Val: write
D:Token at [25, 8]: left parenthese
D:Token at [25, 9]: string literal
D: Val: "Chyba pri nacitani celeho cisla!\n"
D:Token at [25, 45]: right parenthese
D:Token at [26, 2]: end
D:Token at [27, 1]: end
D:Token at [29, 1]: identifier
D: Val: main
D:Token at [29, 5]: left parenthese
D:Token at [29, 6]: right parenthese
D:Token at [30, 0]: eof
