#include "generator.h"

// #include <iostream>
#include <vector>
#include <cmath>

Generator::Options Generator::opt;

Generator::Generator(QObject *parent)
    : QObject{parent}
{}

QString Generator::generatePassword(int length)
{
    std::vector<std::function<char()>> generators;
    int poolSize = 0;

    if (opt.lower) {
        generators.push_back(genLower);
        poolSize += LOWER_NUM;
    }
    if (opt.upper) {
        generators.push_back(genUpper);
        poolSize += UPPER_NUM;
    }
    if (opt.numbers) {
        generators.push_back(genNumber);
        poolSize += NUMBER_NUM;
    }
    if (opt.symbols) {
        generators.push_back(genSymbol);
        poolSize += SYMBOL_NUM;
    }
    if (generators.empty()) {
        generators.push_back(genLower);
        poolSize += LOWER_NUM;
    }

    std::string password;
    int nGeneratons = static_cast<int>(generators.size());
    for (int i = 0; i < length; ++i) {
        password += generators[QRandomGenerator::global()->bounded(0, nGeneratons)]();
    }

    emit entropyCalculated( getPasswordEntropy(length, poolSize) );
    emit passwordGenerated(QString::fromStdString(password));
    return QString::fromStdString(password);
}

int Generator::getPasswordEntropy(int length, int poolSize)
{

    // E = log2(S^L)
    // Where S is the size of the pool of unique possible symbols (character set)
    // and L is the password length or number of symbols in the password
    return (int)std::ceil(std::log2(std::pow(poolSize, length)));
}

void Generator::copy(const QString &text) const
{
    qGuiApp->clipboard()->setText(text);
}

void Generator::onTextEdited(const QString &text)
{
    Options opt {false, false, false, false};
    int length = text.size();
    int poolSize = 0;

    for (auto c : text) {
        int ic = static_cast<int>(c.toLatin1());
        // is lower?
        if (!opt.lower) {
            if (ic >= 97 && ic <= 122) {
                opt.lower = true;
                poolSize += LOWER_NUM;
            }
        }
        // is upper?
        if (!opt.upper) {
            if (ic >= 65 && ic <= 90) {
                opt.upper = true;
                poolSize += UPPER_NUM;
            }
        }
        // is number?
        if (!opt.numbers) {
            if (ic >= 48 && ic <= 57) {
                opt.numbers = true;
                poolSize += NUMBER_NUM;
            }
        }
        // is symbol?
        if (!opt.symbols) {
            for (int i = 0; i < SYMBOL_NUM; ++i) {
                if (ic == SYMBOL_SET[i]) {
                    opt.symbols = true;
                    poolSize += SYMBOL_NUM;
                    break;
                }
            }
        }
    }

    emit entropyCalculated( getPasswordEntropy(length, poolSize) );
}
