#ifndef CALCPARSER_H
#define CALCPARSER_H
#include <QString>
#include <QStack>
#include <QtMath>


class calcparser
{
private:
    QString *expression; // указатель на строку - обрабатываемое выражение/ возврат результата
    QString *error; // указатель на строку для возвращения ошибок
    QString workString;
    QVector<char> allowedChars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(', ')',
                                  '=', '*', '/', '-', '+', 'c', 'o', 's', 'i', 'n', 'x', '.'}; // разрешенные символы в выражении
public:
    calcparser(QString *_expression, QString *_error);
    ~calcparser();
    void init(); // проверка скобок, проверка сивмолов. инициализация
    void parseEquation(); // решение квадратных уравнений
    qreal parseExpression(QString exprInBrackets); // метод вычисляющий qreal значение текстогого арифмитического выражения любой длины без скобок
    qreal parseWithBrackets(QString exprWithBrackets); // метод вычисляющий арифмитическое выражение с тригон. функциями и скобками.

};

#endif // CALCPARSER_H
