#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QRandomGenerator>
#include <QGuiApplication>
#include <QClipboard>

class Generator : public QObject
{
    Q_OBJECT
public:
    enum PasswordEntropy {
        UNKNOWN,
        WEAK,
        MEDIUM,
        STRONG
    };

    struct Options {
        bool upper = false;
        bool lower = true;
        bool numbers = false;
        bool symbols = false;
        PasswordEntropy entropy = UNKNOWN;
    };

    explicit Generator(QObject *parent = nullptr);

    Q_INVOKABLE QString generatePassword(int length);
    inline double getPasswordEntropy(const std::string& password, int poolSize);
    inline double getEffectiveLength(int reps);
    Q_INVOKABLE void copy(const QString& text) const;
    Q_INVOKABLE bool isWeakPassword() { return (opt.entropy == WEAK); }

    static Options opt;

private:
    static constexpr int SYMBOL_SET[] = {
        33,  // Exclamation mark (!)
        34,  // Double quotes (")
        35,  // Number sign (#)
        36,  // Dollar sign ($)
        37,  // Percent sign (%)
        38,  // Ampersand (&)
        39,  // Apostrophe (')
        40,  // Left parenthesis (
        41,  // Right parenthesis )
        42,  // Asterisk (*)
        43,  // Plus sign (+)
        44,  // Comma (,)
        45,  // Hyphen (-)
        46,  // Period (.)
        47,  // Forward slash (/)
        58,  // Colon (:)
        59,  // Semicolon (;)
        60,  // Less than sign (<)
        61,  // Equals sign (=)
        62,  // Greater than sign (>)
        63,  // Question mark (?)
        64,  // At sign (@)
        91,  // Left square bracket ([)
        92,  // Backslash (\)
        93,  // Right square bracket (])
        94,  // Caret (^)
        95,  // Underscore (_)
        96,  // Grave accent (` backtick)
        123, // Left curly brace ({)
        124, // Vertical bar (|)
        125, // Right curly brace (})
        126  // Tilde (~)
    };
    static constexpr int SYMBOL_NUM = sizeof(SYMBOL_SET) / sizeof(SYMBOL_SET[0]);
    static constexpr int UPPER_NUM = 26;
    static constexpr int LOWER_NUM = 26;
    static constexpr int NUMBER_NUM = 10;

    static constexpr auto genUpper = []() { return static_cast<char>(QRandomGenerator::global()->bounded(65, 91)); };
    static constexpr auto genLower = []() { return static_cast<char>(QRandomGenerator::global()->bounded(97, 123)); };
    static constexpr auto genNumber = []() { return static_cast<char>(QRandomGenerator::global()->bounded(48, 58)); };
    static constexpr auto genSymbol = []() { return static_cast<char>(SYMBOL_SET[QRandomGenerator::global()->bounded(0, SYMBOL_NUM)]); };

signals:
    void passwordGenerated(QString password);
    void entropyCalculated(double entropy);

public slots:
    inline void onLowerChanged(bool checked)  { opt.lower = checked; }
    inline void onUpperChanged(bool checked)  { opt.upper = checked; }
    inline void onNumberChanged(bool checked) { opt.numbers = checked; }
    inline void onSymbolChanged(bool checked) { opt.symbols = checked; }

    void onTextEdited(const QString& text);
};

#endif // GENERATOR_H
