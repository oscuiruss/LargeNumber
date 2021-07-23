Лабораторная работа 4
Длинная арифметика
Цель работы: изучить особенности работы с классами в C++.
Описание:
Стандарт С++17.
В программе должен быть реализован класс LN, позволяющий выполнять арифметические операции над целыми числами произвольной точности в десятичной системе. С его использованием должна проводиться работа с входными и выходными файлами.
Входной файл содержит выражение в форме обратной польской записи. Каждое число и знак операции (‘+’, ‘-’, ‘*’, ‘/’, ‘%’ – остаток от деления, ‘~’ – корень квадратный, ‘_’ – унарный минус, ‘<’, ‘<=’, ‘>’, ‘>=’, ‘==’, ‘!=’) располагается на отдельной строке. Каждая строка оканчивается символом новой строки.
Выходной файл должен содержать состояние стека на момент завершения работы программы. Каждое значение находится на новой строке, начиная с вершины, строка оканчивается символом новой строки.
Квадратный корень и деление округляют к 0. Результат при взятии корня из отрицательного числа или делении на ноль: NaN. Результат операций сравнения: 0 (для false) или 1 (для true). Результат арифметических действий и сравнения с NaN: в соответствии со стандартом IEEE-754. Число -0 должно быть полностью эквивалентно 0 (включая вывод).
Аргументы программе передаются через командную строку:
lab4 <имя_входного_файла> <имя_выходного_файла>
Нехватка памяти при операциях с классом должна обрабатываться через исключения C++.
Класс должен иметь конструкторы:
конструктор копирования,
конструктор перемещения,
из long long со значением по умолчанию: 0,
из строки символов C (const char *),
из строки std::string_view.
Для класса должны быть реализованы операторы:
арифметические: +, -, *, /, %, ~ (как корень квадратный), - (унарный);
комбинация арифметических операций и присваивания;
сравнения: <, <=, >, >=, ==, !=;
присваивания;
перемещения;
преобразования типа в: long long (с генерацией исключения в случае, когда значение не умещается), bool (неравенство нулю);
создания из литерала произвольной длины с суффиксом _ln (например, должно работать выражение: LN x; x = 123_ln;).
Реализовать вспомогательные функции/методы: сложение, вычитание, универсальное сравнение (возвращает -1, 0, 1), которые будут использоваться в функциях/методах оператор ‘+’, оператор ‘<’ и пр. Например, при реализации и оператора ‘+’, и оператора ‘-’ необходимо выполнять и сложение, и вычитание, в зависимости от знаков входных чисел

