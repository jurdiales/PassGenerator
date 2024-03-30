#include "generator.h"

#include <vector>
#include <cmath>
#include <algorithm>

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

    emit entropyCalculated( getPasswordEntropy(password, poolSize) );
    emit passwordGenerated(QString::fromStdString(password));
    return QString::fromStdString(password);
}

double Generator::getPasswordEntropy(const std::string &password, int poolSize)
{
    double effective_length = 0.;
    // decrease entropy if there are repeated numbers or letters
    int repeated = 0;
    for (std::size_t i = 0; i < password.length(); ++i) {
        // count for repetitions of character i
        int ireps = 0;
        for (std::size_t j = i + 1; j < password.length(); ++j) {
            // special characters not count for this
            auto beg = std::cbegin(SYMBOL_SET);
            auto end = std::cend(SYMBOL_SET);
            const int value = static_cast<int>(password[i]);
            if ((std::find(beg, end, value) == end) && (password[i] == password[j])) {
                ++repeated;
                ++ireps;
                continue;
            }
            // if the next is not repeated, there is no need to continue
            break;
        }
        // to not count several times the same repeated characters
        i += ireps;
        (ireps == 0) ? effective_length += 1.0 : effective_length += getEffectiveLength(ireps);
    }

    // E = log2(S^L)
    // Where S is the size of the pool of unique possible symbols (character set)
    // and L is the password length or number of symbols in the password
    return std::log2(std::pow((double)poolSize, effective_length));
}

double Generator::getEffectiveLength(int reps)
{
    if (reps == 0) return 1.0;

    double effLength = 1.0;
    for (unsigned int i = 1; i <= reps; ++i) {
        effLength += 1.0 / (i + 1.0);
    }
    return effLength;
}

void Generator::copy(const QString &text) const
{
    qGuiApp->clipboard()->setText(text);
}

void Generator::onTextEdited(const QString &text)
{
    Options opt {false, false, false, false};
    int poolSize = 0;

    for (const auto& c : text) {
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

    emit entropyCalculated( getPasswordEntropy(text.toStdString(), poolSize) );
}
