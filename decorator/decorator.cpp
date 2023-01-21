#include <iostream>
#include <memory>
#include <string>
#include <algorithm>

using namespace std;

struct ILogger
{
    virtual ~ILogger() = default;
    virtual void Log(const string& msg) const = 0;
};

struct SimpleLogger : public ILogger
{
    void Log(const string& msg) const override
    {
        cout << msg << endl;
    }
};

struct LoggerDecorator : public ILogger
{
    LoggerDecorator(unique_ptr<ILogger>&& logger)
        : m_logger(move(logger))
    {
    }

    void Log(const string& msg) const override
    {
        m_logger->Log(msg);
    }

private:
    unique_ptr<ILogger> m_logger;
};

struct TrimLoggerDecorator : public LoggerDecorator
{
    TrimLoggerDecorator(unique_ptr<ILogger>&& logger)
        : LoggerDecorator(move(logger))
    {
    }

    void Log(const string& msg) const override
    {
        std::string res;
        size_t i = find_if(msg.begin(), msg.end(),
            [](unsigned char ch) { return!isspace(ch); }) - msg.begin();

        while (i < msg.size())
        {
            if (isspace(msg[i]))
            {
                while (i < msg.size() && isspace(msg[++i]));
                if (i < msg.size() - 1)
                    res += ' ';
            }

            res += msg[i++];
        }

        LoggerDecorator::Log(res);
    }
};

struct SquareBracketsLogger : public LoggerDecorator
{
    SquareBracketsLogger(unique_ptr<ILogger>&& logger)
        : LoggerDecorator(move(logger))
    {
    }
    void Log(const string& msg) const override
    {
        string modifiedMsg = "[";
        (modifiedMsg += msg) += ']';
        LoggerDecorator::Log(modifiedMsg);
    }
};

int main()
{
    unique_ptr<ILogger> simpleLogger = make_unique<SimpleLogger>();
    simpleLogger->Log("   A   ABC     ");

    unique_ptr<ILogger> trimLogger =  make_unique<TrimLoggerDecorator>(move(simpleLogger));
    trimLogger->Log("   A   ABC   ");

    unique_ptr<ILogger> squareBracketsLogger = make_unique<SquareBracketsLogger>(make_unique<SimpleLogger>());
    squareBracketsLogger->Log("   A    ABC   ");

    unique_ptr<ILogger> squareBracketsTrimLogger = make_unique<TrimLoggerDecorator>(move(squareBracketsLogger));
    squareBracketsTrimLogger->Log("   A   ABC      ");

    return 0;
}
