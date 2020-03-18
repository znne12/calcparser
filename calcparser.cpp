#include "calcparser.h"


calcparser::calcparser(QString *_expression, QString *_error) : expression(_expression), error(_error)
{
    workString = *expression;
    workString = workString.toLower();
}

calcparser::~calcparser(){}

void calcparser::init(){
    QString res;
    QStack<char> bracket;
    qint32 result = 0;
    bool charAccepted;
    for(int i = 0; i < workString.size(); i++){
        if (workString[i] == ' ') workString.remove(i, 1); // удаление пробелов
        charAccepted = false;
        for (auto now : allowedChars) {
            if (workString[i] == now) charAccepted = true; //проверка каждого символа на вхождение в допустимые
        }
        if (charAccepted == false) {
            *error = "Incorrect symbol: " + workString[i];
            return;
        }
        if (workString[i] == '(') bracket.push_back('(');
        if (workString[i] == ')' && bracket.isEmpty() == 0) bracket.pop_back();  // проверка правильности скобок
        else if (workString[i] == ')' && bracket.isEmpty() != 0) {
            *error = "Incorrect bracket: " + workString[i];
            return;
        }
        if (workString[i] == 'x' && result == 0) result = 2;
    }
    if (result == 2)
        for (auto now : workString)
            if (now == 'c' && now == 'o' && now == 's' && now == 'i' && now == 'n') { // если кв. уравнение, то проверить на наличия символов c, o, s, i, n
                *error = "Incorrect symbol in equation";
                return;
            }
        switch(result){
        case 0:
        case 1: res.setNum(parseWithBrackets(workString));
                *expression = res;
                break;
        case 2: parseEquation();
        case 3: return;
    }
}

void calcparser::parseEquation(){
    qreal a = 0, b = 0, c = 0, disc = 0;
    QString tmp;
    bool error_inp = true;
       do{
            tmp.push_back(workString.front());  //считываем цифры
            workString.remove(0, 1);
            if (workString.front() == '+' || workString.front() == '-' || workString.size() == 0) {
                if (tmp.count('x') == 2) {
                    tmp.chop(2);
                    a = tmp.toDouble(&error_inp);
                }else if (tmp.count('x') == 1){
                    tmp.chop(1);
                    b = tmp.toDouble(&error_inp);
                }else {
                    c = tmp.toDouble(&error_inp);
                }
                tmp.clear();
            }
            if (!error_inp) {
                *error = "Please type equation like this: 12xx+54x-4565.3412 =";
                return;
            }
        }while(workString.size() != 0);
    disc = qPow(b, 2) - 4 * a * c;
        if (disc < 0) {
            *expression = "0  The equation has no solutions";
        } else if (disc == 0) {
            *expression = "0 , x1=" + QString :: number(-(b + qSqrt(disc)) / (2 * a));
        } else if (a == 0) {
            *expression = "0 , x1=" + QString :: number(c / b);
        } else {
         *expression = "0 , x1=" + QString :: number(-(b + qSqrt(disc)) / (2 * a)) +
         ", x2=" + QString :: number(-(b - qSqrt(disc)) / (2 * a));
        }
}

qreal calcparser::parseExpression(QString exprInBrackets){
    QString tmp;              // временная строка для считывания в нее текущего числа
    qreal num1 = 0;           // переменная для запоминания предыдущего числа при нахождении символов * или /
    bool multiply = false;    // флаг на необходимость умножения только что считаного числа
    bool divide = false;      // флаг на необходимость деления только что считанного числа
    bool error_inp = true;    // флаг показывающий  в случае преобразования Qstring to Double с ошибкой
    qreal result = 0;
    for (int i = 0; i <= exprInBrackets.size(); i++){
        if (!tmp.isEmpty() && (exprInBrackets[i] == '+' || exprInBrackets[i] == '-' || i == exprInBrackets.size())) {
            if (multiply) {  // если флаг умножения тру, то умножить переменную num1 на текущее число tmp и добавить в result
                result += num1 * tmp.toDouble();
                num1 = 0;
                multiply = false;
            } else if(divide){  // если divide=true, то разделить переменную num1 на текущее число tmp и добавить в result
                result += num1 / tmp.toDouble();
                num1 = 0;
                divide = false;
            }else { // отнять или добавить текущее число в result
                result += tmp.toDouble();
            }
            tmp.clear();
        }
        if (!tmp.isEmpty() && exprInBrackets[i] == '*' ) {
            if(multiply){     //если флаг умножения тру, то умножить число num1 на текущее и положить в num1
                num1 *= tmp.toDouble();
                tmp.clear();
            }else if (divide){//если флаг удаления тру, то разделить число num1 на текущее и результат положить в num1
                if (tmp.toDouble() == 0) {
                        *error = "Can't divide by zero!";
                        return 0;
                    }
                num1 /= tmp.toDouble();
                tmp.clear();
                divide = false;
                multiply = true;
            }else{ //если флаги удаления/умножение false значит это первое число для проведения умножения
                multiply = true;
                num1 = tmp.toDouble();
                tmp.clear();
            }
            i++;
        }
        if (!tmp.isEmpty() && exprInBrackets[i] == '/') {
            if(divide){
                if (tmp.toDouble() == 0) {
                        *error = "Can't divide by zero!";
                        return 0;
                    }
                num1 /= tmp.toDouble();
                tmp.clear();
            }else if (multiply){
                num1 *= tmp.toDouble();
                tmp.clear();
                multiply = false;
                divide = true;
            }else{      //если флаги удаления/умножение false значит это первое число для проведения деления
                divide = true;
                num1 = tmp.toDouble();
                tmp.clear();
            }
            i++;
        }
        if ((exprInBrackets[i] != '/' || exprInBrackets[i] != '*') && i < exprInBrackets.size())   tmp.push_back(exprInBrackets[i]);// считываем текущий символ
        if (!error_inp) {
            *error = "Error in expression";
            return 0;
        }
    }
    return result;
}

qreal calcparser::parseWithBrackets(QString exprWithBrackets){
    do{
        qint32 rigthBracket = exprWithBrackets.indexOf(")", 0);// ищем первую правую скобку
        qint32 leftBracket = exprWithBrackets.lastIndexOf("(", rigthBracket);// от первой правой скобки ищем назад первую левую скобку

        if (leftBracket == 0) {
            exprWithBrackets.remove(0, 1);
            exprWithBrackets.chop(1);
        } else if (rigthBracket == -1) break;
        else if (exprWithBrackets[leftBracket - 1] < 'a' || exprWithBrackets[leftBracket - 1] > 'z' ){// если перед левой скобкой не буква, то обрабатываем parseExpression,
            QString res;                                                                         // то что нашли между скобками и вставляем в обрабатываемую строку
            res.setNum(parseExpression(exprWithBrackets.mid(leftBracket + 1, rigthBracket - leftBracket - 1)));
            exprWithBrackets = exprWithBrackets.replace(leftBracket, rigthBracket - leftBracket + 1, res);
        } else{
            if (exprWithBrackets.mid(leftBracket - 3, 3) == "cos"){ // если нашли перед левой скобкой cos, то обрабатываем с косинусом
                QString res;
                res.setNum(qCos(qDegreesToRadians(parseExpression(exprWithBrackets.mid(leftBracket + 1, rigthBracket - leftBracket - 1)))));
                exprWithBrackets = exprWithBrackets.replace(leftBracket - 3, rigthBracket - leftBracket + 4, res);
            }else if (exprWithBrackets.mid(leftBracket - 3, 3) == "sin"){
                QString res;
                res.setNum(qSin(qDegreesToRadians(parseExpression(exprWithBrackets.mid(leftBracket + 1, rigthBracket - leftBracket - 1)))));
                exprWithBrackets = exprWithBrackets.replace(leftBracket - 3, rigthBracket - leftBracket +4, res);
            }
        }
    }while(exprWithBrackets.count('(') != 0 && exprWithBrackets.count(')') != 0 ); // делаем пока в строке есть скобки
    return parseExpression(exprWithBrackets);
}

