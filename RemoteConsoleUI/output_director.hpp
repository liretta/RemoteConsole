#ifndef OUTPUT_DIRECTOR_H
#define OUTPUT_DIRECTOR_H

#ifndef QDEBUGSTREAM_H
#define QDEBUGSTREAM_H

#include <iostream>
#include <streambuf>
#include <string>
#include <QPlainTextEdit>

class OutputStream : public std::basic_streambuf<char>
{
public:
    OutputStream(std::ostream &stream, QPlainTextEdit* text_edit)
        : m_stream(stream)
    {
        m_log_window = text_edit;
        m_old_buf = stream.rdbuf();
        stream.rdbuf(this);
    }
    ~OutputStream()
    {
        // output anything that is left
        if (!m_string.empty())
            m_log_window->appendPlainText(m_string.c_str());

        m_stream.rdbuf(m_old_buf);
    }

protected:
    virtual int_type overflow(int_type v)
    {
        if (v == '\n')
        {
            m_log_window->appendPlainText(m_string.c_str());
            m_string.erase(m_string.begin(), m_string.end());
        }
        else
            m_string += v;

        return v;
    }

    virtual std::streamsize xsputn(const char *p, std::streamsize n)
    {
        m_string.append(p, p + n);

        int pos = 0;
        while (pos != std::string::npos)
        {
            pos = m_string.find('\n');
            if (pos != std::string::npos)
            {
                std::string tmp(m_string.begin(), m_string.begin() + pos);
                m_log_window->appendPlainText(tmp.c_str());
                m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
            }
        }

        return n;
    }

private:
    std::ostream&   m_stream;
    std::streambuf* m_old_buf;
    std::string     m_string;


    QPlainTextEdit* m_log_window;
};

#endif

#endif // OUTPUT_DIRECTOR_H
